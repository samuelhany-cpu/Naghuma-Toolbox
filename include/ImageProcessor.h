#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <QString>

class ImageProcessor {
public:
    // Basic processing
    static void convertToGrayscale(const cv::Mat& src, cv::Mat& dst);
    static void applyBinaryThreshold(const cv::Mat& src, cv::Mat& dst, int threshold = 128);
    static void applyGaussianBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize = 15);
    static void detectEdges(const cv::Mat& src, cv::Mat& dst, double threshold1 = 100, double threshold2 = 200);
    static void invertColors(const cv::Mat& src, cv::Mat& dst);
    
    // Histogram operations
    static void equalizeHistogram(const cv::Mat& src, cv::Mat& dst);
    static void applyOtsuThreshold(const cv::Mat& src, cv::Mat& dst);
    
    // Adjustments
    static void adjustBrightnessContrast(const cv::Mat& src, cv::Mat& dst, int brightness, int contrast);
    
    // Morphology Operations
    static void applyErosion(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5);
    static void applyDilation(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5);
    static void applyOpening(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5);
    static void applyClosing(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5);
    static void applyMorphGradient(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5);
    
    // FFT Operations
    static void applyFFT(const cv::Mat& src, cv::Mat& magnitude, cv::Mat& phase);
    static void applyLowPassFilter(const cv::Mat& src, cv::Mat& dst, int radius = 30);
    static void applyHighPassFilter(const cv::Mat& src, cv::Mat& dst, int radius = 30);
    
    // Transforms
    static void flipHorizontal(const cv::Mat& src, cv::Mat& dst);
    static void flipVertical(const cv::Mat& src, cv::Mat& dst);
    static void flipBoth(const cv::Mat& src, cv::Mat& dst);
    static void applySkew(const cv::Mat& src, cv::Mat& dst);
    static void applySkew(const cv::Mat& src, cv::Mat& dst, double skewX, double skewY);
    static void translate(const cv::Mat& src, cv::Mat& dst, int tx, int ty);
    static void rotate(const cv::Mat& src, cv::Mat& dst, double angle);
    static void zoom(const cv::Mat& src, cv::Mat& dst, double scale);
};

#endif // IMAGEPROCESSOR_H
