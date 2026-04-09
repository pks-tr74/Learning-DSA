#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QLabel>
#include <QVBoxLayout>

class Dashboard : public QWidget {
    Q_OBJECT
public:
    explicit Dashboard(QWidget *parent = nullptr);
    void updateData(); // Called by MainWindow Timer
    void updateCharts();

protected:
    void showEvent(QShowEvent *event) override;

private:
    void setupUI();
    
    QChart *pieChart;
    QChart *barChart;
    QPieSeries *pieSeries;
    QBarSeries *barSeries;

    QWidget *pieLegendContainer;
    QVBoxLayout *pieLegendLayout;

    QBarCategoryAxis *axisX;
    QValueAxis *axisY;

    QLabel *valAdmitted;
    QLabel *valRooms;
    QLabel *valDoctors;
    QLabel *valDischarged;
};

#endif // DASHBOARD_H
