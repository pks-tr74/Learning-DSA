#include "AdmissionManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QSqlQuery>
#include <QComboBox>
#include <QDateEdit>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QStyledItemDelegate>

class AdmissionDateDelegate : public QStyledItemDelegate {
public:
    AdmissionDateDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override {
        QDateEdit *editor = new QDateEdit(parent);
        editor->setDisplayFormat("yyyy-MM-dd");
        editor->setCalendarPopup(true);
        editor->setMaximumDate(QDate::currentDate());
        editor->setStyleSheet("color: black; background: white; border: none; padding: 0px;");
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QDateEdit *dateEdit = qobject_cast<QDateEdit *>(editor);
        if (dateEdit) dateEdit->setDate(QDate::fromString(index.data(Qt::EditRole).toString(), "yyyy-MM-dd"));
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        QDateEdit *dateEdit = qobject_cast<QDateEdit *>(editor);
        if (dateEdit) model->setData(index, dateEdit->date().toString("yyyy-MM-dd"), Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const override {
        editor->setGeometry(option.rect);
    }
};

class StatusComboDelegate : public QStyledItemDelegate {
public:
    StatusComboDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"Đang Điều Trị", "Chuyển Đang Phẫu Thuật", "Đã Xuất Viện"});
        editor->setStyleSheet(R"(
            QComboBox { color: black; background: white; border: none; padding: 0px; }
            QComboBox QAbstractItemView { background-color: white; selection-background-color: #3A7AFE; selection-color: white; outline: none; }
            QComboBox QAbstractItemView::item { color: black; }
            QComboBox QAbstractItemView::item:hover { background-color: rgba(0,0,0,0.1); color: black; }
        )");
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const override {
        QComboBox *cb = qobject_cast<QComboBox *>(editor);
        if (cb) cb->setCurrentText(index.data(Qt::EditRole).toString());
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        QComboBox *cb = qobject_cast<QComboBox *>(editor);
        if (cb) model->setData(index, cb->currentText(), Qt::EditRole);
    }
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const override {
        editor->setGeometry(option.rect);
    }
};

AdmissionManager::AdmissionManager(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadData();
}

void AdmissionManager::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *topLayout = new QHBoxLayout();
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Tìm kiếm hồ sơ nhập viện...");
    
    QPushButton *btnAdd = new QPushButton("Tạo Hồ Sơ Mới", this);
    btnAdd->setObjectName("btnPrimary");
    
    QPushButton *btnDelete = new QPushButton("Xóa Hồ Sơ", this);
    btnDelete->setObjectName("btnDanger");

    QPushButton *btnUpdate = new QPushButton("Đổi Trạng Thái", this);

    topLayout->addWidget(searchBox);
    topLayout->addStretch();
    topLayout->addWidget(btnAdd);
    topLayout->addWidget(btnDelete);
    topLayout->addWidget(btnUpdate);

    mainLayout->addLayout(topLayout);

    tableView = new QTableView(this);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->verticalHeader()->setVisible(false);
    tableView->verticalHeader()->setDefaultSectionSize(45);
    tableView->setSortingEnabled(true);
    mainLayout->addWidget(tableView);

    connect(searchBox, &QLineEdit::textChanged, this, &AdmissionManager::onSearchTextChanged);
    connect(btnAdd, &QPushButton::clicked, this, &AdmissionManager::onAddAdmission);
    connect(btnUpdate, &QPushButton::clicked, this, &AdmissionManager::onUpdateStatus);
    
    connect(btnDelete, &QPushButton::clicked, this, [this]() {
        QModelIndexList sel = tableView->selectionModel()->selectedRows();
        if(sel.isEmpty()) return;
        
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Xác nhận");
        msgBox.setText("Bạn có chắc chắn muốn xóa hồ sơ này khỏi dữ liệu hệ thống?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.button(QMessageBox::Yes)->setText("Xác Nhận");
        msgBox.button(QMessageBox::No)->setText("Hủy");
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes) {
            int row = proxyModel->mapToSource(sel.at(0)).row();
            int id = model->data(model->index(row, 0)).toInt();
            
            QSqlQuery del;
            del.prepare("DELETE FROM Admissions WHERE id = ?");
            del.addBindValue(id);
            if(del.exec()) {
                model->select();
            }
        }
    });
}

