#ifndef RIGHTSIDEBARWIDGET_H
#define RIGHTSIDEBARWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <opencv2/opencv.hpp>

class HistogramWidget;
class LayerManager;

class RightSidebarWidget : public QWidget {
    Q_OBJECT

public:
    explicit RightSidebarWidget(QWidget *parent = nullptr);
    ~RightSidebarWidget();

    void updateHistogram(const cv::Mat& image);
    void addLayer(const QString& name, const QString& type, const cv::Mat& image,
                  std::function<cv::Mat(const cv::Mat&)> operation = nullptr);
    void clearLayers();
    void resetHistogram();
    void removeLayer(int layerIndex);
    int getLayerCount() const;
    cv::Mat getLayerImage(int layerIndex) const;
    cv::Mat rebuildImage(const cv::Mat& original, int upToLayer = -1) const;

signals:
    void layerRemoveRequested(int layerIndex);
    void layersRemoveRequested(const QList<int>& layerIndices);

private slots:
    void onLayerItemClicked(QListWidgetItem* item);
    void onLayerItemChanged(QListWidgetItem* item);
    void onRemoveLayerClicked();
    void onSelectAllClicked();
    void onDeselectAllClicked();

private:
    void setupUI();
    void createHistogramTab();
    void createLayersTab();
    void updateLayersList();
    void updateRemoveButtonState();

    // UI Components
    QTabWidget *tabWidget;
    HistogramWidget *histogramWidget;
    QListWidget *layersListWidget;
    QPushButton *removeLayerButton;
    QPushButton *selectAllButton;
    QPushButton *deselectAllButton;
    
    // Data
    LayerManager *layerManager;
};

#endif // RIGHTSIDEBARWIDGET_H
