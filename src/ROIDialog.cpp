#include "ROIDialog.h"
#include "Theme.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

ROIDialog::ROIDialog(ROIManager* manager, const cv::Mat& image, QWidget *parent)
    : QDialog(parent), roiManager(manager), currentImage(image) {
    
    setWindowTitle("ROI Statistics");
    setMinimumSize(900, 500);
    
    setupUI();
    updateStatisticsTable();
}

void ROIDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel* titleLabel = new QLabel("Region of Interest Statistics");
    titleLabel->setObjectName("dialogTitle");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 15px;");
    mainLayout->addWidget(titleLabel);
    
    // Info label
    QLabel* infoLabel = new QLabel(QString("Total ROIs: %1").arg(roiManager->count()));
    infoLabel->setStyleSheet("color: #c4b5fd; padding: 5px 15px; font-size: 10pt;");
    mainLayout->addWidget(infoLabel);
    
    // Statistics table
    statsTable = new QTableWidget(this);
    statsTable->setStyleSheet(
        "QTableWidget {"
        "    background-color: rgba(45, 37, 71, 0.5);"
        "    color: #f3e8ff;"
        "    border: 2px solid rgba(91, 75, 115, 0.5);"
        "    border-radius: 10px;"
        "    gridline-color: rgba(232, 121, 249, 0.2);"
        "}"
        "QTableWidget::item {"
        "    padding: 8px;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: rgba(232, 121, 249, 0.3);"
        "}"
        "QHeaderView::section {"
        "    background-color: rgba(91, 75, 115, 0.6);"
        "    color: #e879f9;"
        "    padding: 10px;"
        "    border: none;"
        "    font-weight: bold;"
        "}"
    );
    
    mainLayout->addWidget(statsTable);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    refreshButton = new QPushButton("Refresh", this);
    refreshButton->setProperty("class", "accent");
    refreshButton->setMinimumWidth(100);
    connect(refreshButton, &QPushButton::clicked, this, &ROIDialog::onRefreshClicked);
    buttonLayout->addWidget(refreshButton);
    
    exportButton = new QPushButton("Export CSV", this);
    exportButton->setMinimumWidth(100);
    connect(exportButton, &QPushButton::clicked, this, &ROIDialog::onExportCSVClicked);
    buttonLayout->addWidget(exportButton);
    
    closeButton = new QPushButton("Close", this);
    closeButton->setMinimumWidth(100);
    connect(closeButton, &QPushButton::clicked, this, &ROIDialog::onCloseClicked);
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
}

void ROIDialog::updateStatisticsTable() {
    int roiCount = roiManager->count();
    
    if (roiCount == 0) {
        statsTable->setRowCount(0);
        statsTable->setColumnCount(0);
        return;
    }
    
    // Determine if we have color images
    bool hasColor = currentImage.channels() == 3;
    
    // Set up columns
    QStringList headers;
    headers << "ROI Name" << "Area" << "Mean" << "Std Dev" << "Min" << "Max" << "Median";
    
    if (hasColor) {
        headers << "R Mean" << "G Mean" << "B Mean";
    }
    
    statsTable->setColumnCount(headers.size());
    statsTable->setHorizontalHeaderLabels(headers);
    statsTable->setRowCount(roiCount);
    
    // Fill table with statistics
    for (int i = 0; i < roiCount; i++) {
        ROIShape* roi = roiManager->getROI(i);
        if (!roi) continue;
        
        ROIStats stats = roi->calculateStats(currentImage);
        
        int col = 0;
        
        // ROI Name
        QTableWidgetItem* nameItem = new QTableWidgetItem(roi->getName());
        nameItem->setForeground(QBrush(roi->getColor()));
        nameItem->setFont(QFont("Arial", 10, QFont::Bold));
        statsTable->setItem(i, col++, nameItem);
        
        // Area
        statsTable->setItem(i, col++, new QTableWidgetItem(QString::number(stats.area)));
        
        // Mean
        statsTable->setItem(i, col++, new QTableWidgetItem(formatDouble(stats.mean)));
        
        // Std Dev
        statsTable->setItem(i, col++, new QTableWidgetItem(formatDouble(stats.stdDev)));
        
        // Min
        statsTable->setItem(i, col++, new QTableWidgetItem(formatDouble(stats.min)));
        
        // Max
        statsTable->setItem(i, col++, new QTableWidgetItem(formatDouble(stats.max)));
        
        // Median
        statsTable->setItem(i, col++, new QTableWidgetItem(formatDouble(stats.median)));
        
        if (hasColor && stats.hasColorStats) {
            // Red Mean
            statsTable->setItem(i, col++, new QTableWidgetItem(formatDouble(stats.red.mean)));
            
            // Green Mean
            statsTable->setItem(i, col++, new QTableWidgetItem(formatDouble(stats.green.mean)));
            
            // Blue Mean
            statsTable->setItem(i, col++, new QTableWidgetItem(formatDouble(stats.blue.mean)));
        }
    }
    
    // Resize columns to content
    statsTable->resizeColumnsToContents();
    statsTable->horizontalHeader()->setStretchLastSection(true);
}

QString ROIDialog::formatDouble(double value, int precision) {
    return QString::number(value, 'f', precision);
}

void ROIDialog::onRefreshClicked() {
    updateStatisticsTable();
}

void ROIDialog::onExportCSVClicked() {
    QString filename = QFileDialog::getSaveFileName(this,
        "Export ROI Statistics",
        "",
        "CSV Files (*.csv);;All Files (*)");
    
    if (filename.isEmpty()) return;
    
    if (roiManager->exportStatisticsToCSV(filename, currentImage)) {
        QMessageBox::information(this, "Export Successful",
            "ROI statistics exported to CSV successfully!");
    } else {
        QMessageBox::critical(this, "Export Failed",
            "Failed to export ROI statistics to CSV!");
    }
}

void ROIDialog::onCloseClicked() {
    accept();
}
#include "moc_ROIDialog.cpp"
