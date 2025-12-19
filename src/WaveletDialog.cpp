#include "ColorConversionDialog.h"
#include "ImageCanvas.h"
#include "color/ColorSpace.h"
#include <QGridLayout>
#include <QGroupBox>

ColorConversionDialog::ColorConversionDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), originalImage(image.clone()), applied(false), channelsVisible(false) {
    
    setWindowTitle("Color Space Conversion");
    setMinimumSize(1200, 700);
    
    currentSourceImage = originalImage.clone();
    
    setupUI();
    updateConversion();
}

ColorConversionDialog::~ColorConversionDialog() {
}

void ColorConversionDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *title = new QLabel("Interactive Color Space Converter");
    title->setStyleSheet("font-size: 16pt; font-weight: bold; color: #e879f9; padding: 10px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);
    
    // Color space selection
    QHBoxLayout *selectionLayout = new QHBoxLayout();
    
    selectionLayout->addWidget(new QLabel("Source Color Space:"));
    sourceColorSpaceCombo = new QComboBox();
    populateColorSpaces();
    sourceColorSpaceCombo->setCurrentText("RGB");
    connect(sourceColorSpaceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ColorConversionDialog::onSourceColorSpaceChanged);
    selectionLayout->addWidget(sourceColorSpaceCombo);
    
    selectionLayout->addWidget(new QLabel("  ?  "));
    
    selectionLayout->addWidget(new QLabel("Target Color Space:"));
    targetColorSpaceCombo = new QComboBox();
    targetColorSpaceCombo->addItems({"RGB", "HSV", "LAB", "YCbCr", "HSI", "Grayscale"});
    targetColorSpaceCombo->setCurrentText("HSV");
    connect(targetColorSpaceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ColorConversionDialog::onTargetColorSpaceChanged);
    selectionLayout->addWidget(targetColorSpaceCombo);
    
    selectionLayout->addStretch();
    mainLayout->addLayout(selectionLayout);
    
    // Info label
    infoLabel = new QLabel();
    infoLabel->setStyleSheet("color: #c4b5fd; padding: 5px; font-size: 10pt;");
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);
    
    // Image comparison
    QHBoxLayout *imagesLayout = new QHBoxLayout();
    
    // Original
    QVBoxLayout *originalLayout = new QVBoxLayout();
    QLabel *originalTitle = new QLabel("Original (RGB)");
    originalTitle->setStyleSheet("font-weight: bold; color: #e879f9;");
    originalTitle->setAlignment(Qt::AlignCenter);
    originalLayout->addWidget(originalTitle);
    
    originalCanvas = new ImageCanvas(this, "#e879f9");
    originalCanvas->setMinimumSize(400, 300);
    originalCanvas->setImage(originalImage);
    originalLayout->addWidget(originalCanvas);
    imagesLayout->addLayout(originalLayout);
    
    // Converted
    QVBoxLayout *convertedLayout = new QVBoxLayout();
    QLabel *convertedTitle = new QLabel("Converted");
    convertedTitle->setStyleSheet("font-weight: bold; color: #c026d3;");
    convertedTitle->setAlignment(Qt::AlignCenter);
    convertedLayout->addWidget(convertedTitle);
    
    convertedCanvas = new ImageCanvas(this, "#c026d3");
    convertedCanvas->setMinimumSize(400, 300);
    convertedLayout->addWidget(convertedCanvas);
    imagesLayout->addLayout(convertedLayout);
    
    mainLayout->addLayout(imagesLayout);
    
    // Channel visualization (initially hidden)
    channelsWidget = new QWidget();
    QHBoxLayout *channelsLayout = new QHBoxLayout(channelsWidget);
    
    // Channel 1
    QVBoxLayout *ch1Layout = new QVBoxLayout();
    channel1Label = new QLabel("Channel 1");
    channel1Label->setAlignment(Qt::AlignCenter);
    channel1Label->setStyleSheet("color: #ff6b6b; font-weight: bold;");
    ch1Layout->addWidget(channel1Label);
    channel1Canvas = new ImageCanvas(this, "#ff6b6b");
    channel1Canvas->setFixedSize(250, 200);
    ch1Layout->addWidget(channel1Canvas);
    channelsLayout->addLayout(ch1Layout);
    
    // Channel 2
    QVBoxLayout *ch2Layout = new QVBoxLayout();
    channel2Label = new QLabel("Channel 2");
    channel2Label->setAlignment(Qt::AlignCenter);
    channel2Label->setStyleSheet("color: #4ecdc4; font-weight: bold;");
    ch2Layout->addWidget(channel2Label);
    channel2Canvas = new ImageCanvas(this, "#4ecdc4");
    channel2Canvas->setFixedSize(250, 200);
    ch2Layout->addWidget(channel2Canvas);
    channelsLayout->addLayout(ch2Layout);
    
    // Channel 3
    QVBoxLayout *ch3Layout = new QVBoxLayout();
    channel3Label = new QLabel("Channel 3");
    channel3Label->setAlignment(Qt::AlignCenter);
    channel3Label->setStyleSheet("color: #ffe66d; font-weight: bold;");
    ch3Layout->addWidget(channel3Label);
    channel3Canvas = new ImageCanvas(this, "#ffe66d");
    channel3Canvas->setFixedSize(250, 200);
    ch3Layout->addWidget(channel3Canvas);
    channelsLayout->addLayout(ch3Layout);
    
    channelsWidget->setVisible(false);
    mainLayout->addWidget(channelsWidget);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    showChannelsButton = new QPushButton("Show Channels");
    showChannelsButton->setCheckable(true);
    showChannelsButton->setMaximumWidth(150);
    connect(showChannelsButton, &QPushButton::toggled, this, &ColorConversionDialog::onShowChannelsToggled);
    buttonLayout->addWidget(showChannelsButton);
    
    buttonLayout->addStretch();
    
    cancelButton = new QPushButton("Cancel");
    cancelButton->setMaximumWidth(120);
    connect(cancelButton, &QPushButton::clicked, this, &ColorConversionDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton);
    
    applyButton = new QPushButton("Apply");
    applyButton->setProperty("class", "accent");
    applyButton->setMaximumWidth(120);
    connect(applyButton, &QPushButton::clicked, this, &ColorConversionDialog::onApplyClicked);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void ColorConversionDialog::populateColorSpaces() {
    sourceColorSpaceCombo->addItems({"RGB", "HSV", "LAB", "YCbCr", "HSI", "Grayscale"});
}

