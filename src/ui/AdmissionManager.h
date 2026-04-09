#ifndef ADMISSIONMANAGER_H
#define ADMISSIONMANAGER_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QLineEdit>

class AdmissionManager : public QWidget {
    Q_OBJECT
public:
    explicit AdmissionManager(QWidget *parent = nullptr);

private slots:
    void onSearchTextChanged(const QString &text);
    void onAddAdmission();
    void onUpdateStatus();

private:
    void setupUI();
    void loadData();

    QTableView *tableView;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
    QLineEdit *searchBox;
};

#endif // ADMISSIONMANAGER_H
