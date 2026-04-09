#ifndef DOCTORMANAGER_H
#define DOCTORMANAGER_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QLineEdit>

class DoctorManager : public QWidget {
    Q_OBJECT
public:
    explicit DoctorManager(QWidget *parent = nullptr);

private slots:
    void onSearchTextChanged(const QString &text);
    void onAddDoctor();
    void onDeleteDoctor();

private:
    void setupUI();
    void loadData();

    QTableView *tableView;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
    QLineEdit *searchBox;
};

#endif // DOCTORMANAGER_H
