#include "PatientManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlRecord>
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QSqlQuery>
#include <QUuid>
#include <QStyledItemDelegate>
#include <QRegularExpressionValidator>

class DateDelegate : public QStyledItemDelegate {
public:
    DateDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
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
        if (dateEdit) {
            dateEdit->setDate(QDate::fromString(index.data(Qt::EditRole).toString(), "yyyy-MM-dd"));
        }
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
        QDateEdit *dateEdit = qobject_cast<QDateEdit *>(editor);
        if (dateEdit) {
            model->setData(index, dateEdit->date().toString("yyyy-MM-dd"), Qt::EditRole);
        }
    }
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const override {
        editor->setGeometry(option.rect);
    }
};

class GenderDelegate : public QStyledItemDelegate {
public:
    GenderDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"Nam", "Nữ", "Khác"});
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

PatientManager::PatientManager(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadData();
}

void PatientManager::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *topLayout = new QHBoxLayout();
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Tìm kiếm bệnh nhân...");
    searchBox->setMinimumWidth(300);
    
    QPushButton *btnAdd = new QPushButton("Thêm Bệnh Nhân", this);
    btnAdd->setObjectName("btnPrimary");
    QPushButton *btnDelete = new QPushButton("Xóa Bệnh Nhân", this);
    btnDelete->setObjectName("btnDanger");

    topLayout->addWidget(searchBox);
    topLayout->addStretch();
    topLayout->addWidget(btnAdd);
    topLayout->addWidget(btnDelete);

    mainLayout->addLayout(topLayout);

    tableView = new QTableView(this);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setAlternatingRowColors(false);
    tableView->verticalHeader()->setVisible(false);
    tableView->verticalHeader()->setDefaultSectionSize(45);
    tableView->setSortingEnabled(true);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    mainLayout->addWidget(tableView);

    connect(searchBox, &QLineEdit::textChanged, this, &PatientManager::onSearchTextChanged);
    connect(btnAdd, &QPushButton::clicked, this, &PatientManager::onAddPatient);
    connect(btnDelete, &QPushButton::clicked, this, &PatientManager::onDeletePatient);
}

void PatientManager::loadData() {
    model = new QSqlTableModel(this);
    model->setTable("Patients");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    
    model->setHeaderData(1, Qt::Horizontal, "Mã BN");
    model->setHeaderData(2, Qt::Horizontal, "Họ Tên");
    model->setHeaderData(3, Qt::Horizontal, "Ngày Sinh");
    model->setHeaderData(4, Qt::Horizontal, "Giới Tính");
    model->setHeaderData(5, Qt::Horizontal, "CCCD");
    model->setHeaderData(6, Qt::Horizontal, "Địa Chỉ");
    model->setHeaderData(7, Qt::Horizontal, "SĐT");
    model->setHeaderData(8, Qt::Horizontal, "BHYT");

    model->select();

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1); 

    tableView->setModel(proxyModel);
    tableView->hideColumn(0); 

    tableView->setItemDelegateForColumn(3, new DateDelegate(this));
    tableView->setItemDelegateForColumn(4, new GenderDelegate(this));

    QHeaderView *header = tableView->horizontalHeader();
    header->setStretchLastSection(false);
    header->setSectionResizeMode(QHeaderView::Interactive);
    // Độ rộng cứng phải lớn thì thanh cuộn ngang mới kích hoạt
    tableView->setColumnWidth(1, 100); 
    tableView->setColumnWidth(2, 400); // Họ tên 
    tableView->setColumnWidth(3, 150); 
    tableView->setColumnWidth(4, 100); 
    tableView->setColumnWidth(5, 200); // CCCD
    tableView->setColumnWidth(6, 450); // Địa chỉ
    tableView->setColumnWidth(7, 200); // SDT
    tableView->setColumnWidth(8, 200); // BHYT
}

void PatientManager::onSearchTextChanged(const QString &text) {
    proxyModel->setFilterFixedString(text);
}