void ColorConversionDialog::onSourceColorSpaceChanged(int index) {
    // Convert original image to selected source color space
    QString sourceSpace = sourceColorSpaceCombo->currentText();
    
    if (sourceSpace == "RGB") {
        currentSourceImage = originalImage.clone();
    } else if (sourceSpace == "HSV") {
        ColorSpace::RGBtoHSV(originalImage, currentSourceImage);
    } else if (sourceSpace == "LAB") {
        ColorSpace::RGBtoLAB(originalImage, currentSourceImage);
    } else if (sourceSpace == "YCbCr") {
        ColorSpace::RGBtoYCbCr(originalImage, currentSourceImage);
    } else if (sourceSpace == "HSI") {
        ColorSpace::RGBtoHSI(originalImage, currentSourceImage);
    } else if (sourceSpace == "Grayscale") {
        ColorSpace::RGBtoGray(originalImage, currentSourceImage);
    }
    
    updateConversion();
}

void ColorConversionDialog::onTargetColorSpaceChanged(int index) {
    updateConversion();
}

void ColorConversionDialog::updateConversion() {
    QString sourceSpace = sourceColorSpaceCombo->currentText();
    QString targetSpace = targetColorSpaceCombo->currentText();
    
    // Perform conversion
    if (sourceSpace == targetSpace) {
        convertedImage = currentSourceImage.clone();
        infoLabel->setText("Source and target are the same - no conversion needed");
    } else {
        // Convert from source to RGB first (if needed), then to target
        cv::Mat rgbImage;
        
        // Source to RGB
        if (sourceSpace == "RGB") {
            rgbImage = currentSourceImage.clone();
        } else if (sourceSpace == "HSV") {
            ColorSpace::HSVtoRGB(currentSourceImage, rgbImage);
        } else if (sourceSpace == "LAB") {
            ColorSpace::LABtoRGB(currentSourceImage, rgbImage);
        } else if (sourceSpace == "YCbCr") {
            ColorSpace::YCbCrtoRGB(currentSourceImage, rgbImage);
        } else if (sourceSpace == "HSI") {
            ColorSpace::HSItoRGB(currentSourceImage, rgbImage);
        } else if (sourceSpace == "Grayscale") {
            ColorSpace::GraytoRGB(currentSourceImage, rgbImage);
        }
        
        // RGB to target
        if (targetSpace == "RGB") {
            convertedImage = rgbImage.clone();
        } else if (targetSpace == "HSV") {
            ColorSpace::RGBtoHSV(rgbImage, convertedImage);
        } else if (targetSpace == "LAB") {
            ColorSpace::RGBtoLAB(rgbImage, convertedImage);
        } else if (targetSpace == "YCbCr") {
            ColorSpace::RGBtoYCbCr(rgbImage, convertedImage);
        } else if (targetSpace == "HSI") {
            ColorSpace::RGBtoHSI(rgbImage, convertedImage);
        } else if (targetSpace == "Grayscale") {
            ColorSpace::RGBtoGray(rgbImage, convertedImage);
        }
        
        infoLabel->setText(QString("Converted from %1 to %2").arg(sourceSpace, targetSpace));
    }
    
    // Update displays
    convertedCanvas->setImage(convertedImage);
    
    // Update channels if visible
    if (channelsVisible) {
        updateChannelVisualization();
    }
    
    emit previewUpdated(convertedImage);
}

