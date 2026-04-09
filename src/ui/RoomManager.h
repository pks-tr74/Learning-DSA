#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <QWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QLineEdit>

class RoomManager : public QWidget {
    Q_OBJECT
public:
    explicit RoomManager(QWidget *parent = nullptr);

private slots:
    void onSearchTextChanged(const QString &text);
    void onAddRoom();
    void onDeleteRoom();

private:
    void setupUI();
    void loadData();

    QTableView *tableView;
    QSqlTableModel *model;
    QSortFilterProxyModel *proxyModel;
    QLineEdit *searchBox;
};

#endif // ROOMMANAGER_H
