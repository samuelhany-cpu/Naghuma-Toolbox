#include "GPUAccelerator.h"
#include <QDebug>
#include <chrono>

GPUAccelerator& GPUAccelerator::instance() {
    static GPUAccelerator instance;
    return instance;
}

GPUAccelerator::GPUAccelerator() 
    : cudaAvailable(false), profilingEnabled(false), lastOperationTime(0.0) {
    initialize();
}

GPUAccelerator::~GPUAccelerator() {
}

void GPUAccelerator::initialize() {
#ifdef HAVE_OPENCV_CUDA
    try {
        deviceCount = cv::cuda::getCudaEnabledDeviceCount();
        if (deviceCount > 0) {
            currentDevice = 0;
            cv::cuda::setDevice(currentDevice);
            deviceInfo = cv::cuda::DeviceInfo(currentDevice);
            cudaAvailable = true;
            
            qDebug() << "GPU Acceleration initialized successfully";
            qDebug() << "CUDA Device:" << QString::fromStdString(deviceInfo.name());
            qDebug() << "Compute Capability:" << deviceInfo.majorVersion() << "." << deviceInfo.minorVersion();
            qDebug() << "Total Memory:" << deviceInfo.totalMemory() / (1024 * 1024) << "MB";
        } else {
            qDebug() << "No CUDA-capable devices found. Using CPU fallback.";
            cudaAvailable = false;
        }
    } catch (const cv::Exception& e) {
        qDebug() << "CUDA initialization failed:" << e.what();
        cudaAvailable = false;
    }
#else
    qDebug() << "OpenCV compiled without CUDA support. Using CPU fallback.";
    cudaAvailable = false;
#endif
}

bool GPUAccelerator::isAvailable() const {
    return cudaAvailable;
}

QString GPUAccelerator::getDeviceInfo() const {
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        return QString("GPU: %1 (Compute %2.%3) - %4 MB")
            .arg(QString::fromStdString(deviceInfo.name()))
            .arg(deviceInfo.majorVersion())
            .arg(deviceInfo.minorVersion())
            .arg(deviceInfo.totalMemory() / (1024 * 1024));
    }
#endif
    return "GPU: Not Available (Using CPU)";
}

void GPUAccelerator::enableProfiling(bool enable) {
    profilingEnabled = enable;
}

double GPUAccelerator::getLastOperationTime() const {
    return lastOperationTime;
}

void GPUAccelerator::logPerformance(const QString& operation, double time) {
    lastOperationTime = time;
    if (profilingEnabled) {
        qDebug() << operation << "completed in" << time << "ms";
    }
}

// ============================================================================
// GPU-ACCELERATED OPERATIONS
// ============================================================================

cv::Mat GPUAccelerator::gaussianBlur(const cv::Mat& input, int kernelSize, double sigma) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createGaussianFilter(
                gpuInput.type(), -1, cv::Size(kernelSize, kernelSize), sigma
            );
            filter->apply(gpuInput, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Gaussian Blur", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Gaussian Blur failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::GaussianBlur(input, result, cv::Size(kernelSize, kernelSize), sigma);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Gaussian Blur", elapsed);
    return result;
}

cv::Mat GPUAccelerator::medianFilter(const cv::Mat& input, int kernelSize) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createMedianFilter(
                gpuInput.type(), kernelSize
            );
            filter->apply(gpuInput, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Median Filter", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Median Filter failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::medianBlur(input, result, kernelSize);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Median Filter", elapsed);
    return result;
}

cv::Mat GPUAccelerator::bilateralFilter(const cv::Mat& input, int d, double sigmaColor, double sigmaSpace) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
    // Note: CUDA bilateral filter may not be available in all OpenCV builds
    // Using CPU implementation as it's often more reliable
    cv::bilateralFilter(input, result, d, sigmaColor, sigmaSpace);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("Bilateral Filter", elapsed);
    return result;
}

cv::Mat GPUAccelerator::resize(const cv::Mat& input, cv::Size size, int interpolation) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::cuda::resize(gpuInput, gpuOutput, size, 0, 0, interpolation);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Resize", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Resize failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::resize(input, result, size, 0, 0, interpolation);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Resize", elapsed);
    return result;
}

cv::Mat GPUAccelerator::warpAffine(const cv::Mat& input, const cv::Mat& M, cv::Size size) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::cuda::warpAffine(gpuInput, gpuOutput, M, size);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Warp Affine", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Warp Affine failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::warpAffine(input, result, M, size);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Warp Affine", elapsed);
    return result;
}

cv::Mat GPUAccelerator::flip(const cv::Mat& input, int flipCode) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::cuda::flip(gpuInput, gpuOutput, flipCode);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Flip", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Flip failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::flip(input, result, flipCode);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Flip", elapsed);
    return result;
}

cv::Mat GPUAccelerator::convertColor(const cv::Mat& input, int code) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::cuda::cvtColor(gpuInput, gpuOutput, code);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Color Conversion", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Color Conversion failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::cvtColor(input, result, code);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Color Conversion", elapsed);
    return result;
}

cv::Mat GPUAccelerator::threshold(const cv::Mat& input, double thresh, double maxval, int type) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::cuda::threshold(gpuInput, gpuOutput, thresh, maxval, type);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Threshold", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Threshold failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::threshold(input, result, thresh, maxval, type);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Threshold", elapsed);
    return result;
}

