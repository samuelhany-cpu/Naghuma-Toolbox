#ifndef BLURDIALOG_H
#define BLURDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <opencv2/opencv.hpp>

class BlurDialog : public QDialog {
    Q_OBJECT

public:
    enum BlurType {
        Gaussian,
        Median,
        Bilateral
    };

    explicit BlurDialog(const cv::Mat& inputImage, QWidget* parent = nullptr);
    
    cv::Mat getResultImage() const { return resultImage; }
    BlurType getBlurType() const { return currentBlurType; }
    int getKernelSize() const { return kernelSizeSlider->value(); }

private slots:
    void onBlurTypeChanged();
    void onKernelSizeChanged(int value);
    void onSigmaColorChanged(int value);
    void onSigmaSpaceChanged(int value);
    void updatePreview();
    void onApply();
    void onCancel();

private:
    void setupUI();
    void applyBlur();

    // Input/Output
    cv::Mat sourceImage;
    cv::Mat resultImage;
    BlurType currentBlurType;

    // UI Components
    QButtonGroup* blurTypeGroup;
    QRadioButton* gaussianRadio;
    QRadioButton* medianRadio;
    QRadioButton* bilateralRadio;

    QSlider* kernelSizeSlider;
    QSpinBox* kernelSizeSpinBox;
    QLabel* kernelSizeLabel;

    // Bilateral-specific controls
    QSlider* sigmaColorSlider;
    QSpinBox* sigmaColorSpinBox;
    QLabel* sigmaColorLabel;

    QSlider* sigmaSpaceSlider;
    QSpinBox* sigmaSpaceSpinBox;
    QLabel* sigmaSpaceLabel;

    QLabel* previewLabel;
    QPushButton* applyButton;
    QPushButton* cancelButton;
};

#endif // BLURDIALOG_H
