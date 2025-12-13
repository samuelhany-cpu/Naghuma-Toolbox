#ifndef IMAGEFILTERS_H
#define IMAGEFILTERS_H

#include <opencv2/opencv.hpp>

namespace ImageFilters {

/**
 * @brief Apply Laplacian filter for edge detection
 * @param src Source image
 * @param dst Destination image
 * @param kernelSize Size of the kernel (must be odd, default: 1)
 * @param scale Scale factor (default: 1.0)
 * @param delta Delta value added to results (default: 0.0)
 * @param borderType Border handling type (default: BORDER_DEFAULT)
 */
void applyLaplacian(const cv::Mat& src, cv::Mat& dst, 
                    int kernelSize = 1, 
                    double scale = 1.0, 
                    double delta = 0.0,
                    int borderType = cv::BORDER_DEFAULT);

/**
 * @brief Apply Sobel filter for edge detection
 * @param src Source image
 * @param dst Destination image  
 * @param direction Direction of derivative: 'x', 'y', or 'both'
 * @param kernelSize Size of the Sobel kernel (1, 3, 5, or 7)
 * @param scale Scale factor (default: 1.0)
 * @param delta Delta value added to results (default: 0.0)
 * @param borderType Border handling type (default: BORDER_DEFAULT)
 */
void applySobel(const cv::Mat& src, cv::Mat& dst,
                char direction = 'b',  // 'x', 'y', or 'b' (both)
                int kernelSize = 3,
                double scale = 1.0,
                double delta = 0.0,
                int borderType = cv::BORDER_DEFAULT);

/**
 * @brief Apply combined Sobel filter (horizontal + vertical + diagonal)
 * @param src Source image
 * @param dst_H Horizontal Sobel result
 * @param dst_V Vertical Sobel result
 * @param dst_D Diagonal Sobel result
 * @param dst_S Combined Sobel result
 * @param kernelSize Size of the Sobel kernel (default: 3)
 */
void applySobelCombined(const cv::Mat& src, 
                       cv::Mat& dst_H, 
                       cv::Mat& dst_V,
                       cv::Mat& dst_D,
                       cv::Mat& dst_S,
                       int kernelSize = 3);

/**
 * @brief Apply custom Laplacian kernel from the lab example
 * @param src Source image
 * @param dst Destination image
 */
void applyCustomLaplacian(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply Gaussian blur filter
 * @param src Source image
 * @param dst Destination image
 * @param kernelSize Size of the Gaussian kernel (must be odd)
 * @param sigmaX Standard deviation in X direction
 * @param sigmaY Standard deviation in Y direction (default: 0 = same as sigmaX)
 */
void applyGaussianBlur(const cv::Mat& src, cv::Mat& dst,
                       int kernelSize = 5,
                       double sigmaX = 0,
                       double sigmaY = 0);

/**
 * @brief Apply median blur filter
 * @param src Source image
 * @param dst Destination image
 * @param kernelSize Size of the median kernel (must be odd)
 */
void applyMedianBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5);

/**
 * @brief Apply bilateral filter (edge-preserving smoothing)
 * @param src Source image
 * @param dst Destination image
 * @param diameter Diameter of pixel neighborhood
 * @param sigmaColor Filter sigma in the color space
 * @param sigmaSpace Filter sigma in the coordinate space
 */
void applyBilateralFilter(const cv::Mat& src, cv::Mat& dst,
                          int diameter = 9,
                          double sigmaColor = 75,
                          double sigmaSpace = 75);

/**
 * @brief Apply Canny edge detection
 * @param src Source image (grayscale)
 * @param dst Destination image
 * @param threshold1 First threshold for hysteresis
 * @param threshold2 Second threshold for hysteresis
 * @param apertureSize Aperture size for Sobel operator
 */
void applyCanny(const cv::Mat& src, cv::Mat& dst,
                double threshold1 = 100,
                double threshold2 = 200,
                int apertureSize = 3);

/**
 * @brief Apply Prewitt filter for edge detection
 * @param src Source image
 * @param dst Destination image
 * @param direction Direction: 'x', 'y', or 'b' (both)
 */
void applyPrewitt(const cv::Mat& src, cv::Mat& dst, char direction = 'b');

/**
 * @brief Apply Scharr filter for edge detection (more accurate than Sobel)
 * @param src Source image
 * @param dst Destination image
 * @param direction Direction: 'x', 'y', or 'b' (both)
 */
void applyScharr(const cv::Mat& src, cv::Mat& dst, char direction = 'b');

/**
 * @brief Apply custom convolution kernel
 * @param src Source image
 * @param dst Destination image
 * @param kernel Custom kernel matrix
 * @param normalize Whether to normalize the result
 */
void applyCustomKernel(const cv::Mat& src, cv::Mat& dst, 
                       const cv::Mat& kernel,
                       bool normalize = true);

/**
 * @brief Sharpen image using unsharp masking
 * @param src Source image
 * @param dst Destination image
 * @param amount Strength of sharpening effect (0.0 - 2.0)
 */
void applySharpen(const cv::Mat& src, cv::Mat& dst, double amount = 1.0);

/**
 * @brief Apply Traditional filter (Laplacian with all 1s kernel)
 * @param src Source image
 * @param dst Destination image
 */
void applyTraditionalFilter(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply Pyramidal filter with custom kernel
 * @param src Source image
 * @param dst Destination image
 */
void applyPyramidalFilter(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply Circular filter with custom kernel
 * @param src Source image
 * @param dst Destination image
 */
void applyCircularFilter(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply Cone filter with custom kernel
 * @param src Source image
 * @param dst Destination image
 */
void applyConeFilter(const cv::Mat& src, cv::Mat& dst);

// ============================================================================
// PHASE 13: BASIC EDGE DETECTORS
// ============================================================================

/**
 * @brief Apply Prewitt edge detector (combined X and Y)
 * @param src Source image
 * @param dst Destination image
 */
void applyPrewittEdge(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply Prewitt edge detector in X direction
 * @param src Source image
 * @param dst Destination image
 */
void applyPrewittX(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply Prewitt edge detector in Y direction
 * @param src Source image
 * @param dst Destination image
 */
void applyPrewittY(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply Roberts Cross operator for edge detection
 * @param src Source image
 * @param dst Destination image
 */
void applyRobertsCross(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply Laplacian of Gaussian (LoG) edge detector
 * @param src Source image
 * @param dst Destination image
 * @param kernelSize Gaussian kernel size (default: 5)
 * @param sigma Gaussian sigma (default: 1.4)
 */
void applyLoG(const cv::Mat& src, cv::Mat& dst, int kernelSize = 5, double sigma = 1.4);

/**
 * @brief Apply Difference of Gaussians (DoG) edge detector
 * @param src Source image
 * @param dst Destination image
 * @param kernelSize1 First Gaussian kernel size (default: 5)
 * @param sigma1 First Gaussian sigma (default: 1.0)
 * @param kernelSize2 Second Gaussian kernel size (default: 9)
 * @param sigma2 Second Gaussian sigma (default: 2.0)
 */
void applyDoG(const cv::Mat& src, cv::Mat& dst, 
              int kernelSize1 = 5, double sigma1 = 1.0,
              int kernelSize2 = 9, double sigma2 = 2.0);

/**
 * @brief Calculate edge magnitude and direction from gradient images
 * @param gradX Gradient in X direction
 * @param gradY Gradient in Y direction
 * @param magnitude Output magnitude image
 * @param direction Output direction image (in radians)
 */
void calculateEdgeMagnitudeDirection(const cv::Mat& gradX, const cv::Mat& gradY,
                                    cv::Mat& magnitude, cv::Mat& direction);

} // namespace ImageFilters

#endif // IMAGEFILTERS_H
