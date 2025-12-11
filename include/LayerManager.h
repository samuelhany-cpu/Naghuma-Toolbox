#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <functional>
#include <opencv2/opencv.hpp>

struct ProcessingLayer {
    QString name;
    QString type;  // "filter", "transform", "adjustment"
    cv::Mat image;
    bool visible;
    std::function<cv::Mat(const cv::Mat&)> operation;  // Store the operation function
};

class LayerManager : public QObject {
    Q_OBJECT

public:
    explicit LayerManager(QObject *parent = nullptr);
    ~LayerManager();

    void addLayer(const QString& name, const QString& type, const cv::Mat& image,
                  std::function<cv::Mat(const cv::Mat&)> operation = nullptr);
    void removeLayer(int index);
    void clearLayers();

    const QVector<ProcessingLayer>& getLayers() const { return layers; }
    int getLayerCount() const { return layers.size(); }

    ProcessingLayer getLayer(int index) const;
    bool hasLayers() const { return !layers.isEmpty(); }

    // Rebuild image from operations
    cv::Mat rebuildFromLayers(const cv::Mat& original, int upToLayer = -1) const;

signals:
    void layersChanged();
    void layerAdded(const QString& name);
    void layerRemoved(int index);

private:
    QVector<ProcessingLayer> layers;
};

#endif // LAYERMANAGER_H
