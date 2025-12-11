#ifndef HISTOGRAMOPERATIONS_H
#define HISTOGRAMOPERATIONS_H

#include <opencv2/opencv.hpp>
#include <vector>

namespace HistogramOperations {

/**
 * @brief Calculate histogram for grayscale image
 * @param src Source image (grayscale)
 * @param histogram Output histogram (256 bins)
 * @return Maximum frequency value
 */
int calculateHistogram(const cv::Mat& src, std::vector<int>& histogram);

/**
 * @brief Calculate histogram for color image (BGR)
 * @param src Source image (BGR)
 * @param histB Blue channel histogram
 * @param histG Green channel histogram
 * @param histR Red channel histogram
 * @return Maximum frequency value across all channels
 */
int calculateColorHistogram(const cv::Mat& src,
                            std::vector<int>& histB,
                            std::vector<int>& histG,
                            std::vector<int>& histR);

/**
 * @brief Apply histogram equalization to enhance contrast
 * @param src Source image
 * @param dst Destination image
 * @param preserveColor If true, equalize in YCrCb space to preserve colors
 */
void equalizeHistogram(const cv::Mat& src, cv::Mat& dst, bool preserveColor = true);

/**
 * @brief Apply Otsu's automatic thresholding method
 * @param src Source image (grayscale or will be converted)
 * @param dst Destination binary image
 * @return Optimal threshold value found by Otsu's method
 */
double otsuThreshold(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Apply adaptive histogram equalization (CLAHE)
 * @param src Source image
 * @param dst Destination image
 * @param clipLimit Threshold for contrast limiting (default: 2.0)
 * @param tileSize Size of grid for histogram equalization (default: 8x8)
 */
void adaptiveHistogramEqualization(const cv::Mat& src, cv::Mat& dst,
                                  double clipLimit = 2.0,
                                  int tileSize = 8);

/**
 * @brief Apply manual threshold to create binary image
 * @param src Source image (grayscale or will be converted)
 * @param dst Destination binary image
 * @param threshold Threshold value (0-255)
 * @param maxValue Maximum value for pixels above threshold
 * @param thresholdType Type of thresholding (THRESH_BINARY, THRESH_BINARY_INV, etc.)
 */
void applyThreshold(const cv::Mat& src, cv::Mat& dst,
                   double threshold = 128,
                   double maxValue = 255,
                   int thresholdType = cv::THRESH_BINARY);

/**
 * @brief Apply adaptive thresholding
 * @param src Source image (grayscale)
 * @param dst Destination binary image
 * @param maxValue Maximum value for pixels
 * @param method Adaptive method (ADAPTIVE_THRESH_MEAN_C or ADAPTIVE_THRESH_GAUSSIAN_C)
 * @param type Threshold type (THRESH_BINARY or THRESH_BINARY_INV)
 * @param blockSize Size of pixel neighborhood
 * @param C Constant subtracted from mean or weighted mean
 */
void adaptiveThreshold(const cv::Mat& src, cv::Mat& dst,
                      double maxValue = 255,
                      int method = cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                      int type = cv::THRESH_BINARY,
                      int blockSize = 11,
                      double C = 2);

/**
 * @brief Calculate image statistics from histogram
 * @param src Source image
 * @param mean Output: mean pixel value
 * @param stddev Output: standard deviation
 * @param minVal Output: minimum pixel value
 * @param maxVal Output: maximum pixel value
 */
void calculateStatistics(const cv::Mat& src,
                        double& mean,
                        double& stddev,
                        double& minVal,
                        double& maxVal);

/**
 * @brief Stretch histogram to full range (0-255)
 * @param src Source image
 * @param dst Destination image
 */
void histogramStretching(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Match histogram of source to reference image
 * @param src Source image to modify
 * @param ref Reference image with desired histogram
 * @param dst Destination image
 */
void histogramMatching(const cv::Mat& src, const cv::Mat& ref, cv::Mat& dst);

/**
 * @brief Apply gamma correction
 * @param src Source image
 * @param dst Destination image
 * @param gamma Gamma value (>1 = darker, <1 = brighter)
 */
void gammaCorrection(const cv::Mat& src, cv::Mat& dst, double gamma);

/**
 * @brief Adjust brightness and contrast
 * @param src Source image
 * @param dst Destination image
 * @param alpha Contrast control (1.0-3.0, 1.0 = no change)
 * @param beta Brightness control (-100 to 100, 0 = no change)
 */
void adjustBrightnessContrast(const cv::Mat& src, cv::Mat& dst,
                             double alpha = 1.0,
                             double beta = 0);

} // namespace HistogramOperations

#endif // HISTOGRAMOPERATIONS_H