void ColorConversionDialog::updateChannelVisualization() {
    QString targetSpace = targetColorSpaceCombo->currentText();
    
    if (convertedImage.channels() == 1) {
        // Grayscale - show same image in all channels
        channel1Canvas->setImage(convertedImage);
        channel2Canvas->clear();
        channel3Canvas->clear();
        channel1Label->setText("Gray");
        channel2Label->setText("");
        channel3Label->setText("");
    } else {
        // Multi-channel image
        std::vector<cv::Mat> channels;
        ColorSpace::extractChannels(convertedImage, channels);
        
        if (channels.size() >= 3) {
            channel1Canvas->setImage(channels[0]);
            channel2Canvas->setImage(channels[1]);
            channel3Canvas->setImage(channels[2]);
            
            channel1Label->setText(getChannelName(targetSpace, 0));
            channel2Label->setText(getChannelName(targetSpace, 1));
            channel3Label->setText(getChannelName(targetSpace, 2));
        }
    }
}

QString ColorConversionDialog::getChannelName(const QString& colorSpace, int channelIndex) {
    auto names = ColorSpace::getChannelNames(colorSpace);
    if (channelIndex < names.size()) {
        return names[channelIndex];
    }
    return QString("Channel %1").arg(channelIndex);
}

void ColorConversionDialog::onShowChannelsToggled(bool checked) {
    channelsVisible = checked;
    channelsWidget->setVisible(checked);
    
    if (checked) {
        showChannelsButton->setText("Hide Channels");
        updateChannelVisualization();
        setMinimumHeight(900);
    } else {
        showChannelsButton->setText("Show Channels");
        setMinimumHeight(700);
    }
}

void ColorConversionDialog::onApplyClicked() {
    applied = true;
    accept();
}

void ColorConversionDialog::onCancelClicked() {
    applied = false;
    reject();
}

QString ColorConversionDialog::getSourceColorSpace() const {
    return sourceColorSpaceCombo->currentText();
}

QString ColorConversionDialog::getTargetColorSpace() const {
    return targetColorSpaceCombo->currentText();
}

#include "moc_ColorConversionDialog.cpp"
