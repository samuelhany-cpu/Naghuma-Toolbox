#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application metadata
    QApplication::setApplicationName("Naghuma Toolbox");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("Naghuma");
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}