#include "RightSidebarWidget.h"
#include "HistogramWidget.h"
#include "LayerManager.h"
#include <QLabel>

RightSidebarWidget::RightSidebarWidget(QWidget *parent)
    : QWidget(parent), layerManager(new LayerManager(this)) {
    
    setFixedWidth(350);
    setStyleSheet(
        "background: rgba(31, 21, 53, 0.95); "
        "border-left: 2px solid rgba(232, 121, 249, 0.25);"
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
            background: transparent;
        }
        QTabBar::tab {
            background: rgba(45, 37, 71, 0.5);
            color: #c4b5fd;
            padding: 12px 24px;
            border: none;
            border-bottom: 2px solid transparent;
        }
        QTabBar::tab:selected {
            background: rgba(232, 121, 249, 0.15);
            color: #e879f9;
            border-bottom: 2px solid #e879f9;
        }
        QTabBar::tab:hover {
            background: rgba(91, 75, 115, 0.4);
            color: #e879f9;
        }
    )");
    
    createHistogramTab();
    createLayersTab();
    
    mainLayout->addWidget(tabWidget);
}

void RightSidebarWidget::createHistogramTab() {
    QWidget *histogramTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(histogramTab);
    layout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *title = new QLabel("Image Histogram");
    title->setStyleSheet("font-size: 12pt; font-weight: 600; color: #e879f9; padding: 10px;");
    layout->addWidget(title);
    
    histogramWidget = new HistogramWidget(histogramTab);
    histogramWidget->setMinimumHeight(400);
    layout->addWidget(histogramWidget);
    
    layout->addStretch();
    
    tabWidget->addTab(histogramTab, "Histogram");
}

void RightSidebarWidget::createLayersTab() {
    QWidget *layersTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(layersTab);
    layout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *title = new QLabel("Processing Layers");
    title->setStyleSheet("font-size: 12pt; font-weight: 600; color: #e879f9; padding: 10px;");
    layout->addWidget(title);
    
    layersListWidget = new QListWidget();
    layersListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    layersListWidget->setStyleSheet(R"(
        QListWidget {
            background: rgba(45, 37, 71, 0.3);
            border: 2px solid rgba(91, 75, 115, 0.5);
            border-radius: 10px;
            padding: 8px;
            color: #f3e8ff;
        }
        QListWidget::item {
            background: rgba(91, 75, 115, 0.3);
            border: 1px solid rgba(232, 121, 249, 0.2);
            border-radius: 6px;
            padding: 10px;
            margin: 4px;
        }
        QListWidget::item:selected {
            background: rgba(232, 121, 249, 0.3);
            border: 1px solid #e879f9;
            color: #e879f9;
        }
        QListWidget::item:hover {
            background: rgba(91, 75, 115, 0.5);
            border: 1px solid rgba(232, 121, 249, 0.4);
        }
        QCheckBox {
            color: #f3e8ff;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border-radius: 4px;
            border: 2px solid rgba(232, 121, 249, 0.5);
            background: rgba(45, 37, 71, 0.5);
        }
        QCheckBox::indicator:checked {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #e879f9, stop:1 #c026d3);
            border: 2px solid #e879f9;
        }
        QCheckBox::indicator:hover {
            border: 2px solid #e879f9;
        }
    )");
    connect(layersListWidget, &QListWidget::itemChanged, 
            this, &RightSidebarWidget::onLayerItemChanged);
    layout->addWidget(layersListWidget);
    
    // Buttons layout
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    
    selectAllButton = new QPushButton("Select All");
    selectAllButton->setStyleSheet(
        "background-color: rgba(91, 75, 115, 0.4); "
        "color: #c4b5fd; "
        "border: 1px solid rgba(232, 121, 249, 0.3); "
        "border-radius: 8px; "
        "padding: 8px 12px; "
        "font-weight: 600;"
    );
    connect(selectAllButton, &QPushButton::clicked, this, &RightSidebarWidget::onSelectAllClicked);
    buttonsLayout->addWidget(selectAllButton);
    
    deselectAllButton = new QPushButton("Deselect All");
    deselectAllButton->setStyleSheet(
        "background-color: rgba(91, 75, 115, 0.4); "
        "color: #c4b5fd; "
        "border: 1px solid rgba(232, 121, 249, 0.3); "
        "border-radius: 8px; "
        "padding: 8px 12px; "
        "font-weight: 600;"
    );
    connect(deselectAllButton, &QPushButton::clicked, this, &RightSidebarWidget::onDeselectAllClicked);
    buttonsLayout->addWidget(deselectAllButton);
    
    layout->addLayout(buttonsLayout);
    
    removeLayerButton = new QPushButton("Remove Selected Layers");
    removeLayerButton->setStyleSheet(
        "background-color: rgba(251, 113, 133, 0.25); "
        "color: #fb7185; "
        "border: 1px solid rgba(251, 113, 133, 0.5); "
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
    originalItem->setForeground(QBrush(QColor("#c4b5fd")));
    originalItem->setFlags(originalItem->flags() & ~Qt::ItemIsUserCheckable);
    layersListWidget->addItem(originalItem);
    
    // Add processing layers with checkboxes
    const auto& layers = layerManager->getLayers();
    for (int i = 0; i < layers.size(); ++i) {
        QString icon = "[FX]";
        if (layers[i].type == "filter") icon = "[FLT]";
        else if (layers[i].type == "transform") icon = "[TRF]";
        else if (layers[i].type == "adjustment") icon = "[ADJ]";
        
        QListWidgetItem *item = new QListWidgetItem(
            QString("%1 %2").arg(icon).arg(layers[i].name));
        item->setData(Qt::UserRole, i);
        item->setForeground(QBrush(QColor("#e879f9")));
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
