#include "ROIShape.h"
#include <algorithm>
#include <vector>

// ROIShape base class implementation
ROIShape::ROIShape(const QString& name, const QColor& color)
    : name(name), color(color), visible(true) {
}

ROIStats ROIShape::calculateStats(const cv::Mat& image) {
    ROIStats result;
    
    if (image.empty()) {
        return result;
    }
    
    QRect bounds = getBounds();
    
    // Validate bounds
    if (bounds.x() < 0 || bounds.y() < 0 ||
        bounds.x() + bounds.width() > image.cols ||
        bounds.y() + bounds.height() > image.rows) {
        return result;
    }
    
    // Extract ROI
    cv::Rect cvRoi(bounds.x(), bounds.y(), bounds.width(), bounds.height());
    cv::Mat roiMat = image(cvRoi);
    
    result.area = bounds.width() * bounds.height();
    
    // Calculate statistics based on image type
    if (image.channels() == 1) {
        // Grayscale image
        cv::Scalar mean, stddev;
        cv::meanStdDev(roiMat, mean, stddev);
        
        result.mean = mean[0];
        result.stdDev = stddev[0];
        
        double minVal, maxVal;
        cv::minMaxLoc(roiMat, &minVal, &maxVal);
        result.min = minVal;
        result.max = maxVal;
        
        // Calculate median
        cv::Mat flat = roiMat.reshape(1, roiMat.total());
        cv::Mat sorted;
        cv::sort(flat, sorted, cv::SORT_ASCENDING);
        result.median = sorted.at<uchar>(sorted.rows / 2);
        
        // Calculate sum
        result.sum = cv::sum(roiMat)[0];
        
        result.hasColorStats = false;
    } else if (image.channels() == 3) {
        // Color image - calculate overall and per-channel stats
        cv::Scalar mean, stddev;
        cv::meanStdDev(roiMat, mean, stddev);
        
        // Overall mean (average of all channels)
        result.mean = (mean[0] + mean[1] + mean[2]) / 3.0;
        result.stdDev = (stddev[0] + stddev[1] + stddev[2]) / 3.0;
        
        // Split channels
        std::vector<cv::Mat> channels;
        cv::split(roiMat, channels);
        
        // Blue channel (OpenCV uses BGR)
        cv::Scalar blueMean, blueStd;
        cv::meanStdDev(channels[0], blueMean, blueStd);
        result.blue.mean = blueMean[0];
        result.blue.stdDev = blueStd[0];
        double blueMin, blueMax;
        cv::minMaxLoc(channels[0], &blueMin, &blueMax);
        result.blue.min = blueMin;
        result.blue.max = blueMax;
        
        // Green channel
        cv::Scalar greenMean, greenStd;
        cv::meanStdDev(channels[1], greenMean, greenStd);
        result.green.mean = greenMean[0];
        result.green.stdDev = greenStd[0];
        double greenMin, greenMax;
        cv::minMaxLoc(channels[1], &greenMin, &greenMax);
        result.green.min = greenMin;
        result.green.max = greenMax;
        
        // Red channel
        cv::Scalar redMean, redStd;
        cv::meanStdDev(channels[2], redMean, redStd);
        result.red.mean = redMean[0];
        result.red.stdDev = redStd[0];
        double redMin, redMax;
        cv::minMaxLoc(channels[2], &redMin, &redMax);
        result.red.min = redMin;
        result.red.max = redMax;
        
        // Overall min/max
        result.min = std::min({result.blue.min, result.green.min, result.red.min});
        result.max = std::max({result.blue.max, result.green.max, result.red.max});
        
        // Calculate median (use green channel as representative)
        cv::Mat flat = channels[1].reshape(1, channels[1].total());
        cv::Mat sorted;
        cv::sort(flat, sorted, cv::SORT_ASCENDING);
        result.median = sorted.at<uchar>(sorted.rows / 2);
        
        // Sum
        result.sum = cv::sum(roiMat)[0] + cv::sum(roiMat)[1] + cv::sum(roiMat)[2];
        
        result.hasColorStats = true;
    }
    
    result.isValid = true;
    stats = result;
    return result;
}

// RectangleROI implementation
RectangleROI::RectangleROI(const QRect& rect, const QString& name, const QColor& color)
    : ROIShape(name, color), rect(rect) {
}

bool RectangleROI::contains(const QPoint& point) const {
    return rect.contains(point);
}

void RectangleROI::draw(QPainter& painter) const {
    if (!visible) return;
    
    QPen pen(color, 2, Qt::SolidLine);
    painter.setPen(pen);
    
    // Draw rectangle border
    painter.drawRect(rect);
    
    // Draw semi-transparent fill
    QColor fillColor = color;
    fillColor.setAlpha(30);
    painter.fillRect(rect, fillColor);
    
    // Draw label
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    
    QString label = name;
    QRect labelRect = painter.fontMetrics().boundingRect(label);
    
    // Position label at top-left of ROI
    QPoint labelPos(rect.x() + 5, rect.y() - 5);
    
    // Draw label background
    QRect labelBg(labelPos.x() - 3, labelPos.y() - labelRect.height(),
                  labelRect.width() + 6, labelRect.height() + 4);
    painter.fillRect(labelBg, QColor(0, 0, 0, 180));
    
    // Draw label text
    painter.setPen(Qt::white);
    painter.drawText(labelPos.x(), labelPos.y(), label);
}

ROIShape* RectangleROI::clone() const {
    RectangleROI* copy = new RectangleROI(rect, name, color);
    copy->setVisible(visible);
    copy->setStats(stats);
    return copy;
}
