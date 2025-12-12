#include "ImageCanvas.h"
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <algorithm>
#include <cmath>

constexpr double ImageCanvas::ZOOM_LEVELS[];

ImageCanvas::ImageCanvas(QWidget *parent, const QString& borderColor)
    : QWidget(parent), 
      borderColor(borderColor), 
      mouseEventsEnabled(false),
      zoomLevel(1.0),
      fitToWindowMode(true),
      panOffset(0, 0),
      isPanning(false),
      currentZoomIndex(3) {  // Index 3 = 1.0 (100%)
    
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
    
    if (fitToWindowMode) {
        // Auto-fit mode
        QSize canvasSize = size() - QSize(20, 20); // Padding
        scaledPixmap = currentPixmap.scaled(canvasSize, 
                                           Qt::KeepAspectRatio, 
                                           Qt::SmoothTransformation);
    } else {
        // Manual zoom mode
        updateZoomedPixmap();
        return;
    }
    
    imageLabel->setPixmap(scaledPixmap);
    imageLabel->adjustSize();
    
    // Center the label
    int x = (width() - imageLabel->width()) / 2;
    int y = (height() - imageLabel->height()) / 2;
    imageLabel->move(x, y);
}

void ImageCanvas::updateZoomedPixmap() {
    if (currentPixmap.isNull()) return;
    
    // Calculate zoomed size
    QSize zoomedSize = currentPixmap.size() * zoomLevel;
    
    scaledPixmap = currentPixmap.scaled(zoomedSize, 
                                       Qt::KeepAspectRatio, 
                                       Qt::SmoothTransformation);
    
    imageLabel->setPixmap(scaledPixmap);
    imageLabel->adjustSize();
    
    // Apply pan offset
    int x = panOffset.x() + (width() - imageLabel->width()) / 2;
    int y = panOffset.y() + (height() - imageLabel->height()) / 2;
    imageLabel->move(x, y);
}

// Zoom functionality implementation
void ImageCanvas::zoomIn() {
    if (currentZoomIndex < ZOOM_LEVELS_COUNT - 1) {
        currentZoomIndex++;
        setZoomLevel(ZOOM_LEVELS[currentZoomIndex]);
    }
}

void ImageCanvas::zoomOut() {
    if (currentZoomIndex > 0) {
        currentZoomIndex--;
        setZoomLevel(ZOOM_LEVELS[currentZoomIndex]);
    }
}

void ImageCanvas::setZoomLevel(double level) {
    if (currentPixmap.isNull()) return;
    
    // Clamp zoom level
    level = std::max(0.25, std::min(4.0, level));
    
    if (std::abs(zoomLevel - level) < 0.01) return; // No change
    
    zoomLevel = level;
    fitToWindowMode = false;
    
    // Find closest zoom index
    double minDiff = std::abs(ZOOM_LEVELS[0] - zoomLevel);
    currentZoomIndex = 0;
    for (int i = 1; i < ZOOM_LEVELS_COUNT; i++) {
        double diff = std::abs(ZOOM_LEVELS[i] - zoomLevel);
        if (diff < minDiff) {
            minDiff = diff;
            currentZoomIndex = i;
        }
    }
    
    updateZoomedPixmap();
    emit zoomChanged(zoomLevel);
}

void ImageCanvas::fitToWindow() {
    if (currentPixmap.isNull()) return;
    
    fitToWindowMode = true;
    zoomLevel = calculateFitToWindowZoom();
    panOffset = QPoint(0, 0);
    
    updateScaledPixmap();
    emit zoomChanged(zoomLevel);
}

void ImageCanvas::actualSize() {
    setZoomLevel(1.0);
    currentZoomIndex = 3; // 100%
}

double ImageCanvas::calculateFitToWindowZoom() {
    if (currentPixmap.isNull()) return 1.0;
    
    QSize canvasSize = size() - QSize(20, 20);
    double widthRatio = double(canvasSize.width()) / double(currentPixmap.width());
    double heightRatio = double(canvasSize.height()) / double(currentPixmap.height());
    
    return std::min(widthRatio, heightRatio);
}

void ImageCanvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateScaledPixmap();
}

void ImageCanvas::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void ImageCanvas::mousePressEvent(QMouseEvent *event) {
    if (currentPixmap.isNull()) return;
    
    // Middle button or Ctrl+Left = Pan
    if (event->button() == Qt::MiddleButton || 
        (event->button() == Qt::LeftButton && (event->modifiers() & Qt::ControlModifier))) {
        isPanning = true;
        lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    
    // Regular mouse events for tools
    if (mouseEventsEnabled && event->button() == Qt::LeftButton) {
        QPoint imagePos = mapToImageCoords(event->pos());
        if (imagePos.x() >= 0 && imagePos.y() >= 0) {
            emit mousePressed(imagePos);
        }
    }
}

void ImageCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (currentPixmap.isNull()) return;
    
    // Handle panning
    if (isPanning) {
        QPoint delta = event->pos() - lastPanPoint;
        panOffset += delta;
        lastPanPoint = event->pos();
        
        if (!fitToWindowMode) {
            updateZoomedPixmap();
        }
        event->accept();
        return;
    }
    
    // Regular mouse events for tools
    if (mouseEventsEnabled && (event->buttons() & Qt::LeftButton)) {
        QPoint imagePos = mapToImageCoords(event->pos());
        if (imagePos.x() >= 0 && imagePos.y() >= 0) {
            emit mouseMoved(imagePos);
        }
    }
}

void ImageCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (currentPixmap.isNull()) return;
    
    // End panning
    if (isPanning && (event->button() == Qt::MiddleButton || event->button() == Qt::LeftButton)) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    
    // Regular mouse events for tools
    if (mouseEventsEnabled && event->button() == Qt::LeftButton) {
        QPoint imagePos = mapToImageCoords(event->pos());
        if (imagePos.x() >= 0 && imagePos.y() >= 0) {
            emit mouseReleased(imagePos);
        }
    }
}

void ImageCanvas::wheelEvent(QWheelEvent *event) {
    if (currentPixmap.isNull()) {
        QWidget::wheelEvent(event);
        return;
    }
    
    // Ctrl + Wheel = Zoom
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
        event->accept();
    } else {
        QWidget::wheelEvent(event);
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
