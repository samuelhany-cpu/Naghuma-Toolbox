#ifndef ROIMANAGER_H
#define ROIMANAGER_H

#include <QObject>
#include <QList>
#include "ROIShape.h"

class ROIManager : public QObject {
    Q_OBJECT
    
public:
    explicit ROIManager(QObject *parent = nullptr);
    ~ROIManager();
    
    // ROI management
    void addROI(ROIShape* roi);
    void removeROI(int index);
    void clearAll();
    int count() const { return rois.size(); }
    ROIShape* getROI(int index) const;
    QList<ROIShape*> getAllROIs() const { return rois; }
    
    // File operations
    bool saveToFile(const QString& filepath);
    bool loadFromFile(const QString& filepath);
    
    // Statistics export
    bool exportStatisticsToCSV(const QString& filepath, const cv::Mat& image);
    
signals:
    void roiAdded(int index);
    void roiRemoved(int index);
    void roiModified(int index);
    void allRoisCleared();
    
private:
    QList<ROIShape*> rois;
};

#endif // ROIMANAGER_H
