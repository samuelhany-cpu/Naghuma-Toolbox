#ifndef SHARPENINGDIALOG_H
#define SHARPENINGDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <opencv2/opencv.hpp>

class SharpeningDialog : public QDialog {
    Q_OBJECT

public:
    explicit SharpeningDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~SharpeningDialog();

    cv::Mat getSharpenedImage() const { return sharpenedImage; }
    bool wasApplied() const { return applied; }
    QString getOperationType() const { return operationType; }
    
    // Parameters
    int getFilterType() const { return filterType; }
    double getAmount() const { return amount; }
    double getBoostFactor() const { return boostFactor; }

signals:
    void previewUpdated(const cv::Mat& preview);

private slots:
    void onFilterTypeChanged(int index);
    void onAmountChanged(double value);
    void onBoostFactorChanged(double value);
    void onResetClicked();
    void onApplyClicked();

private:
    void setupUI();
    void updatePreview();
    void applyLaplacianSharpening();
    void applyUnsharpMasking();
    void applyHighBoostFiltering();

    cv::Mat originalImage;
    cv::Mat sharpenedImage;
    bool applied;
    QString operationType;

    // Parameters
    int filterType;      // 0=Laplacian, 1=Unsharp Mask, 2=High-Boost
    double amount;       // For unsharp masking
    double boostFactor;  // k for high-boost filtering
    
    // UI elements
    QComboBox* filterCombo;
    QWidget* unsharpWidget;
    QWidget* highBoostWidget;
    QSlider* amountSlider;
    QDoubleSpinBox* amountSpinBox;
    QSlider* boostSlider;
    QDoubleSpinBox* boostSpinBox;
    QLabel* infoLabel;
    QPushButton* resetButton;
    QPushButton* applyButton;
};

#endif // SHARPENINGDIALOG_H
