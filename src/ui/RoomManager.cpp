#include "RoomManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSpinBox>
#include <QComboBox>

RoomManager::RoomManager(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadData();
}

void RoomManager::setupUI() {
    // ...
    // To safe guard size constraints, same layout structure
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *topLayout = new QHBoxLayout();
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Tìm kiếm phòng...");
    QPushButton *btnAdd = new QPushButton("Thêm Phòng", this);
    btnAdd->setObjectName("btnPrimary");
    QPushButton *btnDelete = new QPushButton("Xóa Phòng", this);
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

    connect(searchBox, &QLineEdit::textChanged, this, &RoomManager::onSearchTextChanged);
    connect(btnAdd, &QPushButton::clicked, this, &RoomManager::onAddRoom);
    connect(btnDelete, &QPushButton::clicked, this, &RoomManager::onDeleteRoom);
}

void RoomManager::loadData() {
    model = new QSqlTableModel(this);
    model->setTable("Rooms");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    
    // id, ma_phong, ten_phong, so_giuong, bac_si_quan_ly
    model->setHeaderData(1, Qt::Horizontal, "Mã Phòng");
    model->setHeaderData(2, Qt::Horizontal, "Tên Phòng");
    model->setHeaderData(3, Qt::Horizontal, "Số Giường (Tối Đa)");
    model->setHeaderData(4, Qt::Horizontal, "Bác Sĩ Quản Lý");

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1); 

    tableView->setModel(proxyModel);
    tableView->hideColumn(0); 

    QHeaderView *header = tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::Stretch); // Cột Bác Sĩ dãn dài nhất
}

void RoomManager::onSearchTextChanged(const QString &text) {
    proxyModel->setFilterFixedString(text);
}

void RoomManager::onAddRoom() {
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm Phòng");
    QFormLayout form(&dialog);
    form.setSpacing(15);
    form.setSizeConstraint(QLayout::SetFixedSize);
    
    QLineEdit *eMaPhong = new QLineEdit(&dialog);
    eMaPhong->setPlaceholderText("VD: P102");
    QLineEdit *eTenPhong = new QLineEdit(&dialog);
    eTenPhong->setPlaceholderText("VD: Khoa Ngoại");
    QSpinBox *eSoGiuong = new QSpinBox(&dialog);
    eSoGiuong->setRange(1, 100);
    
    QComboBox *eBacSi = new QComboBox(&dialog);
    QSqlQuery qBS("SELECT ma_bac_si, ho_ten FROM Doctors");
    bool hasDoctors = false;
    while(qBS.next()) {
        hasDoctors = true;
        eBacSi->addItem(qBS.value(1).toString(), qBS.value(0).toString());
    }
    if (!hasDoctors) eBacSi->addItem("Chưa có bác sĩ", "");

    form.addRow("Mã Phòng:", eMaPhong);
    form.addRow("Tên Phòng:", eTenPhong);
    form.addRow("Số Giường:", eSoGiuong);
    form.addRow("Bác Sĩ QL:", eBacSi);
    
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
        if(eMaPhong->text().isEmpty() || eTenPhong->text().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng điền đủ!");
            return;
        }
        QSqlQuery q;
        q.prepare("INSERT INTO Rooms (ma_phong, ten_phong, so_giuong, bac_si_quan_ly) VALUES (?, ?, ?, ?)");
        q.addBindValue(eMaPhong->text());
        q.addBindValue(eTenPhong->text());
        q.addBindValue(eSoGiuong->value());
        q.addBindValue(eBacSi->currentText());
        if(q.exec()) { model->select(); dialog.accept(); }
        else { QMessageBox::critical(&dialog, "Lỗi DB", "Không thể thêm phòng!"); }
    });
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}

void RoomManager::onDeleteRoom() {
    QModelIndexList sel = tableView->selectionModel()->selectedRows();
    if(sel.isEmpty()) return;

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Xác nhận");
    msgBox.setText("Bạn có chắc chắn muốn xóa phòng vĩnh viễn khỏi hệ thống?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.button(QMessageBox::Yes)->setText("Xác Nhận");
    msgBox.button(QMessageBox::No)->setText("Hủy");
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        int row = proxyModel->mapToSource(sel.at(0)).row();
        QString maPhong = model->data(model->index(row, 1)).toString();
        
        QSqlQuery delQuery;
        delQuery.prepare("DELETE FROM Rooms WHERE ma_phong = ?");
        delQuery.addBindValue(maPhong);
        if(delQuery.exec()) model->select();
        
    }
}
