#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Suppress Qt stylesheet warnings for CSS3 properties
    qputenv("QT_LOGGING_RULES", "qt.qpa.*.warning=false");
    
    // Set application metadata
    QApplication::setApplicationName("Naghumaaz Toolbox");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("Naghumaaz");
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}