void AdmissionManager::loadData() {
    model = new QSqlTableModel(this);
    model->setTable("Admissions");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    
    // DB Schema is: id, ma_benh_nhan, ma_phong, ngay_nhap_vien, trang_thai, benh_ly
    // Set Header index correctly according to SQLite table column ordering.
    model->setHeaderData(1, Qt::Horizontal, "Mã Bệnh Nhân");
    model->setHeaderData(2, Qt::Horizontal, "Mã Phòng");
    model->setHeaderData(3, Qt::Horizontal, "Ngày Nhập Viện"); // Index 3
    model->setHeaderData(4, Qt::Horizontal, "Trạng Thái"); // Index 4
    model->setHeaderData(5, Qt::Horizontal, "Bệnh Lý"); // Index 5

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1); 

    tableView->setModel(proxyModel);
    tableView->hideColumn(0); 
    
    // Apply delegates
    tableView->setItemDelegateForColumn(3, new AdmissionDateDelegate(this));
    tableView->setItemDelegateForColumn(4, new StatusComboDelegate(this));

    QHeaderView *header = tableView->horizontalHeader();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(QHeaderView::Interactive); 
    
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    
    tableView->setColumnWidth(1, 150);
    tableView->setColumnWidth(2, 100);
    tableView->setColumnWidth(3, 180);
    tableView->setColumnWidth(4, 250);
    tableView->setColumnWidth(5, 300);
}

void AdmissionManager::onSearchTextChanged(const QString &text) {
    proxyModel->setFilterFixedString(text);
}

