#ifndef NOISEREMOVALDIALOG_H
#define NOISEREMOVALDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QSlider>
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <opencv2/opencv.hpp>

class ImageCanvas;

class NoiseRemovalDialog : public QDialog {
    Q_OBJECT

public:
    explicit NoiseRemovalDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~NoiseRemovalDialog();

    // Getters
    cv::Mat getDenoisedImage() const { return denoisedImage; }
    QString getFilterType() const { return filterType; }
    int getKernelSize() const { return kernelSize; }
    double getSigmaColor() const { return sigmaColor; }
    double getSigmaSpace() const { return sigmaSpace; }
    double getSNRImprovement() const { return snrImprovement; }
    double getOriginalSNR() const { return originalSNR; }
    double getFilteredSNR() const { return filteredSNR; }
    bool wasApplied() const { return applied; }

signals:
    void filterUpdated(const cv::Mat& denoised);

private slots:
    void onFilterTypeChanged();
    void onKernelSizeChanged(int value);
    void onSigmaColorChanged(int value);
    void onSigmaSpaceChanged(int value);
    void onApplyClicked();
    void onCancelClicked();

private:
    void setupUI();
    void updateFilter();
    void updateMetrics();
    void updateComparison();
    
    // Noise removal filters
    cv::Mat applyGaussianFilter();
    cv::Mat applyMedianFilter();
    cv::Mat applyBilateralFilter();
    
    // Metrics calculation
    double calculateSNR(const cv::Mat& img);
    double calculatePSNR(const cv::Mat& img1, const cv::Mat& img2);

    // UI Components
    QButtonGroup *filterGroup;
    QRadioButton *gaussianRadio;
    QRadioButton *medianRadio;
    QRadioButton *bilateralRadio;
    
    QGroupBox *gaussianGroup;
    QGroupBox *medianGroup;
    QGroupBox *bilateralGroup;
    
    QSlider *gaussianKernelSlider;
    QSlider *medianKernelSlider;
    QSlider *bilateralKernelSlider;
    QSlider *sigmaColorSlider;
    QSlider *sigmaSpaceSlider;
    
    QLabel *gaussianKernelLabel;
    QLabel *medianKernelLabel;
    QLabel *bilateralKernelLabel;
    QLabel *sigmaColorLabel;
    QLabel *sigmaSpaceLabel;
    
    ImageCanvas *originalCanvas;
    ImageCanvas *denoisedCanvas;
    
    QLabel *originalSNRLabel;
    QLabel *filteredSNRLabel;
    QLabel *snrImprovementLabel;
    QLabel *psnrLabel;
    QLabel *qualityAssessmentLabel;
    
    QPushButton *applyButton;
    QPushButton *cancelButton;

    // Data
    cv::Mat originalImage;
    cv::Mat denoisedImage;
    QString filterType;
    int kernelSize;
    double sigmaColor;
    double sigmaSpace;
    double originalSNR;
    double filteredSNR;
    double snrImprovement;
    double psnr;
    bool applied;
};

#endif // NOISEREMOVALDIALOG_H
