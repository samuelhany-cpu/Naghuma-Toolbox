#ifndef IMAGECANVAS_H
#define IMAGECANVAS_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
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
    
signals:
    void mousePressed(const QPoint& pos);
    void mouseMoved(const QPoint& pos);
    void mouseReleased(const QPoint& pos);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    
private:
    void updateScaledPixmap();
    QPoint mapToImageCoords(const QPoint& widgetPos);
    
    QLabel *imageLabel;
    QPixmap currentPixmap;
    QPixmap scaledPixmap;
    cv::Mat currentImage;
    QString borderColor;
    bool mouseEventsEnabled;
};

#endif // IMAGECANVAS_H
