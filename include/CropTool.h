#ifndef CROPTOOL_H
#define CROPTOOL_H

#include <QObject>
#include <QPoint>
#include <QRect>
#include <opencv2/opencv.hpp>

class CropTool : public QObject {
    Q_OBJECT

public:
    explicit CropTool(QObject *parent = nullptr);
    ~CropTool();

    // Selection state
    void startSelection(const QPoint& pos);
    void updateSelection(const QPoint& pos);
    void finishSelection();
    void cancelSelection();
    
    // Get crop rectangle
    QRect getCropRect() const { return cropRect; }
    bool hasSelection() const { return isSelecting || selectionComplete; }
    bool isSelectingNow() const { return isSelecting; }
    
    // Apply crop to image
    cv::Mat applyCrop(const cv::Mat& image) const;
    
    // Preview
    cv::Mat getPreview(const cv::Mat& baseImage) const;
    
    // Validate crop rectangle
    bool isValidCrop() const;
    QRect getValidatedRect(const cv::Size& imageSize) const;

signals:
    void selectionChanged(const QRect& rect);
    void selectionFinished(const QRect& rect);

private:
    QPoint startPoint;
    QPoint currentPoint;
    QRect cropRect;
    bool isSelecting;
    bool selectionComplete;
    
    void updateCropRect();
};

#endif // CROPTOOL_H
