#ifndef CANNYDIALOG_H
#define CANNYDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <opencv2/opencv.hpp>

class CannyDialog : public QDialog {
    Q_OBJECT

public:
    explicit CannyDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~CannyDialog();

    cv::Mat getCannyImage() const { return cannyImage; }
    bool wasApplied() const { return applied; }
    
    // Get parameters
    double getLowThreshold() const { return lowThreshold; }
    double getHighThreshold() const { return highThreshold; }
    double getSigma() const { return sigma; }
    int getApertureSize() const { return apertureSize; }
    bool getL2Gradient() const { return useL2; }

signals:
    void previewUpdated(const cv::Mat& preview);

private slots:
    void onLowThresholdChanged(int value);
    void onHighThresholdChanged(int value);
    void onSigmaChanged(double value);
    void onApertureSizeChanged(int index);
    void onL2GradientChanged(int state);
    void onAutoThresholdClicked();
    void onResetClicked();
    void onApplyClicked();

private:
    void setupUI();
    void updatePreview();
    void calculateAutoThresholds();

    cv::Mat originalImage;
    cv::Mat cannyImage;
    bool applied;

    // Parameters
    double lowThreshold;
    double highThreshold;
    double sigma;
    int apertureSize;
    bool useL2;

    // UI elements
    QSlider* lowThresholdSlider;
    QSpinBox* lowThresholdSpinBox;
    QSlider* highThresholdSlider;
    QSpinBox* highThresholdSpinBox;
    QDoubleSpinBox* sigmaSpinBox;
    QComboBox* apertureSizeCombo;
    QCheckBox* l2GradientCheckBox;
    QLabel* infoLabel;
    QPushButton* autoThresholdButton;
    QPushButton* resetButton;
    QPushButton* applyButton;
};

#endif // CANNYDIALOG_H
