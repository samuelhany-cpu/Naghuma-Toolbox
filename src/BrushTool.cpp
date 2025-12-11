#include "BrushTool.h"
#include <cmath>

BrushTool::BrushTool(QObject *parent)
    : QObject(parent),
      brushType(BrushType::Circle),
      brushSize(10),
      brushOpacity(100),
      brushColor(Qt::black),
      eraserMode(false),
      isDrawing(false) {
}

BrushTool::~BrushTool() {
}

void BrushTool::startDrawing(const QPoint& pos) {
    isDrawing = true;
    lastPoint = pos;
    strokePoints.clear();
    strokePoints.push_back(pos);
}

void BrushTool::continueDrawing(const QPoint& pos) {
    if (!isDrawing) return;
    
    strokePoints.push_back(pos);
    lastPoint = pos;
    emit strokeUpdated();
}

void BrushTool::finishDrawing() {
    isDrawing = false;
}

void BrushTool::applyToImage(cv::Mat& image) {
    if (strokePoints.empty() || strokeCanvas.empty()) return;
    
    // Blend the stroke onto the image
    for (size_t i = 1; i < strokePoints.size(); ++i) {
        drawBrushStroke(image, strokePoints[i-1], strokePoints[i]);
    }
}

void BrushTool::clearStroke() {
    strokePoints.clear();
    strokeCanvas.release();
}

cv::Mat BrushTool::getStrokePreview(const cv::Mat& baseImage) const {
    if (strokePoints.empty()) return baseImage;
    
    cv::Mat preview = baseImage.clone();
    
    // Draw all stroke points
    for (size_t i = 1; i < strokePoints.size(); ++i) {
        const_cast<BrushTool*>(this)->drawBrushStroke(preview, strokePoints[i-1], strokePoints[i]);
    }
    
    return preview;
}

void BrushTool::drawBrushStroke(cv::Mat& canvas, const QPoint& from, const QPoint& to) {
    // Interpolate points between from and to for smooth drawing
    int dx = to.x() - from.x();
    int dy = to.y() - from.y();
    float dist = std::sqrt(dx*dx + dy*dy);
    
    if (dist < 1.0f) {
        drawCircleBrush(canvas, cv::Point(to.x(), to.y()));
        return;
    }
    
    int steps = static_cast<int>(dist);
    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        int x = from.x() + static_cast<int>(t * dx);
        int y = from.y() + static_cast<int>(t * dy);
        
        cv::Point pt(x, y);
        
        switch (brushType) {
            case BrushType::Circle:
                drawCircleBrush(canvas, pt);
                break;
            case BrushType::Square:
                drawSquareBrush(canvas, pt);
                break;
            case BrushType::SoftEdge:
                drawSoftBrush(canvas, pt);
                break;
        }
    }
}

void BrushTool::drawCircleBrush(cv::Mat& canvas, const cv::Point& center) {
    if (canvas.empty()) return;
    
    cv::Scalar color;
    if (eraserMode) {
        color = cv::Scalar(255, 255, 255); // White for eraser
    } else {
        color = cv::Scalar(brushColor.blue(), brushColor.green(), brushColor.red());
    }
    
    // Draw circle with opacity
    cv::Mat overlay = canvas.clone();
    cv::circle(overlay, center, brushSize / 2, color, -1);
    
    double alpha = brushOpacity / 100.0;
    cv::addWeighted(overlay, alpha, canvas, 1.0 - alpha, 0, canvas);
}

void BrushTool::drawSquareBrush(cv::Mat& canvas, const cv::Point& center) {
    if (canvas.empty()) return;
    
    cv::Scalar color;
    if (eraserMode) {
        color = cv::Scalar(255, 255, 255);
    } else {
        color = cv::Scalar(brushColor.blue(), brushColor.green(), brushColor.red());
    }
    
    int halfSize = brushSize / 2;
    cv::Point topLeft(center.x - halfSize, center.y - halfSize);
    cv::Point bottomRight(center.x + halfSize, center.y + halfSize);
    
    // Ensure points are within bounds
    topLeft.x = std::max(0, topLeft.x);
    topLeft.y = std::max(0, topLeft.y);
    bottomRight.x = std::min(canvas.cols - 1, bottomRight.x);
    bottomRight.y = std::min(canvas.rows - 1, bottomRight.y);
    
    cv::Mat overlay = canvas.clone();
    cv::rectangle(overlay, topLeft, bottomRight, color, -1);
    
    double alpha = brushOpacity / 100.0;
    cv::addWeighted(overlay, alpha, canvas, 1.0 - alpha, 0, canvas);
}

void BrushTool::drawSoftBrush(cv::Mat& canvas, const cv::Point& center) {
    if (canvas.empty()) return;
    
    cv::Scalar color;
    if (eraserMode) {
        color = cv::Scalar(255, 255, 255);
    } else {
        color = cv::Scalar(brushColor.blue(), brushColor.green(), brushColor.red());
    }
    
    // Create a soft brush with gradient
    int radius = brushSize / 2;
    cv::Mat overlay = canvas.clone();
    
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            float dist = std::sqrt(x*x + y*y);
            if (dist <= radius) {
                int px = center.x + x;
                int py = center.y + y;
                
                if (px >= 0 && px < canvas.cols && py >= 0 && py < canvas.rows) {
                    // Soft falloff from center to edge
                    float softness = 1.0f - (dist / radius);
                    softness = softness * softness; // Quadratic falloff
                    
                    double alpha = (brushOpacity / 100.0) * softness;
                    
                    cv::Vec3b& pixel = overlay.at<cv::Vec3b>(py, px);
                    pixel[0] = static_cast<uchar>(pixel[0] * (1 - alpha) + color[0] * alpha);
                    pixel[1] = static_cast<uchar>(pixel[1] * (1 - alpha) + color[1] * alpha);
                    pixel[2] = static_cast<uchar>(pixel[2] * (1 - alpha) + color[2] * alpha);
                }
            }
        }
    }
    
    canvas = overlay;
}
