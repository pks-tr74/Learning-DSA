#include "SettingsManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

SettingsManager::SettingsManager(QWidget *parent) : QWidget(parent), isDarkMode(false) {
    setupUI();
}

void SettingsManager::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);

    QLabel *lblTitle = new QLabel("CÀI ĐẶT HỆ THỐNG", this);
    lblTitle->setStyleSheet("font-size: 28px; font-weight: bold; color: #3A7AFE;");
    lblTitle->setAlignment(Qt::AlignCenter);

    QHBoxLayout *splitLayout = new QHBoxLayout();
    splitLayout->setSpacing(20);

    QFrame *infoContainer = new QFrame(this);
    infoContainer->setStyleSheet("background-color: white; border-radius: 10px; padding: 20px; border: 1px solid #E5E7EB;");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoContainer);
    
    QLabel *lblAbout = new QLabel("<h3>Phần mềm Quản Lý Hồ Sơ Bệnh Nhân (HMS)</h3>"
                                  "<p>Hệ thống hỗ trợ lưu trữ nội trú tối ưu cho Bệnh viện và Cơ sở Y tế Cấp Xã/Phường. "
                                  "Giúp số hóa toàn bộ quá trình Nhập Viện, Cấp Phát Thuốc, Quản Lý Dữ Liệu nhanh chóng nhất.</p>", this);
    lblAbout->setWordWrap(true);
    lblAbout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    lblAbout->setStyleSheet("font-size: 15px; color: #111827;");
    infoLayout->addWidget(lblAbout);

    QFrame *teamContainer = new QFrame(this);
    teamContainer->setStyleSheet("background-color: white; border-radius: 10px; padding: 20px; border: 1px solid #E5E7EB;");
    QVBoxLayout *teamLayout = new QVBoxLayout(teamContainer);
    
    QLabel *lblDev = new QLabel("<b>Thành viên góp mặt trong dự án:</b><br/><br/>- Lê Huy Hoàng<br/>- Trần Xuân Bách<br/>- Nguyễn Xuân Hoàn<br/>- Trần Anh Tú", this);
    lblDev->setWordWrap(true);
    lblDev->setStyleSheet("font-size: 15px; color: #111827; line-height: 1.5;");
    lblDev->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    teamLayout->addWidget(lblDev);

    splitLayout->addWidget(infoContainer);
    splitLayout->addWidget(teamContainer);
    splitLayout->setStretch(0, 3);
    splitLayout->setStretch(1, 2);

    QPushButton *btnTheme = new QPushButton("Bật / Tắt Giao Diện Nền Tối", this);
    btnTheme->setFixedSize(300, 50);
    btnTheme->setObjectName("btnPrimary");

    mainLayout->addWidget(lblTitle);
    mainLayout->addLayout(splitLayout);
    mainLayout->addWidget(btnTheme, 0, Qt::AlignCenter);

    connect(btnTheme, &QPushButton::clicked, [this]() {
        isDarkMode = !isDarkMode;
        emit themeToggled(isDarkMode);
    });
}
