#include "SelectionTool.h"
#include <algorithm>
#include <QDebug>

SelectionTool::SelectionTool(QObject* parent)
    : QObject(parent)
    , currentMode(SelectionMode::Rectangle)
    , isSelectingRect(false)
    , magicWandTolerance(30)
{
}

QString SelectionTool::getModeName() const {
    switch (currentMode) {
        case SelectionMode::Rectangle: return "Rectangle";
        case SelectionMode::Polygon: return "Polygon";
        case SelectionMode::MagicWand: return "Magic Wand";
        case SelectionMode::Threshold: return "Threshold";
        case SelectionMode::EdgeBased: return "Edge-Based";
        default: return "Unknown";
    }
}

// ==================== RECTANGLE SELECTION ====================

void SelectionTool::startRectangle(const QPoint& pos) {
    rectStart = pos;
    rectEnd = pos;
    isSelectingRect = true;
    emit selectionUpdated();
}

void SelectionTool::updateRectangle(const QPoint& pos) {
    if (!isSelectingRect) return;
    rectEnd = pos;
    emit selectionUpdated();
}

void SelectionTool::endRectangle(const cv::Size& imageSize) {
    isSelectingRect = false;
    
    // Create mask with proper image size
    if (rectStart != rectEnd && imageSize.width > 0 && imageSize.height > 0) {
        mask = cv::Mat::zeros(imageSize, CV_8UC1);
        
        int x1 = std::max(0, std::min(rectStart.x(), rectEnd.x()));
        int y1 = std::max(0, std::min(rectStart.y(), rectEnd.y()));
        int x2 = std::min(imageSize.width - 1, std::max(rectStart.x(), rectEnd.x()));
        int y2 = std::min(imageSize.height - 1, std::max(rectStart.y(), rectEnd.y()));
        
        if (x2 > x1 && y2 > y1) {
            cv::Rect rect(x1, y1, x2 - x1, y2 - y1);
            mask(rect).setTo(255);
            qDebug() << "Rectangle: mask non-zero pixels =" << cv::countNonZero(mask) 
                     << "rect:" << x1 << y1 << x2 << y2;
        }
    }
    
    emit selectionCompleted();
}

void SelectionTool::createRectangleMask(const cv::Size& imageSize) {
    mask = cv::Mat::zeros(imageSize, CV_8UC1);
    
    int x1 = std::max(0, std::min(rectStart.x(), rectEnd.x()));
    int y1 = std::max(0, std::min(rectStart.y(), rectEnd.y()));
    int x2 = std::min(imageSize.width - 1, std::max(rectStart.x(), rectEnd.x()));
    int y2 = std::min(imageSize.height - 1, std::max(rectStart.y(), rectEnd.y()));
    
    cv::Rect rect(x1, y1, x2 - x1, y2 - y1);
    mask(rect).setTo(255);
}

// ==================== POLYGON SELECTION ====================

void SelectionTool::addPolygonPoint(const QPoint& pos) {
    polygonPoints.push_back(pos);
    emit selectionUpdated();
}

void SelectionTool::closePolygon(const cv::Size& imageSize) {
    if (polygonPoints.size() < 3) {
        clearPolygon();
        return;
    }
    
    // Create mask with proper image size
    if (imageSize.width > 0 && imageSize.height > 0) {
        mask = cv::Mat::zeros(imageSize, CV_8UC1);
        
        // Convert QPoints to cv::Points
        std::vector<cv::Point> cvPoints;
        for (const QPoint& p : polygonPoints) {
            cvPoints.push_back(cv::Point(p.x(), p.y()));
        }
        
        // Fill polygon
        std::vector<std::vector<cv::Point>> polygons = {cvPoints};
        cv::fillPoly(mask, polygons, cv::Scalar(255));
        
        qDebug() << "Polygon: mask non-zero pixels =" << cv::countNonZero(mask);
    }
    
    emit selectionCompleted();
}

void SelectionTool::clearPolygon() {
    polygonPoints.clear();
    emit selectionUpdated();
}

void SelectionTool::createPolygonMask(const cv::Size& imageSize) {
    if (polygonPoints.size() < 3) return;
    
    mask = cv::Mat::zeros(imageSize, CV_8UC1);
    
    // Convert QPoints to cv::Point
    std::vector<cv::Point> cvPoints;
    for (const QPoint& p : polygonPoints) {
        cvPoints.push_back(cv::Point(p.x(), p.y()));
    }
    
    // Fill polygon
    std::vector<std::vector<cv::Point>> polygons = {cvPoints};
    cv::fillPoly(mask, polygons, cv::Scalar(255));
}

// ==================== MAGIC WAND (COLOR-BASED) ====================

void SelectionTool::magicWandSelect(const cv::Mat& image, const QPoint& seedPoint, int tolerance) {
    if (image.empty()) return;
    
    magicWandTolerance = tolerance;
    cv::Point seed(seedPoint.x(), seedPoint.y());
    
    // Validate seed point
    if (seed.x < 0 || seed.y < 0 || seed.x >= image.cols || seed.y >= image.rows) {
        return;
    }
    
    floodFillSelection(image, seed, tolerance);
    emit selectionCompleted();
}

