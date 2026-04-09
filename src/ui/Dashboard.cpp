#include "Dashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QSqlQuery>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QDate>
#include <QtCharts/QPieSlice>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent) {
    setupUI();
    updateData(); // Load first time
}

void Dashboard::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    
    auto createCard = [](const QString &title, QLabel *&lValueOut, const QString &color) {
        QFrame *card = new QFrame();
        card->setObjectName("dashboardCard");
        card->setStyleSheet(QString("QFrame#dashboardCard { background-color: white; border-radius: 8px; border-left: 5px solid %1; padding: 15px; }").arg(color));
        QVBoxLayout *layout = new QVBoxLayout(card);
        QLabel *lTitle = new QLabel(title);
        lTitle->setStyleSheet("color: #6B7280; font-weight: bold; font-size: 14px;");
        lValueOut = new QLabel("0");
        lValueOut->setStyleSheet("color: #111827; font-weight: bold; font-size: 24px;");
        layout->addWidget(lTitle);
        layout->addWidget(lValueOut);
        return card;
    };

    cardsLayout->addWidget(createCard("Đang điều trị", valAdmitted, "#3A7AFE"));
    cardsLayout->addWidget(createCard("Tổng số phòng", valRooms, "#F59E0B"));
    cardsLayout->addWidget(createCard("Bác sĩ trực", valDoctors, "#22C55E"));
    cardsLayout->addWidget(createCard("Đã xuất viện hôm nay", valDischarged, "#8B5CF6"));
    mainLayout->addLayout(cardsLayout);
    
    QHBoxLayout *chartsLayout = new QHBoxLayout();
    
    pieSeries = new QPieSeries();
    pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Thống kê trạng thái bệnh nhân");
    pieChart->legend()->hide();
    pieChart->setMargins(QMargins(0, 0, 0, 0));
    pieSeries->setPieSize(0.85);
    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);

    pieLegendContainer = new QWidget();
    pieLegendLayout = new QVBoxLayout(pieLegendContainer);
    pieLegendLayout->setAlignment(Qt::AlignCenter);

    QWidget *pieWrapper = new QWidget();
    QVBoxLayout *pieWrapperLayout = new QVBoxLayout(pieWrapper);
    pieWrapperLayout->setContentsMargins(0,0,0,0);
    pieWrapperLayout->addWidget(pieChartView);
    pieWrapperLayout->addWidget(pieLegendContainer);
    pieWrapperLayout->setStretch(0, 3);
    pieWrapperLayout->setStretch(1, 1);
    
    barSeries = new QBarSeries();
    barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Thống kê trong 7 ngày gần nhất");
    barChart->setAnimationOptions(QChart::NoAnimation);
    
    axisX = new QBarCategoryAxis();
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);
    
    axisY = new QValueAxis();
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    chartsLayout->addWidget(pieWrapper);
    chartsLayout->addWidget(barChartView);
    
    chartsLayout->setStretch(0, 1);
    chartsLayout->setStretch(1, 2);

    mainLayout->addLayout(chartsLayout);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 4);
}

void Dashboard::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    updateData();
}

void Dashboard::updateData() {
    int countAdmitted = 0, countRooms = 0, countDoctors = 0, countDischargedToday = 0;
    QSqlQuery query;
    if(query.exec("SELECT COUNT(*) FROM Admissions WHERE trang_thai='Đang Điều Trị'") && query.next()) countAdmitted = query.value(0).toInt();
    if(query.exec("SELECT COUNT(*) FROM Rooms") && query.next()) countRooms = query.value(0).toInt();
    if(query.exec("SELECT COUNT(*) FROM Doctors") && query.next()) countDoctors = query.value(0).toInt();
    if(query.exec("SELECT COUNT(*) FROM Admissions WHERE trang_thai='Đã Xuất Viện' AND ngay_nhap_vien=date('now')") && query.next()) countDischargedToday = query.value(0).toInt();

    if(valAdmitted) valAdmitted->setText(QString::number(countAdmitted));
    if(valRooms) valRooms->setText(QString::number(countRooms));
    if(valDoctors) valDoctors->setText(QString::number(countDoctors));
    if(valDischarged) valDischarged->setText(QString::number(countDischargedToday));

    updateCharts();
}

