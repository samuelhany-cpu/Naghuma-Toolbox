#include "ROIManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

ROIManager::ROIManager(QObject *parent)
    : QObject(parent) {
}

ROIManager::~ROIManager() {
    clearAll();
}

void ROIManager::addROI(ROIShape* roi) {
    if (!roi) return;
    
    rois.append(roi);
    emit roiAdded(rois.size() - 1);
}

void ROIManager::removeROI(int index) {
    if (index < 0 || index >= rois.size()) return;
    
    delete rois[index];
    rois.removeAt(index);
    emit roiRemoved(index);
}

void ROIManager::clearAll() {
    for (ROIShape* roi : rois) {
        delete roi;
    }
    rois.clear();
    emit allRoisCleared();
}

ROIShape* ROIManager::getROI(int index) const {
    if (index < 0 || index >= rois.size()) return nullptr;
    return rois[index];
}

bool ROIManager::saveToFile(const QString& filepath) {
    QJsonArray roiArray;
    
    for (const ROIShape* roi : rois) {
        QJsonObject roiObj;
        roiObj["name"] = roi->getName();
        roiObj["type"] = roi->getType();
        roiObj["color"] = roi->getColor().name();
        roiObj["visible"] = roi->isVisible();
        
        QRect bounds = roi->getBounds();
        roiObj["x"] = bounds.x();
        roiObj["y"] = bounds.y();
        roiObj["width"] = bounds.width();
        roiObj["height"] = bounds.height();
        
        roiArray.append(roiObj);
    }
    
    QJsonObject root;
    root["version"] = "1.0";
    root["rois"] = roiArray;
    
    QJsonDocument doc(root);
    
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    return true;
}

bool ROIManager::loadFromFile(const QString& filepath) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }
    
    QJsonObject root = doc.object();
    QJsonArray roiArray = root["rois"].toArray();
    
    clearAll();
    
    for (const QJsonValue& val : roiArray) {
        QJsonObject roiObj = val.toObject();
        
        QString name = roiObj["name"].toString();
        QColor color(roiObj["color"].toString());
        bool visible = roiObj["visible"].toBool();
        
        int x = roiObj["x"].toInt();
        int y = roiObj["y"].toInt();
        int width = roiObj["width"].toInt();
        int height = roiObj["height"].toInt();
        
        QRect rect(x, y, width, height);
        RectangleROI* roi = new RectangleROI(rect, name, color);
        roi->setVisible(visible);
        
        addROI(roi);
    }
    
    return true;
}

bool ROIManager::exportStatisticsToCSV(const QString& filepath, const cv::Mat& image) {
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream out(&file);
    
    // Write header
    out << "ROI Name,X,Y,Width,Height,Area,Mean,StdDev,Min,Max,Median,Sum";
    
    // Check if color image
    bool hasColor = !rois.isEmpty() && image.channels() == 3;
    if (hasColor) {
        out << ",Red Mean,Red StdDev,Red Min,Red Max";
        out << ",Green Mean,Green StdDev,Green Min,Green Max";
        out << ",Blue Mean,Blue StdDev,Blue Min,Blue Max";
    }
    out << "\n";
    
    // Write data for each ROI
    for (ROIShape* roi : rois) {
        ROIStats stats = roi->calculateStats(image);
        
        if (!stats.isValid) continue;
        
        QRect bounds = roi->getBounds();
        
        out << roi->getName() << ","
            << bounds.x() << ","
            << bounds.y() << ","
            << bounds.width() << ","
            << bounds.height() << ","
            << stats.area << ","
            << stats.mean << ","
            << stats.stdDev << ","
            << stats.min << ","
            << stats.max << ","
            << stats.median << ","
            << stats.sum;
        
        if (hasColor && stats.hasColorStats) {
            out << "," << stats.red.mean << "," << stats.red.stdDev << "," << stats.red.min << "," << stats.red.max;
            out << "," << stats.green.mean << "," << stats.green.stdDev << "," << stats.green.min << "," << stats.green.max;
            out << "," << stats.blue.mean << "," << stats.blue.stdDev << "," << stats.blue.min << "," << stats.blue.max;
        }
        
        out << "\n";
    }
    
    file.close();
    return true;
}

#include "moc_ROIManager.cpp"