void SelectionTool::floodFillSelection(const cv::Mat& image, const cv::Point& seed, int tolerance) {
    mask = cv::Mat::zeros(image.size(), CV_8UC1);
    
    // For grayscale images, use simpler approach
    if (image.channels() == 1) {
        cv::Mat floodMask = cv::Mat::zeros(image.rows + 2, image.cols + 2, CV_8UC1);
        cv::Mat imageCopy = image.clone();
        
        int flags = 4 | (255 << 8) | cv::FLOODFILL_MASK_ONLY;
        cv::Scalar loDiff(tolerance);
        cv::Scalar upDiff(tolerance);
        
        cv::floodFill(imageCopy, floodMask, seed, cv::Scalar(255), nullptr, loDiff, upDiff, flags);
        
        // Extract the mask (remove border)
        mask = floodMask(cv::Rect(1, 1, image.cols, image.rows)).clone();
        
        qDebug() << "Magic Wand grayscale: mask non-zero pixels =" << cv::countNonZero(mask);
    } else {
        // For color images (shouldn't happen with grayscale requirement)
        cv::Mat flooded = image.clone();
        cv::Mat floodMask = cv::Mat::zeros(image.rows + 2, image.cols + 2, CV_8UC1);
        
        int flags = 4 | (255 << 8) | cv::FLOODFILL_MASK_ONLY;
        cv::Scalar loDiff(tolerance, tolerance, tolerance);
        cv::Scalar upDiff(tolerance, tolerance, tolerance);
        
        cv::floodFill(flooded, floodMask, seed, cv::Scalar(255), nullptr, loDiff, upDiff, flags);
        
        // Extract the mask (remove border)
        mask = floodMask(cv::Rect(1, 1, image.cols, image.rows)).clone();
        
        qDebug() << "Magic Wand color: mask non-zero pixels =" << cv::countNonZero(mask);
    }
}

// ==================== THRESHOLD-BASED SELECTION ====================

void SelectionTool::thresholdSelect(const cv::Mat& image, int minThreshold, int maxThreshold) {
    if (image.empty()) return;
    
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }
    
    // Create mask for pixels in range
    mask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::inRange(gray, minThreshold, maxThreshold, mask);
    
    qDebug() << "Threshold: mask non-zero pixels =" << cv::countNonZero(mask);
    
    emit selectionCompleted();
}

// ==================== EDGE-BASED SELECTION ====================

void SelectionTool::edgeBasedSelect(const cv::Mat& image, const QPoint& seedPoint) {
    if (image.empty()) return;
    
    cv::Point seed(seedPoint.x(), seedPoint.y());
    if (seed.x < 0 || seed.y < 0 || seed.x >= image.cols || seed.y >= image.rows) {
        return;
    }
    
    // Detect edges
    cv::Mat gray, edges;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }
    
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 1.5);
    cv::Canny(gray, edges, 50, 150);
    
    // Dilate edges to close gaps
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(edges, edges, kernel);
    
    // Invert edges (we want to flood-fill the areas, not the edges)
    cv::Mat invEdges = 255 - edges;
    
    // Flood fill from seed point
    mask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::Mat floodMask = cv::Mat::zeros(image.rows + 2, image.cols + 2, CV_8UC1);
    
    // Copy inverted edges to floodMask interior
    invEdges.copyTo(floodMask(cv::Rect(1, 1, image.cols, image.rows)));
    
    int flags = 4 | (255 << 8) | cv::FLOODFILL_MASK_ONLY;
    cv::floodFill(invEdges, floodMask, seed, cv::Scalar(255), nullptr, 
                  cv::Scalar(10), cv::Scalar(10), flags);
    
    // Extract mask
    mask = floodMask(cv::Rect(1, 1, image.cols, image.rows)).clone();
    
    qDebug() << "Edge-Based: mask non-zero pixels =" << cv::countNonZero(mask);
    
    emit selectionCompleted();
}

// ==================== MASK OPERATIONS ====================

cv::Mat SelectionTool::getMask(const cv::Size& imageSize) const {
    // Generate mask based on current mode
    cv::Mat resultMask;
    
    switch (currentMode) {
        case SelectionMode::Rectangle:
            if (isSelectingRect || (rectStart != rectEnd)) {
                const_cast<SelectionTool*>(this)->createRectangleMask(imageSize);
            }
            break;
            
        case SelectionMode::Polygon:
            if (polygonPoints.size() >= 3) {
                const_cast<SelectionTool*>(this)->createPolygonMask(imageSize);
            }
            break;
            
        default:
            // Other modes generate mask directly
            break;
    }
    
    return mask.clone();
}

