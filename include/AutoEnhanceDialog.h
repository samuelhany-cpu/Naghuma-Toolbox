#ifndef AUTOENHANCEDIALOG_H
#define AUTOENHANCEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <opencv2/opencv.hpp>

class ImageCanvas;

class AutoEnhanceDialog : public QDialog {
    Q_OBJECT

public:
    explicit AutoEnhanceDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~AutoEnhanceDialog();

    // Getters
    cv::Mat getEnhancedImage() const { return enhancedImage; }
    QString getAlgorithmType() const { return algorithmType; }
    double getRMSE() const { return rmse; }
    double getPSNR() const { return psnr; }
    bool wasApplied() const { return applied; }

signals:
    void enhancementUpdated(const cv::Mat& enhanced);

private slots:
    void onAlgorithmChanged();
    void onApplyClicked();
    void onCancelClicked();

private:
    void setupUI();
    void updateEnhancement();
    void updateMetrics();
    void updateComparison();
    
    // Enhancement algorithms
    cv::Mat applyAdaptiveHistogramEqualization();
    cv::Mat applyContrastStretching();
    
    // Metrics calculation
    double calculateRMSE(const cv::Mat& img1, const cv::Mat& img2);
    double calculatePSNR(double rmse);

    // UI Components
    QButtonGroup *algorithmGroup;
    QRadioButton *adaptiveHistRadio;
    QRadioButton *contrastStretchRadio;
    
    ImageCanvas *originalCanvas;
    ImageCanvas *enhancedCanvas;
    
    QLabel *rmseLabel;
    QLabel *psnrLabel;
    QLabel *qualityAssessmentLabel;
    QLabel *algorithmDescLabel;
    
    QPushButton *applyButton;
    QPushButton *cancelButton;

    // Data
    cv::Mat originalImage;
    cv::Mat enhancedImage;
    QString algorithmType;
    double rmse;
    double psnr;
    bool applied;
};

#endif // AUTOENHANCEDIALOG_H
