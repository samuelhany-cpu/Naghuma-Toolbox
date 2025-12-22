#include "CropTool.h"
#include <algorithm>

CropTool::CropTool(QObject *parent)
    : QObject(parent), isSelecting(false), selectionComplete(false) {
}

CropTool::~CropTool() {
}

void CropTool::startSelection(const QPoint& pos) {
    startPoint = pos;
    currentPoint = pos;
    cropRect = QRect();
    isSelecting = true;
    selectionComplete = false;
    updateCropRect();
}

void CropTool::updateSelection(const QPoint& pos) {
    if (!isSelecting) return;
    
    currentPoint = pos;
    updateCropRect();
    emit selectionChanged(cropRect);
}

void CropTool::finishSelection() {
    if (!isSelecting) return;
    
    isSelecting = false;
    selectionComplete = true;
    updateCropRect();
    
    if (isValidCrop()) {
        emit selectionFinished(cropRect);
    }
}

void CropTool::cancelSelection() {
    isSelecting = false;
    selectionComplete = false;
    cropRect = QRect();
    startPoint = QPoint();
    currentPoint = QPoint();
}

void CropTool::updateCropRect() {
    // Create rectangle from start and current points
    int x = std::min(startPoint.x(), currentPoint.x());
    int y = std::min(startPoint.y(), currentPoint.y());
    int width = std::abs(currentPoint.x() - startPoint.x());
    int height = std::abs(currentPoint.y() - startPoint.y());
    
    cropRect = QRect(x, y, width, height);
}

bool CropTool::isValidCrop() const {
    return cropRect.width() > 10 && cropRect.height() > 10;
}

QRect CropTool::getValidatedRect(const cv::Size& imageSize) const {
    if (!isValidCrop()) {
        return QRect();
    }
    
    // Ensure rectangle is within image bounds
    int x = std::max(0, cropRect.x());
    int y = std::max(0, cropRect.y());
    int width = std::min(cropRect.width(), imageSize.width - x);
    int height = std::min(cropRect.height(), imageSize.height - y);
    
    // Ensure minimum size
    if (width < 10 || height < 10) {
        return QRect();
    }
    
    return QRect(x, y, width, height);
}

cv::Mat CropTool::applyCrop(const cv::Mat& image) const {
    if (image.empty() || !isValidCrop()) {
        return cv::Mat();
    }
    
    QRect validRect = getValidatedRect(cv::Size(image.cols, image.rows));
    
    if (validRect.isEmpty()) {
        return cv::Mat();
    }
    
    // Create OpenCV Rect from QRect
    cv::Rect cvRect(validRect.x(), validRect.y(), 
                    validRect.width(), validRect.height());
    
    // Ensure rect is within image bounds
    cvRect.x = std::max(0, cvRect.x);
    cvRect.y = std::max(0, cvRect.y);
    cvRect.width = std::min(cvRect.width, image.cols - cvRect.x);
    cvRect.height = std::min(cvRect.height, image.rows - cvRect.y);
    
    if (cvRect.width <= 0 || cvRect.height <= 0) {
        return cv::Mat();
    }
    
    // Crop the image
    cv::Mat cropped = image(cvRect).clone();
    return cropped;
}

cv::Mat CropTool::getPreview(const cv::Mat& baseImage) const {
    if (baseImage.empty() || !hasSelection()) {
        return baseImage;
    }
    
    cv::Mat preview = baseImage.clone();
    
    if (!isValidCrop()) {
        return preview;
    }
    
    QRect validRect = getValidatedRect(cv::Size(baseImage.cols, baseImage.rows));
    
    if (validRect.isEmpty()) {
        return preview;
    }
    
    // Draw semi-transparent overlay outside crop area
    cv::Mat overlay = preview.clone();
    overlay *= 0.5; // Darken entire image
    
    // Copy original pixels in crop area
    cv::Rect cvRect(validRect.x(), validRect.y(), 
                    validRect.width(), validRect.height());
    
    // Ensure rect is valid
    cvRect.x = std::max(0, cvRect.x);
    cvRect.y = std::max(0, cvRect.y);
    cvRect.width = std::min(cvRect.width, baseImage.cols - cvRect.x);
    cvRect.height = std::min(cvRect.height, baseImage.rows - cvRect.y);
    
    if (cvRect.width > 0 && cvRect.height > 0) {
        baseImage(cvRect).copyTo(overlay(cvRect));
        
        // Draw rectangle border
        cv::rectangle(overlay, cvRect, cv::Scalar(232, 121, 249), 2);
        
        // Draw corner markers
        int cornerSize = 10;
        cv::Scalar cornerColor(192, 38, 211);
        
        // Top-left
        cv::line(overlay, 
                cv::Point(cvRect.x, cvRect.y), 
                cv::Point(cvRect.x + cornerSize, cvRect.y), 
                cornerColor, 3);
        cv::line(overlay, 
                cv::Point(cvRect.x, cvRect.y), 
                cv::Point(cvRect.x, cvRect.y + cornerSize), 
                cornerColor, 3);
        
        // Top-right
        cv::line(overlay, 
                cv::Point(cvRect.x + cvRect.width, cvRect.y), 
                cv::Point(cvRect.x + cvRect.width - cornerSize, cvRect.y), 
                cornerColor, 3);
        cv::line(overlay, 
                cv::Point(cvRect.x + cvRect.width, cvRect.y), 
                cv::Point(cvRect.x + cvRect.width, cvRect.y + cornerSize), 
                cornerColor, 3);
        
        // Bottom-left
        cv::line(overlay, 
                cv::Point(cvRect.x, cvRect.y + cvRect.height), 
                cv::Point(cvRect.x + cornerSize, cvRect.y + cvRect.height), 
                cornerColor, 3);
        cv::line(overlay, 
                cv::Point(cvRect.x, cvRect.y + cvRect.height), 
                cv::Point(cvRect.x, cvRect.y + cvRect.height - cornerSize), 
                cornerColor, 3);
        
        // Bottom-right
        cv::line(overlay, 
                cv::Point(cvRect.x + cvRect.width, cvRect.y + cvRect.height), 
                cv::Point(cvRect.x + cvRect.width - cornerSize, cvRect.y + cvRect.height), 
                cornerColor, 3);
        cv::line(overlay, 
                cv::Point(cvRect.x + cvRect.width, cvRect.y + cvRect.height), 
                cv::Point(cvRect.x + cvRect.width, cvRect.y + cvRect.height - cornerSize), 
                cornerColor, 3);
    }
    
    return overlay;
}

#include "moc_CropTool.cpp"
