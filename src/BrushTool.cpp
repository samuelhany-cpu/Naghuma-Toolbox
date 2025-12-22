#include "BrushTool.h"
#include <algorithm>

BrushTool::BrushTool(QObject* parent)
    : QObject(parent)
    , brushSize(20)
    , eraseMode(false)
    , isDrawing(false)
{
}

void BrushTool::startStroke(const QPoint& pos, const cv::Size& imageSize) {
    // Initialize mask if needed
    if (mask.empty() || mask.size() != imageSize) {
        mask = cv::Mat::zeros(imageSize, CV_8UC1);
    }
    
    // Validate coordinates are within bounds
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= imageSize.width || pos.y() >= imageSize.height) {
        return;
    }
    
    isDrawing = true;
    lastPoint = pos;
    
    // Draw initial point
    cv::circle(mask, cv::Point(pos.x(), pos.y()), brushSize, 
               eraseMode ? cv::Scalar(0) : cv::Scalar(255), -1);
    
    emit maskUpdated();
}

void BrushTool::continueStroke(const QPoint& pos) {
    if (!isDrawing || mask.empty()) return;
    
    // Validate coordinates
    if (pos.x() < 0 || pos.y() < 0 || pos.x() >= mask.cols || pos.y() >= mask.rows) {
        return;
    }
    
    // Draw line from last point to current point
    drawBrushStroke(lastPoint, pos);
    lastPoint = pos;
    
    emit maskUpdated();
}

void BrushTool::endStroke() {
    isDrawing = false;
}

void BrushTool::drawBrushStroke(const QPoint& from, const QPoint& to) {
    if (mask.empty()) return;
    
    // Clamp points to image bounds
    int x1 = std::max(0, std::min(from.x(), mask.cols - 1));
    int y1 = std::max(0, std::min(from.y(), mask.rows - 1));
    int x2 = std::max(0, std::min(to.x(), mask.cols - 1));
    int y2 = std::max(0, std::min(to.y(), mask.rows - 1));
    
    cv::Point p1(x1, y1);
    cv::Point p2(x2, y2);
    
    // Draw thick line with rounded ends
    cv::line(mask, p1, p2, eraseMode ? cv::Scalar(0) : cv::Scalar(255), 
             brushSize * 2, cv::LINE_AA);
    
    // Draw circles at endpoints for smooth stroke
    cv::circle(mask, p1, brushSize, eraseMode ? cv::Scalar(0) : cv::Scalar(255), -1);
    cv::circle(mask, p2, brushSize, eraseMode ? cv::Scalar(0) : cv::Scalar(255), -1);
}

void BrushTool::clearMask() {
    if (!mask.empty()) {
        mask = cv::Mat::zeros(mask.size(), CV_8UC1);
        emit maskUpdated();
    }
}

cv::Mat BrushTool::applyMaskToResult(const cv::Mat& original, const cv::Mat& processed) const {
    if (mask.empty() || original.empty() || processed.empty()) {
        return processed.clone();
    }
    
    // Ensure sizes match
    if (mask.size() != original.size() || original.size() != processed.size()) {
        return processed.clone();
    }
    
    cv::Mat result = original.clone();
    
    // Convert mask to 3-channel if needed
    cv::Mat mask3ch;
    if (original.channels() == 3) {
        cv::cvtColor(mask, mask3ch, cv::COLOR_GRAY2BGR);
    } else {
        mask3ch = mask;
    }
    
    // Copy processed pixels only where mask is white (255)
    processed.copyTo(result, mask);
    
    return result;
}

cv::Mat BrushTool::getMaskOverlay(const cv::Mat& image, const cv::Scalar& color) const {
    if (mask.empty() || image.empty()) {
        return image.clone();
    }
    
    // Resize mask if sizes don't match
    cv::Mat resizedMask = mask;
    if (mask.size() != image.size()) {
        cv::resize(mask, resizedMask, image.size(), 0, 0, cv::INTER_NEAREST);
    }
    
    cv::Mat overlay = image.clone();
    
    // Ensure image is 3-channel
    if (overlay.channels() == 1) {
        cv::cvtColor(overlay, overlay, cv::COLOR_GRAY2BGR);
    }
    
    // Create colored overlay where mask is active
    cv::Mat colorMask = cv::Mat::zeros(resizedMask.size(), CV_8UC3);
    colorMask.setTo(color, resizedMask);
    
    // Blend overlay with original (50% opacity for better visibility)
    cv::addWeighted(overlay, 0.5, colorMask, 0.5, 0, overlay);
    
    return overlay;
}
