#include "BrushDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QScrollArea>
#include <cmath>

BrushDialog::BrushDialog(BrushTool* brushTool, QWidget *parent)
    : QDialog(parent), brushTool(brushTool), selectedFilter("None") {
    
    setWindowTitle("Brush Settings");
    setFixedSize(420, 650);
    
    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #1a0a1f, stop:1 #251e35);
        }
    )");
    
    setupUI();
}

BrushDialog::~BrushDialog() {
}

void BrushDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *titleLabel = new QLabel("Brush Tool Settings");
    titleLabel->setStyleSheet("font-size: 13pt; font-weight: bold; color: #e879f9; padding: 8px;");
    mainLayout->addWidget(titleLabel);
    
    // Create scroll area for controls
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");
    
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    scrollLayout->setSpacing(10);
    
    // Brush Type
    QGroupBox *typeGroup = new QGroupBox("Brush Type");
    QVBoxLayout *typeLayout = new QVBoxLayout(typeGroup);
    
    brushTypeCombo = new QComboBox();
    brushTypeCombo->addItem("Circle");
    brushTypeCombo->addItem("Square");
    brushTypeCombo->addItem("Soft Edge");
    brushTypeCombo->setCurrentIndex(static_cast<int>(brushTool->getBrushType()));
    typeLayout->addWidget(brushTypeCombo);
    scrollLayout->addWidget(typeGroup);
    
    // Brush Size
    QGroupBox *sizeGroup = new QGroupBox("Brush Size (1-100 px)");
    QVBoxLayout *sizeLayout = new QVBoxLayout(sizeGroup);
    
    sizeSlider = new QSlider(Qt::Horizontal);
    sizeSlider->setRange(1, 100);
    sizeSlider->setValue(brushTool->getBrushSize());
    
    sizeSpinBox = new QSpinBox();
    sizeSpinBox->setRange(1, 100);
    sizeSpinBox->setValue(brushTool->getBrushSize());
    
    sizeLayout->addWidget(sizeSlider);
    sizeLayout->addWidget(sizeSpinBox);
    scrollLayout->addWidget(sizeGroup);
    
    // Opacity
    QGroupBox *opacityGroup = new QGroupBox("Opacity (0-100%)");
    QVBoxLayout *opacityLayout = new QVBoxLayout(opacityGroup);
    
    opacitySlider = new QSlider(Qt::Horizontal);
    opacitySlider->setRange(0, 100);
    opacitySlider->setValue(brushTool->getBrushOpacity());
    
    opacitySpinBox = new QSpinBox();
    opacitySpinBox->setRange(0, 100);
    opacitySpinBox->setValue(brushTool->getBrushOpacity());
    opacitySpinBox->setSuffix("%");
    
    opacityLayout->addWidget(opacitySlider);
    opacityLayout->addWidget(opacitySpinBox);
    scrollLayout->addWidget(opacityGroup);
    
    // Color
    QGroupBox *colorGroup = new QGroupBox("Brush Color");
    QHBoxLayout *colorLayout = new QHBoxLayout(colorGroup);
    
    colorButton = new QPushButton("Choose Color");
    QColor currentColor = brushTool->getBrushColor();
    colorButton->setStyleSheet(QString(
        "background-color: %1; "
        "color: %2; "
        "border: 2px solid #e879f9; "
        "border-radius: 8px; "
        "padding: 10px; "
        "font-weight: 600;")
        .arg(currentColor.name())
        .arg(currentColor.lightness() > 128 ? "black" : "white"));
    
    colorLayout->addWidget(colorButton);
    scrollLayout->addWidget(colorGroup);
    
    // Eraser Mode
    eraserCheckBox = new QCheckBox("Eraser Mode");
    eraserCheckBox->setChecked(brushTool->isEraserMode());
    eraserCheckBox->setStyleSheet("color: #e879f9; font-size: 11pt; padding: 10px;");
    scrollLayout->addWidget(eraserCheckBox);
    
    // Filter Selection
    QGroupBox *filterGroup = new QGroupBox("Brush Effect Filter");
    QVBoxLayout *filterLayout = new QVBoxLayout(filterGroup);
    
    filterCombo = new QComboBox();
    filterCombo->addItem("None (Color Paint)");
    filterCombo->addItem("Blur");
    filterCombo->addItem("Sharpen");
    filterCombo->addItem("Edge Detection");
    filterCombo->addItem("Grayscale");
    filterCombo->addItem("Invert");
    filterCombo->setCurrentIndex(0);
    filterLayout->addWidget(filterCombo);
    
    QLabel *filterInfo = new QLabel("Paint with filter effects instead of solid color");
    filterInfo->setStyleSheet("color: #c4b5fd; font-size: 9pt; padding: 5px;");
    filterInfo->setWordWrap(true);
    filterLayout->addWidget(filterInfo);
    
    scrollLayout->addWidget(filterGroup);
    
    // Preview
    QGroupBox *previewGroup = new QGroupBox("Brush Preview");
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);
    
    previewLabel = new QLabel();
    previewLabel->setFixedSize(120, 120);
    previewLabel->setStyleSheet(
        "background-color: white; "
        "border: 2px solid #e879f9; "
        "border-radius: 8px;"
    );
    previewLayout->addWidget(previewLabel, 0, Qt::AlignCenter);
    scrollLayout->addWidget(previewGroup);
    
    scrollLayout->addStretch();
    
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);
    
    // Close button
    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setProperty("class", "accent");
    closeBtn->setMaximumWidth(150);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    mainLayout->addLayout(btnLayout);
    
    // Connect signals
    connect(brushTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BrushDialog::onBrushTypeChanged);
    connect(sizeSlider, &QSlider::valueChanged, sizeSpinBox, &QSpinBox::setValue);
    connect(sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), sizeSlider, &QSlider::setValue);
    connect(sizeSlider, &QSlider::valueChanged, this, &BrushDialog::onBrushSizeChanged);
    
    connect(opacitySlider, &QSlider::valueChanged, opacitySpinBox, &QSpinBox::setValue);
    connect(opacitySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), opacitySlider, &QSlider::setValue);
    connect(opacitySlider, &QSlider::valueChanged, this, &BrushDialog::onOpacityChanged);
    
    connect(colorButton, &QPushButton::clicked, this, &BrushDialog::onColorClicked);
    connect(eraserCheckBox, &QCheckBox::toggled, this, &BrushDialog::onEraserToggled);
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BrushDialog::onFilterChanged);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    
    updateBrushPreview();
}

