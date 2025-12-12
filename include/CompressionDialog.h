#ifndef COMPRESSIONDIALOG_H
#define COMPRESSIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <opencv2/opencv.hpp>

class CompressionDialog : public QDialog {
    Q_OBJECT

public:
    explicit CompressionDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~CompressionDialog();

    // Getters
    cv::Mat getCompressedImage() const { return compressedImage; }
    QString getCompressionType() const { return compressionType; }
    int getQuality() const { return quality; }
    int getPngLevel() const { return pngLevel; }
    double getCompressionRatio() const { return compressionRatio; }
    double getRMSE() const { return rmse; }
    double getPSNR() const { return psnr; }
    bool wasApplied() const { return applied; }

signals:
    void compressionUpdated(const cv::Mat& compressed);

private slots:
    void onCompressionTypeChanged(int index);
    void onJpegQualityChanged(int value);
    void onPngLevelChanged(int value);
    void onApplyClicked();
    void onCancelClicked();

private:
    void setupUI();
    void updateCompression();
    void updateMetrics();
    void updatePreview();
    
    // Compression methods
    cv::Mat compressJPEG(int quality);
    cv::Mat compressPNG(int level);
    
    // Metrics calculation
    double calculateRMSE(const cv::Mat& img1, const cv::Mat& img2);
    double calculatePSNR(double rmse);
    double calculateCompressionRatio(const cv::Mat& original, const cv::Mat& compressed);

    // UI Components
    QComboBox *typeComboBox;
    QSlider *jpegQualitySlider;
    QSlider *pngLevelSlider;
    QLabel *jpegQualityLabel;
    QLabel *pngLevelLabel;
    QLabel *compressionRatioLabel;
    QLabel *rmseLabel;
    QLabel *psnrLabel;
    QLabel *originalSizeLabel;
    QLabel *compressedSizeLabel;
    QLabel *qualityAssessmentLabel;
    QGroupBox *jpegGroup;
    QGroupBox *pngGroup;
    QPushButton *applyButton;
    QPushButton *cancelButton;

    // Data
    cv::Mat originalImage;
    cv::Mat compressedImage;
    QString compressionType;
    int quality;
    int pngLevel;
    double compressionRatio;
    double rmse;
    double psnr;
    bool applied;
    
    size_t originalSize;
    size_t compressedSize;
};

#endif // COMPRESSIONDIALOG_H