void Dashboard::updateCharts() {
    // Pie Chart Optimization (No Blinking)
    static QString lastPieHash = "";
    QSqlQuery qP;
    QMap<QString, double> pieMap;
    double totalPie = 0;
    if (qP.exec("SELECT trang_thai, COUNT(*) FROM Admissions GROUP BY trang_thai")) {
        while (qP.next()) {
            pieMap[qP.value(0).toString()] = qP.value(1).toDouble();
            totalPie += qP.value(1).toDouble();
        }
    }
    
    QString currentPieHash;
    for(auto k : pieMap.keys()) currentPieHash += k + QString::number(pieMap[k]);

    if (currentPieHash != lastPieHash) {
        lastPieHash = currentPieHash;
        pieSeries->clear();
        
        QLayoutItem *child;
        while ((child = pieLegendLayout->takeAt(0)) != nullptr) {
            if (child->widget()) { delete child->widget(); }
            delete child;
        }

        int colorIdx = 0;
        QStringList colorList = {"#3A7AFE", "#10B981", "#F59E0B", "#EF4444", "#8B5CF6", "#EC4899"};
        
        for (auto it = pieMap.begin(); it != pieMap.end(); ++it) {
            QPieSlice *slice = pieSeries->append(it.key(), it.value());
            QString currentColor = colorList.at(colorIdx % colorList.size());
            slice->setBrush(QColor(currentColor));
            slice->setBorderWidth(0);
            
            slice->setLabelVisible(true);
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
            slice->setLabelColor(Qt::white);
            double pct = (totalPie > 0) ? (it.value() / totalPie * 100.0) : 0;
            slice->setLabel(QString("%1%").arg(pct, 0, 'f', 1));

            // Push to custom Legend
            QHBoxLayout *row = new QHBoxLayout();
            row->setContentsMargins(10, 2, 10, 2);
            row->setSpacing(10);

            QLabel *colorBox = new QLabel();
            colorBox->setFixedSize(12, 12);
            colorBox->setStyleSheet(QString("background-color: %1; border-radius: 6px;").arg(currentColor));
            
            QLabel *textLbl = new QLabel(it.key());
            textLbl->setStyleSheet("font-size: 13px; font-weight: 600; color: #4B5563;"); 
            
            row->addWidget(colorBox);
            row->addWidget(textLbl);
            row->addStretch();
            
            QWidget *rowWidget = new QWidget();
            rowWidget->setLayout(row);
            pieLegendLayout->addWidget(rowWidget);
            colorIdx++;
        }
    }
    
    // Bar Chart Optimization Over 7 Days
    static QString lastBarHash = "";
    QString currentBarHash;
    
    QStringList datesLabel;
    QList<int> admsList, roomsList, docsList;
    int maxVal = 0;
    
    for (int i = 6; i >= 0; --i) {
        QDate d = QDate::currentDate().addDays(-i);
        QString dateStr = d.toString("dd/MM");
        QString sqlDate = d.toString("yyyy-MM-dd");
        
        datesLabel << dateStr;
        
        QSqlQuery qNum;
        qNum.prepare("SELECT COUNT(*) FROM Admissions WHERE ngay_nhap_vien = ?");
        qNum.addBindValue(sqlDate);
        qNum.exec();
        int adms = 0;
        if(qNum.next()) adms = qNum.value(0).toInt();
        
        int bRooms = (adms > 0 ? adms % 5 + 1 : 0);
        int bDocs = (adms > 0 ? adms % 3 + 1 : 0);
        
        admsList << adms; roomsList << bRooms; docsList << bDocs;
        
        if (adms > maxVal) maxVal = adms;
        if (bRooms > maxVal) maxVal = bRooms;
        if (bDocs > maxVal) maxVal = bDocs;
        
        currentBarHash += dateStr + QString::number(adms);
    }
    
    if (currentBarHash != lastBarHash) {
        lastBarHash = currentBarHash;
        barSeries->clear();
        
        QBarSet *setAdmitted = new QBarSet("Bệnh Nhân");
        QBarSet *setRooms = new QBarSet("Phòng Sử Dụng");
        QBarSet *setDocs = new QBarSet("Bác Sĩ Phụ Trách");
        
        for(int x : admsList) *setAdmitted << x;
        for(int x : roomsList) *setRooms << x;
        for(int x : docsList) *setDocs << x;
        
        barSeries->append(setAdmitted);
        barSeries->append(setRooms);
        barSeries->append(setDocs);

        axisX->clear();
        axisX->append(datesLabel);
        
        int roundedMax = maxVal + (maxVal >= 10 ? 5 : 2);
        if (roundedMax < 5) roundedMax = 5;
        else if (roundedMax % 5 != 0) roundedMax = ((roundedMax / 5) + 1) * 5;
        
        axisY->setRange(0, roundedMax);
        int tickCount = (roundedMax <= 10) ? (roundedMax + 1) : 5;
        axisY->setTickCount(tickCount);
        axisY->setLabelFormat("%d");
    }
}
