#ifndef IMAGECANVAS_H
#define IMAGECANVAS_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QScrollArea>
#include <opencv2/opencv.hpp>

class ImageCanvas : public QWidget {
    Q_OBJECT

public:
    explicit ImageCanvas(QWidget *parent = nullptr, const QString& borderColor = "#ff6b9d");
    
    void setImage(const QPixmap& pixmap);
    void setImage(const cv::Mat& mat);
    void clear();
    QPixmap getPixmap() const { return currentPixmap; }
    cv::Mat getCurrentImage() const { return currentImage; }
    
    void setMouseEventsEnabled(bool enabled) { mouseEventsEnabled = enabled; }
    
    // Zoom functionality
    void zoomIn();
    void zoomOut();
    void setZoomLevel(double level);
    void fitToWindow();
    void actualSize();
    double getZoomLevel() const { return zoomLevel; }
    int getZoomPercent() const { return static_cast<int>(zoomLevel * 100); }
    
signals:
    void mousePressed(const QPoint& pos);
    void mouseMoved(const QPoint& pos);
    void mouseReleased(const QPoint& pos);
    void zoomChanged(double level);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    
private:
    void updateScaledPixmap();
    void updateZoomedPixmap();
    QPoint mapToImageCoords(const QPoint& widgetPos);
    double calculateFitToWindowZoom();
    
    QLabel *imageLabel;
    QPixmap currentPixmap;
    QPixmap scaledPixmap;
    cv::Mat currentImage;
    QString borderColor;
    bool mouseEventsEnabled;
    
    // Zoom and pan
    double zoomLevel;           // 0.25 to 4.0
    bool fitToWindowMode;
    QPoint panOffset;
    bool isPanning;
    QPoint lastPanPoint;
    
    // Zoom level steps
    static constexpr double ZOOM_LEVELS[] = {0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 4.0};
    static constexpr int ZOOM_LEVELS_COUNT = 8;
    int currentZoomIndex;
};

#endif // IMAGECANVAS_H