void AdmissionManager::onAddAdmission() {
    QDialog dialog(this);
    dialog.setWindowTitle("Tạo Hồ Sơ Nhập Viện Mới");
    QFormLayout form(&dialog);
    form.setSpacing(15);
    form.setSizeConstraint(QLayout::SetFixedSize); // Co lại height cho vừa in không trắng thừa
    
    // Lọc ra các bệnh nhân ĐÃ nhập viện (Đang Điều Trị) để trừ khỏi danh sách thêm mới
    QStringList admittedPatients;
    QSqlQuery qPats("SELECT ma_benh_nhan FROM Admissions WHERE trang_thai = 'Đang Điều Trị'");
    while(qPats.next()) admittedPatients << qPats.value(0).toString();

    QComboBox *eBenhNhan = new QComboBox(&dialog);
    eBenhNhan->setMinimumWidth(300);
    QSqlQuery qBN("SELECT ma_benh_nhan, ho_ten FROM Patients");
    bool hasPats = false;
    while(qBN.next()) {
        QString maBN = qBN.value(0).toString();
        // Bỏ qua nếu bệnh nhân này đang nằm viện
        if(!admittedPatients.contains(maBN)) {
            hasPats = true;
            eBenhNhan->addItem(maBN + " - " + qBN.value(1).toString(), maBN);
        }
    }
    if(!hasPats) eBenhNhan->addItem("Không có Bệnh nhân nào trống", "");

    // Phòng chỉ hiện nếu chứa lượng bệnh nhân < số giường max
    QComboBox *ePhong = new QComboBox(&dialog);
    QSqlQuery qRooms("SELECT ma_phong, ten_phong, so_giuong FROM Rooms");
    bool hasRooms = false;
    while(qRooms.next()) {
        QString maP = qRooms.value(0).toString();
        int maxBeds = qRooms.value(2).toInt();
        
        QSqlQuery qCountPats;
        qCountPats.prepare("SELECT COUNT(*) FROM Admissions WHERE trang_thai = 'Đang Điều Trị' AND ma_phong = ?");
        qCountPats.addBindValue(maP);
        qCountPats.exec();
        int currentPats = 0;
        if(qCountPats.next()) currentPats = qCountPats.value(0).toInt();
        
        if (currentPats < maxBeds) {
            hasRooms = true;
            ePhong->addItem(QString("%1 - %2 (Trống %3/%4)").arg(maP).arg(qRooms.value(1).toString()).arg(maxBeds - currentPats).arg(maxBeds), maP);
        }
    }
    if(!hasRooms) ePhong->addItem("Hết Phòng trống", "");
    
    QLineEdit *eBenhLy = new QLineEdit(&dialog);
    eBenhLy->setPlaceholderText("Chuẩn đoán ban đầu");
    
    QDateEdit *eNgay = new QDateEdit(QDate::currentDate(), &dialog);
    eNgay->setDisplayFormat("yyyy-MM-dd");
    eNgay->setCalendarPopup(true);
    eNgay->setMaximumDate(QDate::currentDate());

    QComboBox *eTrangThai = new QComboBox(&dialog);
    eTrangThai->addItems({"Đang Điều Trị", "Chuyển Đang Phẫu Thuật", "Đã Xuất Viện"});

    form.addRow("Chọn Bệnh Nhân:", eBenhNhan);
    form.addRow("Chọn Phòng:", ePhong);
    form.addRow("Bệnh Lý:", eBenhLy);
    form.addRow("Ngày Nhập:", eNgay);
    form.addRow("Trạng Thái:", eTrangThai);
    
    QHBoxLayout *btns = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("Đồng ý", &dialog);
    btnSave->setObjectName("btnPrimary");
    btnSave->setMinimumHeight(40);
    QPushButton *btnCancel = new QPushButton("Hủy", &dialog);
    btnCancel->setMinimumHeight(40);
    btns->addWidget(btnSave);
    btns->addWidget(btnCancel);
    form.addRow(btns);

    connect(btnSave, &QPushButton::clicked, [&]() {
        if(eBenhNhan->currentData().toString().isEmpty() || ePhong->currentData().toString().isEmpty()) {
             QMessageBox::warning(&dialog, "Lỗi", "Bệnh Nhân hoặc Phòng bị trống do hết slot!");
             return;
        }
        
        // Database Columns Map -> id, ma_bn, ma_phong, ngay_nhap_vien, trang_thai, benh_ly
        QSqlQuery q;
        q.prepare("INSERT INTO Admissions (ma_benh_nhan, ma_phong, ngay_nhap_vien, trang_thai, benh_ly) VALUES (?, ?, ?, ?, ?)");
        q.addBindValue(eBenhNhan->currentData().toString());
        q.addBindValue(ePhong->currentData().toString());
        q.addBindValue(eNgay->date().toString("yyyy-MM-dd"));
        q.addBindValue(eTrangThai->currentText());
        q.addBindValue(eBenhLy->text());
        
        if(q.exec()) { model->select(); dialog.accept(); }
        else { QMessageBox::critical(&dialog, "Lỗi DB", "Lỗi nhập SQL: " + q.lastError().text()); }
    });
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void AdmissionManager::onUpdateStatus() {
    QModelIndexList sel = tableView->selectionModel()->selectedRows();
    if(sel.isEmpty()) return;
    
    int row = proxyModel->mapToSource(sel.at(0)).row();
    QString currentStatus = model->record(row).value("trang_thai").toString();
    
    // index 4 corresponds to Trang_Thai in SQLite Schema
    if (currentStatus == "Đang Điều Trị") {
        model->setData(model->index(row, 4), "Đã Xuất Viện");
    } else {
        model->setData(model->index(row, 4), "Đang Điều Trị");
    }
    model->submitAll();
    QMessageBox::information(this, "Thành Công", "Cập nhật trạng thái thành công!");
}
