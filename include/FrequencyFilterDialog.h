#ifndef FREQUENCYFILTERDIALOG_H
#define FREQUENCYFILTERDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <opencv2/opencv.hpp>
#include "ImageCanvas.h"

/**
 * @brief Dialog for advanced frequency domain filtering
 * 
 * Phase 19: Advanced Frequency Filters
 * - Butterworth Lowpass/Highpass
 * - Gaussian Lowpass/Highpass
 * - Homomorphic Filtering
 * - Bandpass/Bandreject Filters
 */
class FrequencyFilterDialog : public QDialog {
    Q_OBJECT

public:
    explicit FrequencyFilterDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~FrequencyFilterDialog();

    cv::Mat getFilteredImage() const { return filteredImage; }
    QString getFilterType() const { return filterType; }
    bool wasApplied() const { return applied; }

signals:
    void previewUpdated(const cv::Mat& preview);

private slots:
    void onFilterTypeChanged(int index);
    void onParameterChanged();
    void onApplyClicked();
    void onResetClicked();

private:
    void setupUI();
    void updatePreview();
    
    // Filter methods
    void applyButterworthLowpass();
    void applyButterworthHighpass();
    void applyGaussianLowpass();
    void applyGaussianHighpass();
    void applyBandpassFilter();
    void applyBandrejectFilter();
    void applyHomomorphicFilter();
    
    // Helper methods
    void createFrequencyFilter(cv::Mat& filter, int type, double cutoff, int order = 2);
    void createBandFilter(cv::Mat& filter, int type, double centerFreq, double bandwidth);
    void applyFrequencyFilter(const cv::Mat& input, cv::Mat& output, const cv::Mat& filter);
    void shiftDFT(cv::Mat& fImage);

    cv::Mat inputImage;
    cv::Mat filteredImage;
    cv::Mat previewImage;
    
    QString filterType;
    bool applied;

    // UI Components
    QComboBox* filterTypeCombo;
    ImageCanvas* previewCanvas;
    
    // Common parameters
    QSlider* cutoffFreqSlider;
    QLabel* cutoffFreqLabel;
    
    QSpinBox* filterOrderSpin;
    
    // Bandpass/Bandreject parameters
    QSlider* centerFreqSlider;
    QLabel* centerFreqLabel;
    QSlider* bandwidthSlider;
    QLabel* bandwidthLabel;
    
    // Homomorphic parameters
    QSlider* gammaLowSlider;
    QLabel* gammaLowLabel;
    QSlider* gammaHighSlider;
    QLabel* gammaHighLabel;
    QSlider* homoCutoffSlider;
    QLabel* homoCutoffLabel;
    
    // Parameter containers
    QWidget* standardParams;
    QWidget* bandParams;
    QWidget* homoParams;
    
    QPushButton* applyButton;
    QPushButton* resetButton;
    
    QLabel* infoLabel;
};

#endif // FREQUENCYFILTERDIALOG_H
