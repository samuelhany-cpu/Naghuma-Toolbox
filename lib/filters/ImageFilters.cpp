#include "ImageFilters.h"

namespace ImageFilters {

void applyLaplacian(const cv::Mat& src, cv::Mat& dst, 
                    int kernelSize, 
                    double scale, 
                    double delta,
                    int borderType) {
    cv::Mat gray;
    
    // Convert to grayscale if needed
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Apply Laplacian filter
    cv::Laplacian(gray, dst, CV_16S, kernelSize, scale, delta, borderType);
    
    // Convert back to 8-bit
    cv::convertScaleAbs(dst, dst);
}

void applySobel(const cv::Mat& src, cv::Mat& dst,
                char direction,
                int kernelSize,
                double scale,
                double delta,
                int borderType) {
    cv::Mat gray;
    
    // Convert to grayscale if needed
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    if (direction == 'x' || direction == 'X') {
        // Horizontal edges only
        cv::Sobel(gray, grad_x, CV_16S, 1, 0, kernelSize, scale, delta, borderType);
        cv::convertScaleAbs(grad_x, dst);
    }
    else if (direction == 'y' || direction == 'Y') {
        // Vertical edges only
        cv::Sobel(gray, grad_y, CV_16S, 0, 1, kernelSize, scale, delta, borderType);
        cv::convertScaleAbs(grad_y, dst);
    }
    else {
        // Both directions (combined)
        cv::Sobel(gray, grad_x, CV_16S, 1, 0, kernelSize, scale, delta, borderType);
        cv::Sobel(gray, grad_y, CV_16S, 0, 1, kernelSize, scale, delta, borderType);
        
        cv::convertScaleAbs(grad_x, abs_grad_x);
        cv::convertScaleAbs(grad_y, abs_grad_y);
        
        // Combine gradients
        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
    }
}

void applySobelCombined(const cv::Mat& src, 
                       cv::Mat& dst_H, 
                       cv::Mat& dst_V,
                       cv::Mat& dst_D,
                       cv::Mat& dst_S,
                       int kernelSize) {
    cv::Mat gray;
    
    // Convert to grayscale if needed
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Create kernels based on the lab example
    // Horizontal Sobel kernel
    cv::Mat kernel_TH = (cv::Mat_<int>(3, 3) << -1, -2, -1, 
                                                  0,  0,  0, 
                                                  1,  2,  1);
    
    // Vertical Sobel kernel
    cv::Mat kernel_TV = (cv::Mat_<int>(3, 3) << -1, 0, 1,
                                                 -2, 0, 2,
                                                 -1, 0, 1);
    
    // Diagonal Sobel kernel
    cv::Mat kernel_Td = (cv::Mat_<int>(3, 3) <<  2,  1,  0,
                                                  1,  0, -1,
                                                  0, -1, -2);
    
    // Apply filters
    cv::filter2D(gray, dst_H, CV_8UC1, kernel_TH);
    cv::filter2D(gray, dst_V, CV_8UC1, kernel_TV);
    cv::filter2D(gray, dst_D, CV_8UC1, kernel_Td);
    
    // Combine all directions
    cv::addWeighted(dst_H, 1, dst_V, 1, 0, dst_S);
    cv::addWeighted(dst_S, 1, dst_D, 1, 0, dst_S);
    
    // Normalize
    cv::normalize(dst_S, dst_S, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void applyCustomLaplacian(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat gray;
    
    // Convert to grayscale if needed
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Custom Laplacian kernel from the lab example
    cv::Mat kernel_L = (cv::Mat_<int>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
    
    // Apply filter
    cv::filter2D(gray, dst, CV_8UC1, kernel_L);
    
    // Normalize
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void applyGaussianBlur(const cv::Mat& src, cv::Mat& dst,
                       int kernelSize,
                       double sigmaX,
                       double sigmaY) {
    // Ensure kernel size is odd
    if (kernelSize % 2 == 0) {
        kernelSize++;
    }
    
    cv::GaussianBlur(src, dst, cv::Size(kernelSize, kernelSize), sigmaX, sigmaY);
}

void applyMedianBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    // Ensure kernel size is odd
    if (kernelSize % 2 == 0) {
        kernelSize++;
    }
    
    cv::medianBlur(src, dst, kernelSize);
}

void applyBilateralFilter(const cv::Mat& src, cv::Mat& dst,
                          int diameter,
                          double sigmaColor,
                          double sigmaSpace) {
    cv::bilateralFilter(src, dst, diameter, sigmaColor, sigmaSpace);
}

void applyCanny(const cv::Mat& src, cv::Mat& dst,
                double threshold1,
                double threshold2,
                int apertureSize) {
    cv::Mat gray;
    
    // Convert to grayscale if needed
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    cv::Canny(gray, dst, threshold1, threshold2, apertureSize);
}

void applyPrewitt(const cv::Mat& src, cv::Mat& dst, char direction) {
    cv::Mat gray;
    
    // Convert to grayscale if needed
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    // Prewitt kernels
    cv::Mat kernel_x = (cv::Mat_<int>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
    cv::Mat kernel_y = (cv::Mat_<int>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
    
    if (direction == 'x' || direction == 'X') {
        cv::filter2D(gray, dst, CV_8UC1, kernel_x);
    }
    else if (direction == 'y' || direction == 'Y') {
        cv::filter2D(gray, dst, CV_8UC1, kernel_y);
    }
    else {
        cv::filter2D(gray, grad_x, CV_16S, kernel_x);
        cv::filter2D(gray, grad_y, CV_16S, kernel_y);
        
        cv::convertScaleAbs(grad_x, abs_grad_x);
        cv::convertScaleAbs(grad_y, abs_grad_y);
        
        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
    }
}

void applyScharr(const cv::Mat& src, cv::Mat& dst, char direction) {
    cv::Mat gray;
    
    // Convert to grayscale if needed
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    if (direction == 'x' || direction == 'X') {
        cv::Scharr(gray, grad_x, CV_16S, 1, 0);
        cv::convertScaleAbs(grad_x, dst);
    }
    else if (direction == 'y' || direction == 'Y') {
        cv::Scharr(gray, grad_y, CV_16S, 0, 1);
        cv::convertScaleAbs(grad_y, dst);
    }
    else {
        cv::Scharr(gray, grad_x, CV_16S, 1, 0);
        cv::Scharr(gray, grad_y, CV_16S, 0, 1);
        
        cv::convertScaleAbs(grad_x, abs_grad_x);
        cv::convertScaleAbs(grad_y, abs_grad_y);
        
        cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
    }
}

void applyCustomKernel(const cv::Mat& src, cv::Mat& dst, 
                       const cv::Mat& kernel,
                       bool normalize) {
    cv::filter2D(src, dst, -1, kernel);
    
    if (normalize) {
        cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
    }
}

void applySharpen(const cv::Mat& src, cv::Mat& dst, double amount) {
    cv::Mat blurred;
    cv::GaussianBlur(src, blurred, cv::Size(0, 0), 3);
    
    cv::addWeighted(src, 1.0 + amount, blurred, -amount, 0, dst);
}

void applyTraditionalFilter(const cv::Mat& src, cv::Mat& dst) {
    // Traditional filter kernel (all 1s)
    cv::Mat kernel_T = (cv::Mat_<float>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
    kernel_T = kernel_T / 9.0;  // Normalize
    
    cv::filter2D(src, dst, CV_8UC1, kernel_T);
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void applyPyramidalFilter(const cv::Mat& src, cv::Mat& dst) {
    // Pyramidal filter kernel
    cv::Mat kernel_p = (cv::Mat_<float>(5, 5) << 
        1, 2, 3, 2, 1,
        2, 4, 6, 4, 2,
        3, 6, 9, 6, 3,
        2, 4, 6, 4, 2,
        1, 2, 3, 2, 1);
    
    // Normalize kernel
    double sum = cv::sum(kernel_p)[0];
    kernel_p = kernel_p / sum;
    
    cv::filter2D(src, dst, CV_8UC1, kernel_p);
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void applyCircularFilter(const cv::Mat& src, cv::Mat& dst) {
    // Circular filter kernel (5x5)
    cv::Mat kernel_c = (cv::Mat_<float>(5, 5) << 
        0, 1, 1, 1, 0,
        1, 1, 1, 1, 1,
        1, 1, 0, 1, 1,
        1, 1, 1, 1, 1,
        0, 1, 1, 1, 0);
    
    // Normalize kernel
    double sum = cv::sum(kernel_c)[0];
    kernel_c = kernel_c / sum;
    
    cv::filter2D(src, dst, CV_8UC1, kernel_c);
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void applyConeFilter(const cv::Mat& src, cv::Mat& dst) {
    // Cone filter kernel (5x5)
    cv::Mat kernel_co = (cv::Mat_<float>(5, 5) << 
        0, 0, 1, 0, 0,
        0, 2, 2, 2, 0,
        1, 2, 5, 2, 1,
        0, 2, 2, 2, 0,
        0, 0, 1, 0, 0);
    
    // Normalize kernel
    double sum = cv::sum(kernel_co)[0];
    kernel_co = kernel_co / sum;
    
    cv::filter2D(src, dst, CV_8UC1, kernel_co);
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
}

} // namespace ImageFilters
