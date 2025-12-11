#include "ImageCanvas.h"
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <algorithm>

ImageCanvas::ImageCanvas(QWidget *parent, const QString& borderColor)
    : QWidget(parent), borderColor(borderColor), mouseEventsEnabled(false) {
    
    setMinimumSize(400, 300);
    setStyleSheet(QString("background-color: #1a1625; "
                         "border: 2px solid %1; "
                         "border-radius: 8px;").arg(borderColor));
    
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border: none; background: transparent;");
    
    setMouseTracking(true);
}

void ImageCanvas::setImage(const QPixmap& pixmap) {
    currentPixmap = pixmap;
    updateScaledPixmap();
}

void ImageCanvas::setImage(const cv::Mat& mat) {
    if (mat.empty()) return;
    
    currentImage = mat.clone();
    
    // Convert cv::Mat to QPixmap
    cv::Mat rgb;
    if (mat.channels() == 1) {
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
    } else if (mat.channels() == 3) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    } else if (mat.channels() == 4) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGRA2RGBA);
    } else {
        rgb = mat.clone();
    }
    
    QImage qImg(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), 
                QImage::Format_RGB888);
    currentPixmap = QPixmap::fromImage(qImg.copy());
    updateScaledPixmap();
}

void ImageCanvas::clear() {
    currentPixmap = QPixmap();
    imageLabel->clear();
    update();
}

void ImageCanvas::updateScaledPixmap() {
    if (currentPixmap.isNull()) return;
    
    QSize canvasSize = size() - QSize(20, 20); // Padding
    scaledPixmap = currentPixmap.scaled(canvasSize, 
                                       Qt::KeepAspectRatio, 
                                       Qt::SmoothTransformation);
    
    imageLabel->setPixmap(scaledPixmap);
    imageLabel->adjustSize();
    
    // Center the label
    int x = (width() - imageLabel->width()) / 2;
    int y = (height() - imageLabel->height()) / 2;
    imageLabel->move(x, y);
}

void ImageCanvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateScaledPixmap();
}

void ImageCanvas::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void ImageCanvas::mousePressEvent(QMouseEvent *event) {
    if (!mouseEventsEnabled || currentPixmap.isNull()) return;
    
    QPoint imagePos = mapToImageCoords(event->pos());
    if (imagePos.x() >= 0 && imagePos.y() >= 0) {
        emit mousePressed(imagePos);
    }
}

void ImageCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (!mouseEventsEnabled || currentPixmap.isNull()) return;
    
    if (event->buttons() & Qt::LeftButton) {
        QPoint imagePos = mapToImageCoords(event->pos());
        if (imagePos.x() >= 0 && imagePos.y() >= 0) {
            emit mouseMoved(imagePos);
        }
    }
}

void ImageCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (!mouseEventsEnabled || currentPixmap.isNull()) return;
    
    QPoint imagePos = mapToImageCoords(event->pos());
    if (imagePos.x() >= 0 && imagePos.y() >= 0) {
        emit mouseReleased(imagePos);
    }
}

QPoint ImageCanvas::mapToImageCoords(const QPoint& widgetPos) {
    if (currentPixmap.isNull() || scaledPixmap.isNull()) {
        return QPoint(-1, -1);
    }
    
    // Get the position relative to the image label
    QPoint labelPos = widgetPos - imageLabel->pos();
    
    // Check if point is within the image label
    if (labelPos.x() < 0 || labelPos.y() < 0 || 
        labelPos.x() >= scaledPixmap.width() || 
        labelPos.y() >= scaledPixmap.height()) {
        return QPoint(-1, -1);
    }
    
    // Scale to original image coordinates
    float scaleX = float(currentPixmap.width()) / float(scaledPixmap.width());
    float scaleY = float(currentPixmap.height()) / float(scaledPixmap.height());
    
    int imageX = int(labelPos.x() * scaleX);
    int imageY = int(labelPos.y() * scaleY);
    
    // Clamp to image bounds
    imageX = std::max(0, std::min(imageX, currentPixmap.width() - 1));
    imageY = std::max(0, std::min(imageY, currentPixmap.height() - 1));
    
    return QPoint(imageX, imageY);
}
