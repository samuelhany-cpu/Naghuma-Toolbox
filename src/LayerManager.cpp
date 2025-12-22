#include "LayerManager.h"

LayerManager::LayerManager(QObject *parent)
    : QObject(parent) {
}

LayerManager::~LayerManager() {
}

void LayerManager::addLayer(const QString& name, const QString& type, const cv::Mat& image,
                            std::function<cv::Mat(const cv::Mat&)> operation) {
    ProcessingLayer layer;
    layer.name = name;
    layer.type = type;
    layer.image = image.clone();
    layer.visible = true;
    layer.operation = operation;
    
    layers.append(layer);
    
    emit layerAdded(name);
    emit layersChanged();
}

void LayerManager::removeLayer(int index) {
    if (index >= 0 && index < layers.size()) {
        layers.removeAt(index);
        emit layerRemoved(index);
        emit layersChanged();
    }
}

void LayerManager::clearLayers() {
    layers.clear();
    emit layersChanged();
}

ProcessingLayer LayerManager::getLayer(int index) const {
    if (index >= 0 && index < layers.size()) {
        return layers[index];
    }
    return ProcessingLayer();
}

cv::Mat LayerManager::rebuildFromLayers(const cv::Mat& original, int upToLayer) const {
    if (layers.isEmpty() || original.empty()) {
        return original.clone();
    }
    
    cv::Mat result = original.clone();
    int endLayer = (upToLayer < 0) ? layers.size() : (upToLayer + 1 < layers.size() ? upToLayer + 1 : layers.size());
    
    for (int i = 0; i < endLayer; ++i) {
        if (layers[i].operation) {
            // Use the operation function to replay the transformation
            try {
                result = layers[i].operation(result);
            } catch (...) {
                // If operation fails (e.g., due to dimension mismatch), fall back to stored image
                result = layers[i].image.clone();
            }
        } else {
            // Fallback: use stored image if operation not available
            // This handles operations that can't be replayed (e.g., after dimension changes)
            result = layers[i].image.clone();
        }
        
        // If result is empty after operation, stop rebuilding
        if (result.empty()) {
            break;
        }
    }
    
    return result;
}

#include "moc_LayerManager.cpp"
