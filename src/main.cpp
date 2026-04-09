#include <QApplication>
#include <QMessageBox>
#include "database/DatabaseManager.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Fix issue QFont::setPointSize: Point size <= 0 (-1)
    // We remove default a.setFont and rely entirely on QSS which sets via pixel size font-size: Xpx;
    
    if (!DatabaseManager::instance().initializeDatabase()) {
        QMessageBox::critical(nullptr, "Lỗi Nghiêm Trọng", "Không thể khởi tạo Cơ sở dữ liệu SQLite.\nVui lòng kiểm tra quyền truy cập cục bộ.");
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
