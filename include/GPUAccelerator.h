#ifndef GPUACCELERATOR_H
#define GPUACCELERATOR_H

#include <opencv2/opencv.hpp>
#include <QString>

#ifdef HAVE_OPENCV_CUDA
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#endif

/**
 * @brief GPU Acceleration Manager for OpenCV operations
 * 
 * This class provides a unified interface for GPU-accelerated image processing.
 * It automatically falls back to CPU if CUDA is not available.
 */
class GPUAccelerator {
public:
    static GPUAccelerator& instance();
    
    // Initialization and status
    bool isAvailable() const;
    QString getDeviceInfo() const;
    void initialize();
    
    // GPU-accelerated operations
    cv::Mat gaussianBlur(const cv::Mat& input, int kernelSize, double sigma);
    cv::Mat medianFilter(const cv::Mat& input, int kernelSize);
    cv::Mat bilateralFilter(const cv::Mat& input, int d, double sigmaColor, double sigmaSpace);
    
    cv::Mat resize(const cv::Mat& input, cv::Size size, int interpolation = cv::INTER_LINEAR);
    cv::Mat warpAffine(const cv::Mat& input, const cv::Mat& M, cv::Size size);
    cv::Mat flip(const cv::Mat& input, int flipCode);
    
    cv::Mat convertColor(const cv::Mat& input, int code);
    cv::Mat threshold(const cv::Mat& input, double thresh, double maxval, int type);
    
    cv::Mat sobel(const cv::Mat& input, int dx, int dy, int ksize);
    cv::Mat laplacian(const cv::Mat& input, int ksize);
    cv::Mat canny(const cv::Mat& input, double threshold1, double threshold2);
    
    cv::Mat erode(const cv::Mat& input, const cv::Mat& kernel, int iterations = 1);
    cv::Mat dilate(const cv::Mat& input, const cv::Mat& kernel, int iterations = 1);
    cv::Mat morphologyEx(const cv::Mat& input, int op, const cv::Mat& kernel);
    
    cv::Mat add(const cv::Mat& src1, const cv::Mat& src2);
    cv::Mat subtract(const cv::Mat& src1, const cv::Mat& src2);
    cv::Mat multiply(const cv::Mat& src1, const cv::Mat& src2);
    cv::Mat absdiff(const cv::Mat& src1, const cv::Mat& src2);
    
    // Histogram operations
    cv::Mat equalizeHist(const cv::Mat& input);
    
    // Performance monitoring
    void enableProfiling(bool enable);
    double getLastOperationTime() const;
    
private:
    GPUAccelerator();
    ~GPUAccelerator();
    
    GPUAccelerator(const GPUAccelerator&) = delete;
    GPUAccelerator& operator=(const GPUAccelerator&) = delete;
    
    bool cudaAvailable;
    bool profilingEnabled;
    double lastOperationTime;
    
#ifdef HAVE_OPENCV_CUDA
    int deviceCount;
    int currentDevice;
    cv::cuda::DeviceInfo deviceInfo;
#endif
    
    void logPerformance(const QString& operation, double time);
};

#endif // GPUACCELERATOR_H
