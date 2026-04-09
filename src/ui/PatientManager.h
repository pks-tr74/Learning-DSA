#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QLineEdit>

class PatientManager : public QWidget {
    Q_OBJECT
public:
    explicit PatientManager(QWidget *parent = nullptr);

private slots:
    void onSearchTextChanged(const QString &text);
    void onAddPatient();
    void onDeletePatient();

private:
    void setupUI();
    void loadData();

    QTableView *tableView;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
    QLineEdit *searchBox;
};

#endif // PATIENTMANAGER_H
