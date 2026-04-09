#include "DoctorManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QSqlQuery>
#include <QComboBox>
#include <QStyledItemDelegate>

class DoctorComboDelegate : public QStyledItemDelegate {
public:
    DoctorComboDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"Nội Khoa", "Ngoại Khoa", "Nhi Khoa", "Sản Khoa", "Hồi Sức Cấp Cứu", "Da Liễu"});
        editor->setStyleSheet("color: black; background: white; border: none; padding: 0px;");
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

DoctorManager::DoctorManager(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadData();
}

void DoctorManager::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *topLayout = new QHBoxLayout();
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Tìm kiếm bác sĩ...");
    QPushButton *btnAdd = new QPushButton("Thêm Bác Sĩ", this);
    btnAdd->setObjectName("btnPrimary");
    QPushButton *btnDelete = new QPushButton("Xóa Bác Sĩ", this);
    btnDelete->setObjectName("btnDanger");

    topLayout->addWidget(searchBox);
    topLayout->addStretch();
    topLayout->addWidget(btnAdd);
    topLayout->addWidget(btnDelete);

    mainLayout->addLayout(topLayout);

    tableView = new QTableView(this);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->verticalHeader()->setVisible(false);
    tableView->verticalHeader()->setDefaultSectionSize(45);
    tableView->setSortingEnabled(true);
    mainLayout->addWidget(tableView);

    connect(searchBox, &QLineEdit::textChanged, this, &DoctorManager::onSearchTextChanged);
    connect(btnAdd, &QPushButton::clicked, this, &DoctorManager::onAddDoctor);
    connect(btnDelete, &QPushButton::clicked, this, &DoctorManager::onDeleteDoctor);
}

void DoctorManager::loadData() {
    model = new QSqlTableModel(this);
    model->setTable("Doctors");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    
    model->setHeaderData(1, Qt::Horizontal, "Mã Bác Sĩ");
    model->setHeaderData(2, Qt::Horizontal, "Họ Tên");
    model->setHeaderData(3, Qt::Horizontal, "Khoa");
    model->setHeaderData(4, Qt::Horizontal, "Chuyên Môn");
    model->setHeaderData(5, Qt::Horizontal, "SĐT");
    model->setHeaderData(6, Qt::Horizontal, "Email");

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1); 

    tableView->setModel(proxyModel);
    tableView->hideColumn(0); 
    
    tableView->setItemDelegateForColumn(3, new DoctorComboDelegate(this));

    QHeaderView *header = tableView->horizontalHeader();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(QHeaderView::Interactive);
    tableView->setColumnWidth(1, 100); 
    tableView->setColumnWidth(2, 250); 
    tableView->setColumnWidth(3, 150); 
    tableView->setColumnWidth(4, 200);
    tableView->setColumnWidth(6, 250); // Email rộng hơn mã bác sĩ
}

void DoctorManager::onSearchTextChanged(const QString &text) {
    proxyModel->setFilterFixedString(text);
}

void DoctorManager::onAddDoctor() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Bác Sĩ");
    QFormLayout form(&dialog);
    form.setSpacing(15);
    form.setSizeConstraint(QLayout::SetFixedSize);
    
    QLineEdit *eMaBS = new QLineEdit(&dialog);
    eMaBS->setPlaceholderText("Mã ví dụ: BS002");
    QLineEdit *eHoTen = new QLineEdit(&dialog);
    eHoTen->setPlaceholderText("Nguyễn Văn A");
    eHoTen->setMinimumWidth(250);
    
    QComboBox *eKhoa = new QComboBox(&dialog);
    eKhoa->addItems({"Nội Khoa", "Ngoại Khoa", "Nhi Khoa", "Sản Khoa", "Hồi Sức Cấp Cứu", "Da Liễu"});
    
    QLineEdit *eChuyenMon = new QLineEdit(&dialog);
    eChuyenMon->setPlaceholderText("VD: Phẫu thuật tim, Điều dưỡng...");
    QLineEdit *eSDT = new QLineEdit(&dialog);
    QLineEdit *eEmail = new QLineEdit(&dialog);
    eEmail->setPlaceholderText("Email liên hệ");

    form.addRow("Mã Bác Sĩ:", eMaBS);
    form.addRow("Họ Tên:", eHoTen);
    form.addRow("Khoa:", eKhoa);
    form.addRow("Chuyên Môn:", eChuyenMon);
    form.addRow("Số Điện Thoại:", eSDT);
    form.addRow("Email:", eEmail);
    
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
        if(eMaBS->text().isEmpty() || eHoTen->text().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập đủ thông tin!");
            return;
        }
        QSqlQuery q;
        q.prepare("INSERT INTO Doctors (ma_bac_si, ho_ten, khoa, chuyen_mon, sdt, email) VALUES (?, ?, ?, ?, ?, ?)");
        q.addBindValue(eMaBS->text());
        q.addBindValue(eHoTen->text());
        q.addBindValue(eKhoa->currentText());
        q.addBindValue(eChuyenMon->text());
        q.addBindValue(eSDT->text());
        q.addBindValue(eEmail->text());
        if(q.exec()) { model->select(); dialog.accept(); }
        else { QMessageBox::critical(&dialog, "Lỗi DB", "Không thể thêm!"); }
    });
    
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void DoctorManager::onDeleteDoctor() {
    QModelIndexList sel = tableView->selectionModel()->selectedRows();
    if(sel.isEmpty()) return;

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Xác nhận");
    msgBox.setText("Bạn có chắc chắn muốn xóa bản ghi này hoàn toàn?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.button(QMessageBox::Yes)->setText("Xác Nhận");
    msgBox.button(QMessageBox::No)->setText("Hủy");
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        int row = proxyModel->mapToSource(sel.at(0)).row();
        QString maBS = model->data(model->index(row, 1)).toString();
        
        QSqlQuery delQuery;
        delQuery.prepare("DELETE FROM Doctors WHERE ma_bac_si = ?");
        delQuery.addBindValue(maBS);
        if(delQuery.exec()) {
            model->select();
        }
    }
}