void BrushDialog::onBrushTypeChanged(int index) {
    brushTool->setBrushType(static_cast<BrushType>(index));
    updateBrushPreview();
}

void BrushDialog::onBrushSizeChanged(int value) {
    brushTool->setBrushSize(value);
    updateBrushPreview();
}

void BrushDialog::onOpacityChanged(int value) {
    brushTool->setBrushOpacity(value);
    updateBrushPreview();
}

void BrushDialog::onColorClicked() {
    QColor color = QColorDialog::getColor(brushTool->getBrushColor(), this, "Select Brush Color");
    if (color.isValid()) {
        brushTool->setBrushColor(color);
        colorButton->setStyleSheet(QString(
            "background-color: %1; "
            "color: %2; "
            "border: 2px solid #e879f9; "
            "border-radius: 8px; "
            "padding: 10px; "
            "font-weight: 600;")
            .arg(color.name())
            .arg(color.lightness() > 128 ? "black" : "white"));
        updateBrushPreview();
    }
}

void BrushDialog::onEraserToggled(bool checked) {
    brushTool->setEraserMode(checked);
    updateBrushPreview();
}

void BrushDialog::onFilterChanged(int index) {
    QStringList filters = {"None", "Blur", "Sharpen", "Edge Detection", "Grayscale", "Invert"};
    if (index >= 0 && index < filters.size()) {
        selectedFilter = filters[index];
    }
}

void BrushDialog::updateBrushPreview() {
    // Create preview image
    cv::Mat preview(120, 120, CV_8UC3, cv::Scalar(255, 255, 255));
    
    // Draw brush preview in center
    cv::Point center(60, 60);
    
    QColor color = brushTool->getBrushColor();
    if (brushTool->isEraserMode()) {
        color = Qt::white;
    }
    
    cv::Scalar cvColor(color.blue(), color.green(), color.red());
    int size = std::min(brushTool->getBrushSize(), 100);
    
    switch (brushTool->getBrushType()) {
        case BrushType::Circle:
            cv::circle(preview, center, size / 2, cvColor, -1);
            break;
        case BrushType::Square: {
            int halfSize = size / 2;
            cv::rectangle(preview, 
                         cv::Point(center.x - halfSize, center.y - halfSize),
                         cv::Point(center.x + halfSize, center.y + halfSize),
                         cvColor, -1);
            break;
        }
        case BrushType::SoftEdge: {
            int radius = size / 2;
            for (int y = -radius; y <= radius; ++y) {
                for (int x = -radius; x <= radius; ++x) {
                    float dist = std::sqrt(x*x + y*y);
                    if (dist <= radius) {
                        float softness = 1.0f - (dist / radius);
                        softness = softness * softness;
                        
                        int px = center.x + x;
                        int py = center.y + y;
                        
                        if (px >= 0 && px < 120 && py >= 0 && py < 120) {
                            cv::Vec3b& pixel = preview.at<cv::Vec3b>(py, px);
                            pixel[0] = static_cast<uchar>(255 * (1 - softness) + cvColor[0] * softness);
                            pixel[1] = static_cast<uchar>(255 * (1 - softness) + cvColor[1] * softness);
                            pixel[2] = static_cast<uchar>(255 * (1 - softness) + cvColor[2] * softness);
                        }
                    }
                }
            }
            break;
        }
    }
    
    // Convert to QPixmap
    cv::Mat rgb;
    cv::cvtColor(preview, rgb, cv::COLOR_BGR2RGB);
    QImage qimg(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    previewLabel->setPixmap(QPixmap::fromImage(qimg).scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
