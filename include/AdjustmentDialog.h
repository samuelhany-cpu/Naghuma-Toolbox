#ifndef ADJUSTMENTDIALOG_H
#define ADJUSTMENTDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <opencv2/opencv.hpp>

class AdjustmentDialog : public QDialog {
    Q_OBJECT

public:
    explicit AdjustmentDialog(const cv::Mat& sourceImage, QWidget *parent = nullptr);
    ~AdjustmentDialog();

    cv::Mat getAdjustedImage() const { return adjustedImage; }
    bool wasApplied() const { return applied; }
    int getBrightness() const { return brightness; }
    int getContrast() const { return contrast; }

signals:
    void previewUpdated(const cv::Mat& preview);

private slots:
    void onBrightnessChanged(int value);
    void onContrastChanged(int value);
    void onResetClicked();
    void onApplyClicked();
    void onCancelClicked();

private:
    void setupUI();
    void updatePreview();
    void applyAdjustments();

    cv::Mat sourceImage;
    cv::Mat adjustedImage;
    bool applied;

    int brightness;
    int contrast;

    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    QSpinBox *brightnessSpinBox;
    QSpinBox *contrastSpinBox;
    QPushButton *resetButton;
    QPushButton *applyButton;
    QPushButton *cancelButton;
};

#endif // ADJUSTMENTDIALOG_H
