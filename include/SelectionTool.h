#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <vector>

enum class SelectionMode {
    Rectangle,
    Polygon,
    MagicWand,
    Threshold,
    EdgeBased
};

class SelectionTool : public QObject {
    Q_OBJECT

public:
    explicit SelectionTool(QObject* parent = nullptr);
    
    // Mode management
    void setMode(SelectionMode mode) { currentMode = mode; }
    SelectionMode getMode() const { return currentMode; }
    QString getModeName() const;
    
    // Rectangle selection
    void startRectangle(const QPoint& pos);
    void updateRectangle(const QPoint& pos);
    void endRectangle(const cv::Size& imageSize);
    
    // Polygon selection
    void addPolygonPoint(const QPoint& pos);
    void closePolygon(const cv::Size& imageSize);
    void clearPolygon();
    
    // Magic wand (color-based)
    void magicWandSelect(const cv::Mat& image, const QPoint& seedPoint, int tolerance = 30);
    void setMagicWandTolerance(int tolerance) { magicWandTolerance = tolerance; }
    
    // Threshold-based
    void thresholdSelect(const cv::Mat& image, int minThreshold, int maxThreshold);
    
    // Edge-based (intelligent selection)
    void edgeBasedSelect(const cv::Mat& image, const QPoint& seedPoint);
    
    // Mask operations
    cv::Mat getMask(const cv::Size& imageSize) const;
    bool hasMask() const { return !mask.empty(); }
    void clearMask();
    void setMask(const cv::Mat& newMask) { mask = newMask.clone(); }
    
    // Apply mask to processing
    cv::Mat applyMaskToResult(const cv::Mat& original, const cv::Mat& processed) const;
    
    // Layer integration
    cv::Mat getMaskAsLayer() const;  // Get mask as a visible layer
    void loadMaskFromLayer(const cv::Mat& layerMask);  // Load mask from layer
    QString getSelectionDescription() const;  // Get description of current selection
    
    // Visualization
    cv::Mat getOverlay(const cv::Mat& image) const;
    QRect getRectangle() const { return QRect(rectStart, rectEnd); }
    std::vector<QPoint> getPolygonPoints() const { return polygonPoints; }
    
signals:
    void selectionUpdated();
    void selectionCompleted();
    
private:
    SelectionMode currentMode;
    cv::Mat mask;
    
    // Rectangle mode
    QPoint rectStart;
    QPoint rectEnd;
    bool isSelectingRect;
    
    // Polygon mode
    std::vector<QPoint> polygonPoints;
    
    // Magic wand settings
    int magicWandTolerance;
    
    // Helper functions
    void createRectangleMask(const cv::Size& imageSize);
    void createPolygonMask(const cv::Size& imageSize);
    void floodFillSelection(const cv::Mat& image, const cv::Point& seed, int tolerance);
};

#endif // SELECTIONTOOL_H
