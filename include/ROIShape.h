#ifndef ROISHAPE_H
#define ROISHAPE_H

#include <QString>
#include <QRect>
#include <QColor>
#include <QPoint>
#include <QPainter>
#include <opencv2/opencv.hpp>

// ROI Statistics structure
struct ROIStats {
    double mean = 0.0;
    double stdDev = 0.0;
    double min = 0.0;
    double max = 0.0;
    double median = 0.0;
    int area = 0;
    long long sum = 0;
    bool isValid = false;
    
    // Per-channel stats for color images
    struct ChannelStats {
        double mean = 0.0;
        double stdDev = 0.0;
        double min = 0.0;
        double max = 0.0;
    };
    
    ChannelStats red;
    ChannelStats green;
    ChannelStats blue;
    
    bool hasColorStats = false;
};

// Base class for all ROI shapes
class ROIShape {
public:
    enum ShapeType {
        Rectangle,
        Ellipse,
        Polygon,
        FreeHand
    };
    
    ROIShape(const QString& name = "ROI", const QColor& color = Qt::red);
    virtual ~ROIShape() = default;
    
    // Pure virtual methods
    virtual ShapeType getType() const = 0;
    virtual QRect getBounds() const = 0;
    virtual bool contains(const QPoint& point) const = 0;
    virtual void draw(QPainter& painter) const = 0;
    virtual ROIShape* clone() const = 0;
    
    // Common properties
    QString getName() const { return name; }
    void setName(const QString& n) { name = n; }
    
    QColor getColor() const { return color; }
    void setColor(const QColor& c) { color = c; }
    
    bool isVisible() const { return visible; }
    void setVisible(bool v) { visible = v; }
    
    // Statistics
    ROIStats getStats() const { return stats; }
    void setStats(const ROIStats& s) { stats = s; }
    
    // Calculate statistics from image
    ROIStats calculateStats(const cv::Mat& image);
    
protected:
    QString name;
    QColor color;
    bool visible;
    ROIStats stats;
};

// Rectangle ROI
class RectangleROI : public ROIShape {
public:
    RectangleROI(const QRect& rect = QRect(), 
                 const QString& name = "Rectangle ROI",
                 const QColor& color = Qt::red);
    
    ShapeType getType() const override { return Rectangle; }
    QRect getBounds() const override { return rect; }
    bool contains(const QPoint& point) const override;
    void draw(QPainter& painter) const override;
    ROIShape* clone() const override;
    
    // Rectangle-specific
    void setRect(const QRect& r) { rect = r; }
    QRect getRect() const { return rect; }
    
private:
    QRect rect;
};

#endif // ROISHAPE_H
