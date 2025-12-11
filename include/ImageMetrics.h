#ifndef IMAGEMETRICS_H
#define IMAGEMETRICS_H

#include <opencv2/opencv.hpp>
#include <QString>

class ImageMetrics {
public:
    struct MetricsResult {
        double mse;
        double rmse;
        double snr;
        double psnr;
        bool isValid;
        QString errorMessage;
    };
    
    // Calculate all metrics between original and processed image
    static MetricsResult calculateMetrics(const cv::Mat& original, const cv::Mat& processed);
    
    // Individual metric calculations
    static double calculateMSE(const cv::Mat& original, const cv::Mat& processed);
    static double calculateRMSE(const cv::Mat& original, const cv::Mat& processed);
    static double calculateSNR(const cv::Mat& original, const cv::Mat& processed);
    static double calculatePSNR(const cv::Mat& original, const cv::Mat& processed, double L = 255.0);
    
    // Helper to format metrics for display
    static QString formatMetrics(const MetricsResult& result);
    
private:
    static bool validateImages(const cv::Mat& img1, const cv::Mat& img2);
};

#endif // IMAGEMETRICS_H
