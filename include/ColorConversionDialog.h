#pragma once

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <opencv2/opencv.hpp>

class ImageCanvas;

/**
 * @brief Dialog for interactive color space conversion with live preview
 */
class ColorConversionDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit ColorConversionDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~ColorConversionDialog();
    
    // Getters
    cv::Mat getConvertedImage() const { return convertedImage; }
    QString getSourceColorSpace() const;
    QString getTargetColorSpace() const;
    bool wasApplied() const { return applied; }
    
signals:
    void previewUpdated(const cv::Mat& preview);
    
private slots:
    void onSourceColorSpaceChanged(int index);
    void onTargetColorSpaceChanged(int index);
    void onApplyClicked();
    void onCancelClicked();
    void onShowChannelsToggled(bool checked);
    
private:
    // UI Components
    QComboBox *sourceColorSpaceCombo;
    QComboBox *targetColorSpaceCombo;
    QPushButton *applyButton;
    QPushButton *cancelButton;
    QPushButton *showChannelsButton;
    QLabel *infoLabel;
    
    // Image canvases
    ImageCanvas *originalCanvas;
    ImageCanvas *convertedCanvas;
    
    // Channel visualization canvases
    ImageCanvas *channel1Canvas;
    ImageCanvas *channel2Canvas;
    ImageCanvas *channel3Canvas;
    QLabel *channel1Label;
    QLabel *channel2Label;
    QLabel *channel3Label;
    QWidget *channelsWidget;
    
    // Image data
    cv::Mat originalImage;
    cv::Mat convertedImage;
    cv::Mat currentSourceImage;
    bool applied;
    bool channelsVisible;
    
    // Helper methods
    void setupUI();
    void updateConversion();
    void updateChannelVisualization();
    void populateColorSpaces();
    QString getChannelName(const QString& colorSpace, int channelIndex);
};
