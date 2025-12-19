#ifndef INTENSITYTRANSFORMDIALOG_H
#define INTENSITYTRANSFORMDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <opencv2/opencv.hpp>

class IntensityTransformDialog : public QDialog {
    Q_OBJECT

public:
    explicit IntensityTransformDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~IntensityTransformDialog();

    cv::Mat getTransformedImage() const { return transformedImage; }
    bool wasApplied() const { return applied; }
    QString getOperationType() const { return operationType; }
    
    // Parameters
    double getGamma() const { return gamma; }
    double getLogC() const { return logC; }
    int getTransformType() const { return transformType; }

signals:
    void previewUpdated(const cv::Mat& preview);

private slots:
    void onTransformTypeChanged(int index);
    void onGammaChanged(double value);
    void onLogCChanged(double value);
    void onResetClicked();
    void onApplyClicked();

private:
    void setupUI();
    void updatePreview();
    void applyGammaCorrection();
    void applyLogTransform();
    void applyPowerLaw();

    cv::Mat originalImage;
    cv::Mat transformedImage;
    bool applied;
    QString operationType;

    // Parameters
    int transformType;  // 0=Gamma, 1=Log, 2=Power Law
    double gamma;
    double logC;
    
    // UI elements
    QComboBox* transformCombo;
    QWidget* gammaWidget;
    QWidget* logWidget;
    QSlider* gammaSlider;
    QDoubleSpinBox* gammaSpinBox;
    QSlider* logCSlider;
    QDoubleSpinBox* logCSpinBox;
    QLabel* infoLabel;
    QPushButton* resetButton;
    QPushButton* applyButton;
};

#endif // INTENSITYTRANSFORMDIALOG_H
