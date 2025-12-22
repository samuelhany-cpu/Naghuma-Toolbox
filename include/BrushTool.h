#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QPoint>

class BrushTool : public QObject {
    Q_OBJECT

public:
    explicit BrushTool(QObject* parent = nullptr);
    
    // Brush operations
    void startStroke(const QPoint& pos, const cv::Size& imageSize);
    void continueStroke(const QPoint& pos);
    void endStroke();
    
    // Brush settings
    void setBrushSize(int size) { brushSize = size; }
    int getBrushSize() const { return brushSize; }
    
    void setBrushMode(bool erase) { eraseMode = erase; }
    bool isEraseMode() const { return eraseMode; }
    
    // Mask operations
    void clearMask();
    bool hasMask() const { return !mask.empty(); }
    cv::Mat getMask() const { return mask.clone(); }
    void setMask(const cv::Mat& newMask) { mask = newMask.clone(); }
    
    // Apply mask to image processing
    cv::Mat applyMaskToResult(const cv::Mat& original, const cv::Mat& processed) const;
    
    // Visualization
    cv::Mat getMaskOverlay(const cv::Mat& image, const cv::Scalar& color = cv::Scalar(0, 255, 0)) const;
    
signals:
    void maskUpdated();
    
private:
    cv::Mat mask;           // Binary mask (0 = not selected, 255 = selected)
    int brushSize;          // Brush radius in pixels
    bool eraseMode;         // false = paint, true = erase
    bool isDrawing;         // Currently drawing
    QPoint lastPoint;       // Last mouse position
    
    void drawBrushStroke(const QPoint& from, const QPoint& to);
};

#endif // BRUSHTOOL_H
