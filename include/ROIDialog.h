#ifndef ROIDIALOG_H
#define ROIDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "ROIShape.h"
#include "ROIManager.h"
#include <opencv2/opencv.hpp>

class ROIDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit ROIDialog(ROIManager* manager, const cv::Mat& image, QWidget *parent = nullptr);
    
private slots:
    void onRefreshClicked();
    void onExportCSVClicked();
    void onCloseClicked();
    
private:
    void setupUI();
    void updateStatisticsTable();
    QString formatDouble(double value, int precision = 2);
    
    ROIManager* roiManager;
    cv::Mat currentImage;
    
    QTableWidget* statsTable;
    QPushButton* refreshButton;
    QPushButton* exportButton;
    QPushButton* closeButton;
};

#endif // ROIDIALOG_H
