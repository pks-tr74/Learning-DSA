#include "MainWindow.h"
#include "Dashboard.h"
#include "PatientManager.h"
#include "RoomManager.h"
#include "DoctorManager.h"
#include "AdmissionManager.h"
#include "SettingsManager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QSqlQuery>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Phần Mềm Quản Lý Hồ Sơ Bệnh Nhân");
    resize(1280, 720);
    setupUI();
    loadStyle(false);
    updateSidebarCounts();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSidebarCounts);
    timer->start(3000); // 3 seconds Realtime Updates
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *sidebarContainer = new QWidget(this);
    sidebarContainer->setObjectName("sidebarContainer");
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarContainer);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *logoLabel = new QLabel("HOSPITAL\nSYSTEM", this);
    logoLabel->setObjectName("logoLabel");
    logoLabel->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(logoLabel);

    sidebar = new QListWidget(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFocusPolicy(Qt::NoFocus);
    
    sidebar->addItem("Tổng quan");
    sidebar->addItem("Quản lý bệnh nhân");
    sidebar->addItem("Quản lý bác sĩ");
    sidebar->addItem("Quản lý phòng");
    sidebar->addItem("Nhập viện");
    sidebar->addItem("Cài đặt");
    
    sidebar->setCurrentRow(0);
    sidebarLayout->addWidget(sidebar);
    mainLayout->addWidget(sidebarContainer);

    QVBoxLayout *contentLayout = new QVBoxLayout();
    QWidget *header = new QWidget(this);
    header->setObjectName("header");
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    QLabel *titleLabel = new QLabel("Trang chủ", this);
    titleLabel->setObjectName("headerTitle");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    contentLayout->addWidget(header);

    stackedWidget = new QStackedWidget(this);
    dashboard = new Dashboard(this); // Stored in class to allow realtime updates manually
    PatientManager *patientManager = new PatientManager(this);
    DoctorManager *doctorManager = new DoctorManager(this);
    RoomManager *roomManager = new RoomManager(this);
    AdmissionManager *admissionManager = new AdmissionManager(this);
    SettingsManager *settingsManager = new SettingsManager(this);
    connect(settingsManager, &SettingsManager::themeToggled, this, &MainWindow::onThemeToggled);
    
    stackedWidget->addWidget(dashboard);
    stackedWidget->addWidget(patientManager);
    stackedWidget->addWidget(doctorManager);
    stackedWidget->addWidget(roomManager);
    stackedWidget->addWidget(admissionManager);
    stackedWidget->addWidget(settingsManager);
    
    contentLayout->addWidget(stackedWidget);
    mainLayout->addLayout(contentLayout);
    mainLayout->setStretch(0, 2);
    mainLayout->setStretch(1, 10);

    connect(sidebar, &QListWidget::currentRowChanged, this, &MainWindow::switchPage);
    connect(sidebar, &QListWidget::currentTextChanged, titleLabel, &QLabel::setText);
}

void MainWindow::switchPage(int index) {
    if (index < stackedWidget->count()) stackedWidget->setCurrentIndex(index);
    updateSidebarCounts();
}

void MainWindow::updateSidebarCounts() {
    QSqlQuery query;
    int patientCount = 0, doctorCount = 0, roomCount = 0, admissionCount = 0;
    if(query.exec("SELECT COUNT(*) FROM Patients") && query.next()) patientCount = query.value(0).toInt();
    if(query.exec("SELECT COUNT(*) FROM Doctors") && query.next()) doctorCount = query.value(0).toInt();
    if(query.exec("SELECT COUNT(*) FROM Rooms") && query.next()) roomCount = query.value(0).toInt();
    if(query.exec("SELECT COUNT(*) FROM Admissions WHERE trang_thai='Đang Điều Trị'") && query.next()) admissionCount = query.value(0).toInt();

    sidebar->item(0)->setText("Tổng quan");
    sidebar->item(1)->setText(QString("Quản lý bệnh nhân (%1)").arg(patientCount));
    sidebar->item(2)->setText(QString("Quản lý bác sĩ (%1)").arg(doctorCount));
    sidebar->item(3)->setText(QString("Quản lý phòng (%1)").arg(roomCount));
    sidebar->item(4)->setText(QString("Nhập viện (%1)").arg(admissionCount));
    sidebar->item(5)->setText("Cài đặt");

    // Force Dashboard Charts update dynamically
    if (dashboard) dashboard->updateData();
}

void MainWindow::onThemeToggled(bool isDark) {
    loadStyle(isDark);
}

void MainWindow::loadStyle(bool isDark) {
    QString path = isDark ? "styles/dark_style.qss" : "styles/style.qss";
    QFile file(path);
    if(file.open(QFile::ReadOnly)) {
        setStyleSheet(QString::fromLatin1(file.readAll()));
    }
}
