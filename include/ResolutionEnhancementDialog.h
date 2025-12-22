#ifndef RESOLUTIONENHANCEMENTDIALOG_H
#define RESOLUTIONENHANCEMENTDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <opencv2/opencv.hpp>

class ResolutionEnhancementDialog : public QDialog {
    Q_OBJECT

public:
    enum InterpolationMethod {
        Nearest,      // Fast, blocky
        Bilinear,     // Good for 2x
        Bicubic,      // Better quality
        Lanczos4,     // Best quality, slower
        EdgeDirected  // Custom edge-preserving
    };

    explicit ResolutionEnhancementDialog(const cv::Mat& inputImage, QWidget* parent = nullptr);
    
    cv::Mat getResultImage() const { return resultImage; }
    double getScaleFactor() const { return scaleSlider->value() / 100.0; }
    InterpolationMethod getMethod() const { return currentMethod; }

private slots:
    void onScaleChanged(int value);
    void onMethodChanged(int index);
    void onSharpenChanged(int value);
    void updatePreview();
    void onApply();
    void onCancel();

private:
    void setupUI();
    void enhanceResolution();
    void applySharpeningPass(cv::Mat& image, int strength);

    // Input/Output
    cv::Mat sourceImage;
    cv::Mat resultImage;
    InterpolationMethod currentMethod;

    // UI Components
    QSlider* scaleSlider;
    QSpinBox* scaleSpinBox;
    QLabel* scaleLabel;
    QLabel* targetSizeLabel;

    QComboBox* methodCombo;
    
    QSlider* sharpenSlider;
    QSpinBox* sharpenSpinBox;
    QLabel* sharpenLabel;

    QLabel* previewLabel;
    QPushButton* applyButton;
    QPushButton* cancelButton;
};

#endif // RESOLUTIONENHANCEMENTDIALOG_H
