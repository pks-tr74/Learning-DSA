#include "DatabaseManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) m_db.close();
}

QSqlDatabase DatabaseManager::getDatabase() const {
    return m_db;
}

bool DatabaseManager::initializeDatabase() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("hospital_management.db");

    if (!m_db.open()) return false;
    createTables();
    return true;
}

void DatabaseManager::createTables() {
    QSqlQuery query;
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS Patients (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            ma_benh_nhan TEXT UNIQUE NOT NULL,
            ho_ten TEXT NOT NULL,
            ngay_sinh DATE,
            gioi_tinh TEXT,
            cccd TEXT UNIQUE,
            dia_chi TEXT,
            sdt TEXT,
            bhyt TEXT
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS Doctors (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            ma_bac_si TEXT UNIQUE NOT NULL,
            ho_ten TEXT NOT NULL,
            khoa TEXT,
            chuyen_mon TEXT,
            sdt TEXT,
            email TEXT
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS Rooms (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            ma_phong TEXT UNIQUE NOT NULL,
            ten_phong TEXT NOT NULL,
            so_giuong INTEGER NOT NULL,
            bac_si_quan_ly TEXT
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS Admissions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            ma_benh_nhan TEXT NOT NULL,
            ma_phong TEXT NOT NULL,
            benh_ly TEXT,
            ngay_nhap_vien DATE NOT NULL,
            trang_thai TEXT NOT NULL
        )
    )");

    // Fix lỗi thiếu cột do IF NOT EXISTS không update bảng cũ:
    query.exec("ALTER TABLE Admissions ADD COLUMN benh_ly TEXT");

    // Dummy Data
    query.exec("SELECT COUNT(*) FROM Patients");
    if(query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO Patients (ma_benh_nhan, ho_ten, ngay_sinh, gioi_tinh, cccd, dia_chi, sdt, bhyt) VALUES ('BN001', 'Nguyen Van A', '1990-01-01', 'Nam', '012345678912', 'Ha Noi', '0912345678', 'DN12345678')");
        query.exec("INSERT INTO Doctors (ma_bac_si, ho_ten, khoa, chuyen_mon, sdt, email) VALUES ('BS001', 'Tran Van B', 'Nội Khoa', 'Tim Mạch', '0900000000', 'drb@hospital.com')");
        query.exec("INSERT INTO Rooms (ma_phong, ten_phong, so_giuong, bac_si_quan_ly) VALUES ('PH01', 'Phòng Hồi Sức', 5, 'Tran Van B')");
        query.exec("INSERT INTO Admissions (ma_benh_nhan, ma_phong, benh_ly, ngay_nhap_vien, trang_thai) VALUES ('BN001', 'PH01', 'Viêm phổi', '2026-04-09', 'Đang Điều Trị')");
    }
}
