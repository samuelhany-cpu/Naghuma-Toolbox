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
    
    // Auto Enhancement
    static void applyAdaptiveHistogramEqualization(const cv::Mat& src, cv::Mat& dst);
    static void applyContrastStretching(const cv::Mat& src, cv::Mat& dst);
    
    // Noise Removal
    static void applyGaussianNoiseRemoval(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5);
    static void applyMedianFilter(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5);
    static void applyBilateralFilter(const cv::Mat& src, cv::Mat& dst, int diameter = 9, double sigmaColor = 75, double sigmaSpace = 75);
    
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
    
    // ========== PHASE 16: THRESHOLDING & SEGMENTATION =========
    
    // Simple thresholding
    static void applySimpleThreshold(const cv::Mat& src, cv::Mat& dst, int threshold, int maxValue);
    
    // Adaptive thresholding
    static void applyAdaptiveThreshold(const cv::Mat& src, cv::Mat& dst, int maxValue, 
                                       int blockSize, int C, bool useGaussian = false);
    
    // Otsu thresholding (returns computed threshold)
    static double computeOtsuThreshold(const cv::Mat& src, cv::Mat& dst);
    
    // Multi-level Otsu
    static void applyMultiLevelOtsu(const cv::Mat& src, cv::Mat& dst, int levels = 2);
    
    // Local thresholding
    static void applyLocalThreshold(const cv::Mat& src, cv::Mat& dst, int blockSize, int C);
    
    // Variable thresholding (dynamic threshold based on local statistics)
    static void applyVariableThreshold(const cv::Mat& src, cv::Mat& dst, double k = 0.5);
    
    // ========== IMAGE RESTORATION ==========
    
    // Wiener filter for restoration (deconvolution)
    static void applyWienerFilter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& psf, double noiseVariance = 0.01);
    
    // Constrained Least Squares (CLS) restoration
    static void applyCLSRestoration(const cv::Mat& src, cv::Mat& dst, const cv::Mat& psf, double gamma = 0.1);
    
    // Inverse filtering
    static void applyInverseFilter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& psf, double epsilon = 0.001);
    
    // Motion blur restoration
    static void restoreMotionBlur(const cv::Mat& src, cv::Mat& dst, int length, double angle);
    
    // Atmospheric turbulence restoration
    static void restoreAtmosphericBlur(const cv::Mat& src, cv::Mat& dst, double k = 0.001);
    
    // ========== GEOMETRIC DISTORTION ==========
    
    // Barrel distortion correction
    static void correctBarrelDistortion(const cv::Mat& src, cv::Mat& dst, double k1, double k2 = 0.0);
    
    // Pincushion distortion correction
    static void correctPincushionDistortion(const cv::Mat& src, cv::Mat& dst, double k1, double k2 = 0.0);
    
    // Perspective distortion correction (parallel lines)
    static void correctPerspectiveDistortion(const cv::Mat& src, cv::Mat& dst, 
                                            const std::vector<cv::Point2f>& srcPoints,
                                            const std::vector<cv::Point2f>& dstPoints);
    
    // Apply trapezoidal/keystone correction
    static void correctKeystoneDistortion(const cv::Mat& src, cv::Mat& dst, double angle);
};

#endif // IMAGEPROCESSOR_H