cv::Mat GPUAccelerator::sobel(const cv::Mat& input, int dx, int dy, int ksize) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createSobelFilter(
                gpuInput.type(), CV_8U, dx, dy, ksize
            );
            filter->apply(gpuInput, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Sobel", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Sobel failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::Sobel(input, result, CV_8U, dx, dy, ksize);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Sobel", elapsed);
    return result;
}

cv::Mat GPUAccelerator::laplacian(const cv::Mat& input, int ksize) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createLaplacianFilter(
                gpuInput.type(), CV_8U, ksize
            );
            filter->apply(gpuInput, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Laplacian", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Laplacian failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::Laplacian(input, result, CV_8U, ksize);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Laplacian", elapsed);
    return result;
}

cv::Mat GPUAccelerator::canny(const cv::Mat& input, double threshold1, double threshold2) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::Ptr<cv::cuda::CannyEdgeDetector> canny = cv::cuda::createCannyEdgeDetector(
                threshold1, threshold2
            );
            canny->detect(gpuInput, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Canny", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Canny failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::Canny(input, result, threshold1, threshold2);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Canny", elapsed);
    return result;
}

cv::Mat GPUAccelerator::erode(const cv::Mat& input, const cv::Mat& kernel, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createMorphologyFilter(
                cv::MORPH_ERODE, gpuInput.type(), kernel
            );
            
            for (int i = 0; i < iterations; ++i) {
                filter->apply(gpuInput, gpuOutput);
                gpuInput = gpuOutput;
            }
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Erode", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Erode failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::erode(input, result, kernel, cv::Point(-1, -1), iterations);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Erode", elapsed);
    return result;
}

cv::Mat GPUAccelerator::dilate(const cv::Mat& input, const cv::Mat& kernel, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createMorphologyFilter(
                cv::MORPH_DILATE, gpuInput.type(), kernel
            );
            
            for (int i = 0; i < iterations; ++i) {
                filter->apply(gpuInput, gpuOutput);
                gpuInput = gpuOutput;
            }
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Dilate", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Dilate failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::dilate(input, result, kernel, cv::Point(-1, -1), iterations);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Dilate", elapsed);
    return result;
}

cv::Mat GPUAccelerator::morphologyEx(const cv::Mat& input, int op, const cv::Mat& kernel) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::Ptr<cv::cuda::Filter> filter = cv::cuda::createMorphologyFilter(
                op, gpuInput.type(), kernel
            );
            filter->apply(gpuInput, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Morphology Ex", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Morphology Ex failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::morphologyEx(input, result, op, kernel);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Morphology Ex", elapsed);
    return result;
}

cv::Mat GPUAccelerator::add(const cv::Mat& src1, const cv::Mat& src2) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuSrc1, gpuSrc2, gpuOutput;
            gpuSrc1.upload(src1);
            gpuSrc2.upload(src2);
            
            cv::cuda::add(gpuSrc1, gpuSrc2, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Add", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Add failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::add(src1, src2, result);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Add", elapsed);
    return result;
}

cv::Mat GPUAccelerator::subtract(const cv::Mat& src1, const cv::Mat& src2) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuSrc1, gpuSrc2, gpuOutput;
            gpuSrc1.upload(src1);
            gpuSrc2.upload(src2);
            
            cv::cuda::subtract(gpuSrc1, gpuSrc2, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Subtract", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Subtract failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::subtract(src1, src2, result);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Subtract", elapsed);
    return result;
}

cv::Mat GPUAccelerator::multiply(const cv::Mat& src1, const cv::Mat& src2) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuSrc1, gpuSrc2, gpuOutput;
            gpuSrc1.upload(src1);
            gpuSrc2.upload(src2);
            
            cv::cuda::multiply(gpuSrc1, gpuSrc2, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Multiply", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Multiply failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::multiply(src1, src2, result);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Multiply", elapsed);
    return result;
}

cv::Mat GPUAccelerator::absdiff(const cv::Mat& src1, const cv::Mat& src2) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuSrc1, gpuSrc2, gpuOutput;
            gpuSrc1.upload(src1);
            gpuSrc2.upload(src2);
            
            cv::cuda::absdiff(gpuSrc1, gpuSrc2, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU AbsDiff", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU AbsDiff failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::absdiff(src1, src2, result);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU AbsDiff", elapsed);
    return result;
}

cv::Mat GPUAccelerator::equalizeHist(const cv::Mat& input) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat result;
    
#ifdef HAVE_OPENCV_CUDA
    if (cudaAvailable) {
        try {
            cv::cuda::GpuMat gpuInput, gpuOutput;
            gpuInput.upload(input);
            
            cv::cuda::equalizeHist(gpuInput, gpuOutput);
            
            gpuOutput.download(result);
            
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
            logPerformance("GPU Equalize Histogram", elapsed);
            return result;
        } catch (const cv::Exception& e) {
            qDebug() << "GPU Equalize Histogram failed, falling back to CPU:" << e.what();
        }
    }
#endif
    
    // CPU fallback
    cv::equalizeHist(input, result);
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
    logPerformance("CPU Equalize Histogram", elapsed);
    return result;
}
