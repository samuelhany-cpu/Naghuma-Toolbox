#include "RightSidebarWidget.h"
#include "HistogramWidget.h"
#include "LayerManager.h"
#include <QLabel>

RightSidebarWidget::RightSidebarWidget(QWidget *parent)
    : QWidget(parent), layerManager(new LayerManager(this)) {
    
    setFixedWidth(350);
    setStyleSheet(
        "background: rgba(30, 30, 36, 0.95); "
        "border-left: 2px solid rgba(165, 148, 249, 0.25);"
    );
    
    setupUI();
}

RightSidebarWidget::~RightSidebarWidget() {
}

void RightSidebarWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    tabWidget = new QTabWidget();
    tabWidget->setStyleSheet(R"(
        QTabWidget::pane {
            border: none;
            background: rgba(30, 30, 36, 0.8);
        }
        QTabBar::tab {
            background: rgba(43, 45, 66, 0.6);
            color: #B8BCC4;
            padding: 12px 24px;
            border: none;
            border-bottom: 2px solid transparent;
            font-weight: 500;
        }
        QTabBar::tab:selected {
            background: rgba(165, 148, 249, 0.15);
            color: #A594F9;
            border-bottom: 2px solid #A594F9;
            font-weight: 600;
        }
        QTabBar::tab:hover {
            background: rgba(74, 77, 98, 0.5);
            color: #A594F9;
        }
    )");
    
    createHistogramTab();
    createLayersTab();
    
    mainLayout->addWidget(tabWidget);
}

void RightSidebarWidget::createHistogramTab() {
    QWidget *histogramTab = new QWidget();
    histogramTab->setStyleSheet("background: rgba(30, 30, 36, 0.8);");
    QVBoxLayout *layout = new QVBoxLayout(histogramTab);
    layout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *title = new QLabel("Image Histogram");
    title->setStyleSheet("font-size: 12pt; font-weight: 600; color: #A594F9; padding: 10px; background: transparent;");
    layout->addWidget(title);
    
    histogramWidget = new HistogramWidget(histogramTab);
    histogramWidget->setMinimumHeight(400);
    layout->addWidget(histogramWidget);
    
    layout->addStretch();
    
    tabWidget->addTab(histogramTab, "Histogram");
}

void RightSidebarWidget::createLayersTab() {
    QWidget *layersTab = new QWidget();
    layersTab->setStyleSheet("background: rgba(30, 30, 36, 0.8);");
    QVBoxLayout *layout = new QVBoxLayout(layersTab);
    layout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *title = new QLabel("Processing Layers");
    title->setStyleSheet("font-size: 12pt; font-weight: 600; color: #A594F9; padding: 10px; background: transparent;");
    layout->addWidget(title);
    
    layersListWidget = new QListWidget();
    layersListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    layersListWidget->setStyleSheet(R"(
        QListWidget {
            background: rgba(43, 45, 66, 0.4);
            border: 2px solid rgba(165, 148, 249, 0.3);
            border-radius: 10px;
            padding: 8px;
            color: #EDF2F4;
        }
        QListWidget::item {
            background: rgba(58, 61, 82, 0.5);
            border: 1px solid rgba(165, 148, 249, 0.25);
            border-radius: 6px;
            padding: 10px;
            margin: 4px;
            color: #EDF2F4;
        }
        QListWidget::item:selected {
            background: rgba(165, 148, 249, 0.25);
            border: 1px solid #A594F9;
            color: #A594F9;
        }
        QListWidget::item:hover {
            background: rgba(74, 77, 98, 0.6);
            border: 1px solid rgba(165, 148, 249, 0.4);
        }
        QCheckBox {
            color: #EDF2F4;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 4px;
            border: 2px solid rgba(165, 148, 249, 0.5);
            background: rgba(43, 45, 66, 0.5);
        }
        QCheckBox::indicator:checked {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            border: 2px solid #A594F9;
        }
        QCheckBox::indicator:hover {
            border: 2px solid #A594F9;
        }
    )");
    connect(layersListWidget, &QListWidget::itemChanged, 
            this, &RightSidebarWidget::onLayerItemChanged);
    layout->addWidget(layersListWidget);
    
    // Buttons layout
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    
    selectAllButton = new QPushButton("Select All");
    selectAllButton->setStyleSheet(
        "background-color: rgba(58, 61, 82, 0.6); "
        "color: #B8BCC4; "
        "border: 1px solid rgba(165, 148, 249, 0.3); "
        "border-radius: 8px; "
        "padding: 8px 12px; "
        "font-weight: 600;"
    );
    connect(selectAllButton, &QPushButton::clicked, this, &RightSidebarWidget::onSelectAllClicked);
    buttonsLayout->addWidget(selectAllButton);
    
    deselectAllButton = new QPushButton("Deselect All");
    deselectAllButton->setStyleSheet(
        "background-color: rgba(58, 61, 82, 0.6); "
        "color: #B8BCC4; "
        "border: 1px solid rgba(165, 148, 249, 0.3); "
        "border-radius: 8px; "
        "padding: 8px 12px; "
        "font-weight: 600;"
    );
    connect(deselectAllButton, &QPushButton::clicked, this, &RightSidebarWidget::onDeselectAllClicked);
    buttonsLayout->addWidget(deselectAllButton);
    
    layout->addLayout(buttonsLayout);
    
    removeLayerButton = new QPushButton("Remove Selected Layers");
    removeLayerButton->setStyleSheet(
        "background-color: rgba(231, 111, 81, 0.25); "
        "color: #E76F51; "
        "border: 1px solid rgba(231, 111, 81, 0.5); "
        "border-radius: 8px; "
        "padding: 10px; "
        "font-weight: 600;"
    );
    removeLayerButton->setEnabled(false);
    connect(removeLayerButton, &QPushButton::clicked, 
            this, &RightSidebarWidget::onRemoveLayerClicked);
    layout->addWidget(removeLayerButton);
    
    tabWidget->addTab(layersTab, "Layers");
    
    // Initialize with original layer
    updateLayersList();
}

