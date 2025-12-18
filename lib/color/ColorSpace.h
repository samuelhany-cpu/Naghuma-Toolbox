#pragma once

#include <opencv2/opencv.hpp>
#include <QString>
#include <vector>

namespace ColorSpace {

// ============================================================================
// RGB <-> HSV Conversions
// ============================================================================

/**
 * @brief Convert RGB image to HSV color space
 * @param src Source RGB image (CV_8UC3)
 * @param dst Destination HSV image (CV_8UC3)
 * 
 * HSV ranges:
 * - H: 0-179 (OpenCV uses 0-180 for 8-bit)
 * - S: 0-255
 * - V: 0-255
 */
void RGBtoHSV(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Convert HSV image to RGB color space
 * @param src Source HSV image (CV_8UC3)
 * @param dst Destination RGB image (CV_8UC3)
 */
void HSVtoRGB(const cv::Mat& src, cv::Mat& dst);

// ============================================================================
// RGB <-> LAB Conversions
// ============================================================================

/**
 * @brief Convert RGB image to LAB color space (CIE L*a*b*)
 * @param src Source RGB image (CV_8UC3)
 * @param dst Destination LAB image (CV_8UC3)
 * 
 * LAB ranges (OpenCV 8-bit):
 * - L: 0-255 (lightness)
 * - a: 0-255 (green-red)
 * - b: 0-255 (blue-yellow)
 */
void RGBtoLAB(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Convert LAB image to RGB color space
 * @param src Source LAB image (CV_8UC3)
 * @param dst Destination RGB image (CV_8UC3)
 */
void LABtoRGB(const cv::Mat& src, cv::Mat& dst);

// ============================================================================
// RGB <-> YCbCr Conversions
// ============================================================================

/**
 * @brief Convert RGB image to YCbCr color space
 * @param src Source RGB image (CV_8UC3)
 * @param dst Destination YCbCr image (CV_8UC3)
 * 
 * YCbCr ranges:
 * - Y:  16-235 (luminance)
 * - Cb: 16-240 (blue-difference)
 * - Cr: 16-240 (red-difference)
 */
void RGBtoYCbCr(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Convert YCbCr image to RGB color space
 * @param src Source YCbCr image (CV_8UC3)
 * @param dst Destination RGB image (CV_8UC3)
 */
void YCbCrtoRGB(const cv::Mat& src, cv::Mat& dst);

// ============================================================================
// RGB <-> HSI Conversions
// ============================================================================

/**
 * @brief Convert RGB image to HSI color space
 * @param src Source RGB image (CV_8UC3)
 * @param dst Destination HSI image (CV_8UC3)
 * 
 * HSI ranges (normalized to 0-255):
 * - H: 0-255 (hue)
 * - S: 0-255 (saturation)
 * - I: 0-255 (intensity)
 */
void RGBtoHSI(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Convert HSI image to RGB color space
 * @param src Source HSI image (CV_8UC3)
 * @param dst Destination RGB image (CV_8UC3)
 */
void HSItoRGB(const cv::Mat& src, cv::Mat& dst);

// ============================================================================
// RGB <-> Grayscale Conversions
// ============================================================================

/**
 * @brief Convert RGB to Grayscale (weighted luminance)
 * @param src Source RGB image (CV_8UC3)
 * @param dst Destination grayscale image (CV_8UC1)
 */
void RGBtoGray(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Convert Grayscale to RGB (duplicate channels)
 * @param src Source grayscale image (CV_8UC1)
 * @param dst Destination RGB image (CV_8UC3)
 */
void GraytoRGB(const cv::Mat& src, cv::Mat& dst);

// ============================================================================
// Channel Extraction
// ============================================================================

/**
 * @brief Extract individual channels from an image
 * @param src Source multi-channel image
 * @param channels Vector to store extracted channels
 */
void extractChannels(const cv::Mat& src, std::vector<cv::Mat>& channels);

/**
 * @brief Merge individual channels into a multi-channel image
 * @param channels Vector of channels to merge
 * @param dst Destination multi-channel image
 */
void mergeChannels(const std::vector<cv::Mat>& channels, cv::Mat& dst);

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * @brief Get color space name from OpenCV color code
 * @param code OpenCV color conversion code
 * @return Human-readable color space name
 */
QString getColorSpaceName(int code);

/**
 * @brief Get channel names for a color space
 * @param colorSpace Color space identifier (e.g., "HSV", "LAB", "YCbCr")
 * @return Vector of channel names
 */
std::vector<QString> getChannelNames(const QString& colorSpace);

} // namespace ColorSpace
