#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QWidget>

class SettingsManager : public QWidget {
    Q_OBJECT
public:
    explicit SettingsManager(QWidget *parent = nullptr);

signals:
    void themeToggled(bool isDark);

private:
    void setupUI();
    bool isDarkMode;
};

#endif // SETTINGSMANAGER_H