void RightSidebarWidget::updateHistogram(const cv::Mat& image) {
    if (histogramWidget && !image.empty()) {
        histogramWidget->setImage(image);
    }
}

void RightSidebarWidget::addLayer(const QString& name, const QString& type, const cv::Mat& image,
                                   std::function<cv::Mat(const cv::Mat&)> operation) {
    layerManager->addLayer(name, type, image, operation);
    updateLayersList();
}

void RightSidebarWidget::clearLayers() {
    layerManager->clearLayers();
    updateLayersList();
}

void RightSidebarWidget::removeLayer(int layerIndex) {
    layerManager->removeLayer(layerIndex);
    updateLayersList();
}

int RightSidebarWidget::getLayerCount() const {
    return layerManager->getLayerCount();
}

cv::Mat RightSidebarWidget::getLayerImage(int layerIndex) const {
    const auto& layers = layerManager->getLayers();
    if (layerIndex >= 0 && layerIndex < layers.size()) {
        return layers[layerIndex].image;
    }
    return cv::Mat();
}

cv::Mat RightSidebarWidget::rebuildImage(const cv::Mat& original, int upToLayer) const {
    return layerManager->rebuildFromLayers(original, upToLayer);
}

void RightSidebarWidget::resetHistogram() {
    if (histogramWidget) {
        histogramWidget->clear();
    }
}

void RightSidebarWidget::updateLayersList() {
    layersListWidget->clear();
    
    // Add original image as base layer (not removable, no checkbox)
    QListWidgetItem *originalItem = new QListWidgetItem("[IMG] Original Image");
    originalItem->setData(Qt::UserRole, -1);
    originalItem->setForeground(QBrush(QColor("#B8BCC4")));
    originalItem->setFlags(originalItem->flags() & ~Qt::ItemIsUserCheckable);
    layersListWidget->addItem(originalItem);
    
    // Add processing layers with checkboxes
    const auto& layers = layerManager->getLayers();
    for (int i = 0; i < layers.size(); ++i) {
        QString icon = "[FX]";
        if (layers[i].type == "filter") icon = "[FLT]";
        else if (layers[i].type == "transform") icon = "[TRF]";
        else if (layers[i].type == "adjustment") icon = "[ADJ]";
        else if (layers[i].type == "color") icon = "[CLR]";
        
        QListWidgetItem *item = new QListWidgetItem(
            QString("%1 %2").arg(icon).arg(layers[i].name));
        item->setData(Qt::UserRole, i);
        item->setForeground(QBrush(QColor("#A594F9")));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        layersListWidget->addItem(item);
    }
}

void RightSidebarWidget::onLayerItemChanged(QListWidgetItem* item) {
    // Update button state based on checked items
    updateRemoveButtonState();
}

void RightSidebarWidget::onSelectAllClicked() {
    for (int i = 0; i < layersListWidget->count(); ++i) {
        QListWidgetItem *item = layersListWidget->item(i);
        int layerIndex = item->data(Qt::UserRole).toInt();
        if (layerIndex >= 0) {  // Skip original image
            item->setCheckState(Qt::Checked);
        }
    }
    updateRemoveButtonState();
}

void RightSidebarWidget::onDeselectAllClicked() {
    for (int i = 0; i < layersListWidget->count(); ++i) {
        QListWidgetItem *item = layersListWidget->item(i);
        item->setCheckState(Qt::Unchecked);
    }
    updateRemoveButtonState();
}

void RightSidebarWidget::updateRemoveButtonState() {
    bool hasChecked = false;
    for (int i = 0; i < layersListWidget->count(); ++i) {
        QListWidgetItem *item = layersListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            hasChecked = true;
            break;
        }
    }
    removeLayerButton->setEnabled(hasChecked);
}

void RightSidebarWidget::onLayerItemClicked(QListWidgetItem* item) {
    // No longer needed - checkboxes handle selection
}

void RightSidebarWidget::onRemoveLayerClicked() {
    // Collect all checked layer indices
    QList<int> layersToRemove;
    for (int i = 0; i < layersListWidget->count(); ++i) {
        QListWidgetItem *item = layersListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            int layerIndex = item->data(Qt::UserRole).toInt();
            if (layerIndex >= 0) {
                layersToRemove.append(layerIndex);
            }
        }
    }
    
    if (!layersToRemove.isEmpty()) {
        emit layersRemoveRequested(layersToRemove);
    }
}