void PatientManager::onAddPatient() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Bệnh Nhân");
    QFormLayout formLayout(&dialog);
    formLayout.setSpacing(15);
    formLayout.setSizeConstraint(QLayout::SetFixedSize); // Wrapper co giãn vừa phải
    
    QLineEdit *editHoTen = new QLineEdit(&dialog);
    editHoTen->setPlaceholderText("VD: Nguyễn Văn A");
    
    QDateEdit *editNgaySinh = new QDateEdit(QDate::currentDate(), &dialog);
    editNgaySinh->setDisplayFormat("yyyy-MM-dd");
    editNgaySinh->setCalendarPopup(true);
    editNgaySinh->setMaximumDate(QDate::currentDate());
    
    QComboBox *comboGioiTinh = new QComboBox(&dialog);
    comboGioiTinh->addItems({"Nam", "Nữ", "Khác"});
    
    QLineEdit *editCCCD = new QLineEdit(&dialog);
    editCCCD->setPlaceholderText("Chỉ chứa số");
    QRegularExpression rxNum("^[0-9]+$");
    editCCCD->setValidator(new QRegularExpressionValidator(rxNum, &dialog));
    
    QLineEdit *editDiaChi = new QLineEdit(&dialog);
    editDiaChi->setPlaceholderText("Số nhà, Đường, Quận/Huyện, Tỉnh/TP");
    editDiaChi->setMinimumWidth(300);
    
    QLineEdit *editSDT = new QLineEdit(&dialog);
    editSDT->setPlaceholderText("Chỉ chứa số - VD: 0912345678");
    editSDT->setValidator(new QRegularExpressionValidator(rxNum, &dialog));
    
    QLineEdit *editBHYT = new QLineEdit(&dialog);
    editBHYT->setPlaceholderText("Mã bảo hiểm y tế nếu có");

    formLayout.addRow("Họ Tên:", editHoTen);
    formLayout.addRow("Ngày Sinh:", editNgaySinh);
    formLayout.addRow("Giới Tính:", comboGioiTinh);
    formLayout.addRow("CCCD:", editCCCD);
    formLayout.addRow("Địa Chỉ:", editDiaChi);
    formLayout.addRow("SĐT:", editSDT);
    formLayout.addRow("Mã BHYT:", editBHYT);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btnSave = new QPushButton("Đồng ý", &dialog);
    btnSave->setObjectName("btnPrimary");
    btnSave->setMinimumHeight(40);
    QPushButton *btnCancel = new QPushButton("Hủy", &dialog);
    btnCancel->setMinimumHeight(40);
    
    buttonsLayout->addWidget(btnSave);
    buttonsLayout->addWidget(btnCancel);
    formLayout.addRow(buttonsLayout);

    connect(btnSave, &QPushButton::clicked, [&]() {
        if(editHoTen->text().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập họ tên!");
            return;
        }
        QString maBN = QUuid::createUuid().toString().left(8).remove("{");
        
        QSqlQuery q;
        q.prepare("INSERT INTO Patients (ma_benh_nhan, ho_ten, ngay_sinh, gioi_tinh, cccd, dia_chi, sdt, bhyt) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        q.addBindValue(maBN.toUpper());
        q.addBindValue(editHoTen->text());
        q.addBindValue(editNgaySinh->date().toString("yyyy-MM-dd"));
        q.addBindValue(comboGioiTinh->currentText());
        q.addBindValue(editCCCD->text());
        q.addBindValue(editDiaChi->text());
        q.addBindValue(editSDT->text());
        q.addBindValue(editBHYT->text());
        
        if(q.exec()) {
            model->select(); 
            dialog.accept();
        } else {
            QMessageBox::critical(&dialog, "Lỗi DB", "Không thể thêm hoặc trùng CCCD!");
        }
    });

    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void PatientManager::onDeletePatient() {
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if(selection.isEmpty()) return;
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Xác nhận");
    msgBox.setText("Bạn có chắc chắn muốn xóa bệnh nhân? Dữ liệu hàng sẽ bị xóa hoàn toàn khỏi Database.");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.button(QMessageBox::Yes)->setText("Xác Nhận");
    msgBox.button(QMessageBox::No)->setText("Hủy");
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        int row = proxyModel->mapToSource(selection.at(0)).row();
        QString maBN = model->data(model->index(row, 1)).toString();
        
        QSqlQuery delAdm;
        delAdm.prepare("DELETE FROM Admissions WHERE ma_benh_nhan = ?");
        delAdm.addBindValue(maBN);
        delAdm.exec();

        QSqlQuery delQuery;
        delQuery.prepare("DELETE FROM Patients WHERE ma_benh_nhan = ?");
        delQuery.addBindValue(maBN);
        if(delQuery.exec()) {
            model->select();
        }
    }
}