void SelectionTool::clearMask() {
    mask = cv::Mat();
    rectStart = QPoint();
    rectEnd = QPoint();
    polygonPoints.clear();
    isSelectingRect = false;
    emit selectionUpdated();
}

cv::Mat SelectionTool::applyMaskToResult(const cv::Mat& original, const cv::Mat& processed) const {
    if (mask.empty() || original.empty() || processed.empty()) {
        return processed.clone();
    }
    
    if (mask.size() != original.size() || original.size() != processed.size()) {
        return processed.clone();
    }
    
    cv::Mat result = original.clone();
    processed.copyTo(result, mask);
    
    return result;
}

// ==================== LAYER INTEGRATION ====================

cv::Mat SelectionTool::getMaskAsLayer() const {
    if (mask.empty()) return cv::Mat();
    
    // Create a 3-channel visualization of the mask
    // Green for selected areas, transparent for unselected
    cv::Mat layerVis = cv::Mat::zeros(mask.size(), CV_8UC3);
    layerVis.setTo(cv::Scalar(0, 255, 0), mask);  // Green where mask is active
    
    return layerVis;
}

void SelectionTool::loadMaskFromLayer(const cv::Mat& layerMask) {
    if (layerMask.empty()) {
        clearMask();
        return;
    }
    
    // Convert layer to mask (extract green channel or convert to grayscale)
    if (layerMask.channels() == 3) {
        cv::Mat gray;
        cv::cvtColor(layerMask, gray, cv::COLOR_BGR2GRAY);
        mask = gray.clone();
    } else {
        mask = layerMask.clone();
    }
    
    // Threshold to ensure binary mask
    cv::threshold(mask, mask, 127, 255, cv::THRESH_BINARY);
    
    qDebug() << "Loaded mask from layer. Non-zero pixels:" << cv::countNonZero(mask);
    emit selectionCompleted();
}

QString SelectionTool::getSelectionDescription() const {
    if (!hasMask()) return "No selection";
    
    int pixels = cv::countNonZero(mask);
    double percentage = (pixels * 100.0) / (mask.rows * mask.cols);
    
    QString modeName = getModeName();
    return QString("%1 (%2 pixels, %3%)")
        .arg(modeName)
        .arg(pixels)
        .arg(percentage, 0, 'f', 1);
}

// ==================== VISUALIZATION ====================

cv::Mat SelectionTool::getOverlay(const cv::Mat& image) const {
    if (image.empty()) return cv::Mat();
    
    cv::Mat overlay = image.clone();
    if (overlay.channels() == 1) {
        cv::cvtColor(overlay, overlay, cv::COLOR_GRAY2BGR);
    }
    
    switch (currentMode) {
        case SelectionMode::Rectangle: {
            if (isSelectingRect || (rectStart != rectEnd)) {
                int x1 = std::min(rectStart.x(), rectEnd.x());
                int y1 = std::min(rectStart.y(), rectEnd.y());
                int x2 = std::max(rectStart.x(), rectEnd.x());
                int y2 = std::max(rectStart.y(), rectEnd.y());
                
                // Draw semi-transparent green fill
                cv::Mat rectMask = cv::Mat::zeros(overlay.size(), CV_8UC3);
                cv::rectangle(rectMask, cv::Point(x1, y1), cv::Point(x2, y2), 
                            cv::Scalar(0, 255, 0), -1);
                cv::addWeighted(overlay, 0.7, rectMask, 0.3, 0, overlay);
                
                // Draw thick red border
                cv::rectangle(overlay, cv::Point(x1, y1), cv::Point(x2, y2), 
                            cv::Scalar(0, 0, 255), 3);
            }
            break;
        }
        
        case SelectionMode::Polygon: {
            if (polygonPoints.size() > 1) {
                for (size_t i = 0; i < polygonPoints.size() - 1; i++) {
                    cv::line(overlay, 
                           cv::Point(polygonPoints[i].x(), polygonPoints[i].y()),
                           cv::Point(polygonPoints[i+1].x(), polygonPoints[i+1].y()),
                           cv::Scalar(0, 255, 0), 2);
                }
                // Draw points
                for (const QPoint& p : polygonPoints) {
                    cv::circle(overlay, cv::Point(p.x(), p.y()), 4, cv::Scalar(255, 0, 0), -1);
                }
            }
            break;
        }
        
        default: {
            // For other modes, show mask overlay with high visibility
            if (!mask.empty() && mask.size() == overlay.size()) {
                // Create bright green overlay
                cv::Mat colorMask = cv::Mat::zeros(mask.size(), CV_8UC3);
                colorMask.setTo(cv::Scalar(0, 255, 0), mask);
                cv::addWeighted(overlay, 0.4, colorMask, 0.6, 0, overlay);
                
                // Draw red border around selection for better visibility
                std::vector<std::vector<cv::Point>> contours;
                cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
                cv::drawContours(overlay, contours, -1, cv::Scalar(0, 0, 255), 3);
            }
            break;
        }
    }
    
    return overlay;
}
