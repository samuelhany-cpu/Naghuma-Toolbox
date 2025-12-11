#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include <QObject>
#include <QColor>
#include <QPoint>
#include <opencv2/opencv.hpp>

enum class BrushType {
    Circle,
    Square,
    SoftEdge
};

class BrushTool : public QObject {
    Q_OBJECT

public:
    explicit BrushTool(QObject *parent = nullptr);
    ~BrushTool();

    // Brush properties
    void setBrushType(BrushType type) { brushType = type; }
    void setBrushSize(int size) { brushSize = size; }
    void setBrushOpacity(int opacity) { brushOpacity = opacity; }
    void setBrushColor(const QColor& color) { brushColor = color; }
    void setEraserMode(bool enabled) { eraserMode = enabled; }
    
    BrushType getBrushType() const { return brushType; }
    int getBrushSize() const { return brushSize; }
    int getBrushOpacity() const { return brushOpacity; }
    QColor getBrushColor() const { return brushColor; }
    bool isEraserMode() const { return eraserMode; }
    bool isCurrentlyDrawing() const { return isDrawing; }

    // Drawing operations
    void startDrawing(const QPoint& pos);
    void continueDrawing(const QPoint& pos);
    void finishDrawing();
    
    // Apply drawing to image
    void applyToImage(cv::Mat& image);
    void clearStroke();
    
    // Get preview of current stroke
    cv::Mat getStrokePreview(const cv::Mat& baseImage) const;

signals:
    void strokeUpdated();

private:
    void drawBrushStroke(cv::Mat& canvas, const QPoint& from, const QPoint& to);
    void drawCircleBrush(cv::Mat& canvas, const cv::Point& center);
    void drawSquareBrush(cv::Mat& canvas, const cv::Point& center);
    void drawSoftBrush(cv::Mat& canvas, const cv::Point& center);

    BrushType brushType;
    int brushSize;
    int brushOpacity;
    QColor brushColor;
    bool eraserMode;
    
    QPoint lastPoint;
    bool isDrawing;
    
    cv::Mat strokeCanvas;
    std::vector<QPoint> strokePoints;
};

#endif // BRUSHTOOL_H
