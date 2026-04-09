#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>

class Dashboard;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void switchPage(int index);
    void updateSidebarCounts();
    void onThemeToggled(bool isDark);

private:
    void setupUI();
    void loadStyle(bool isDark = false);
    
    QListWidget *sidebar;
    QStackedWidget *stackedWidget;
    Dashboard *dashboard;
};

#endif // MAINWINDOW_H
