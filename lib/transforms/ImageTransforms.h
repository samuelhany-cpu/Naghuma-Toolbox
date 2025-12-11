#ifndef IMAGETRANSFORMS_H
#define IMAGETRANSFORMS_H

#include <opencv2/opencv.hpp>

namespace ImageTransforms {

/**
 * @brief Apply translation transformation
 * @param src Source image
 * @param dst Destination image
 * @param tx Translation in X direction (pixels)
 * @param ty Translation in Y direction (pixels)
 */
void translate(const cv::Mat& src, cv::Mat& dst, int tx, int ty);

/**
 * @brief Apply rotation transformation
 * @param src Source image
 * @param dst Destination image
 * @param angle Rotation angle in degrees (positive = counter-clockwise)
 * @param scale Scale factor (default: 1.0)
 */
void rotate(const cv::Mat& src, cv::Mat& dst, double angle, double scale = 1.0);

/**
 * @brief Apply rotation around a custom center point
 * @param src Source image
 * @param dst Destination image
 * @param angle Rotation angle in degrees
 * @param centerX Center X coordinate
 * @param centerY Center Y coordinate
 * @param scale Scale factor (default: 1.0)
 */
void rotateAroundPoint(const cv::Mat& src, cv::Mat& dst, 
                       double angle, int centerX, int centerY, 
                       double scale = 1.0);

/**
 * @brief Apply skew/shear transformation
 * @param src Source image
 * @param dst Destination image
 * @param shearX Shear factor in X direction
 * @param shearY Shear factor in Y direction
 */
void skew(const cv::Mat& src, cv::Mat& dst, float shearX, float shearY);

/**
 * @brief Apply affine transformation using three point pairs
 * @param src Source image
 * @param dst Destination image
 * @param srcPoints Array of 3 source points
 * @param dstPoints Array of 3 destination points
 */
void affineTransform(const cv::Mat& src, cv::Mat& dst,
                     const cv::Point2f srcPoints[3],
                     const cv::Point2f dstPoints[3]);

/**
 * @brief Apply perspective transformation using four point pairs
 * @param src Source image
 * @param dst Destination image
 * @param srcPoints Array of 4 source points
 * @param dstPoints Array of 4 destination points
 */
void perspectiveTransform(const cv::Mat& src, cv::Mat& dst,
                          const cv::Point2f srcPoints[4],
                          const cv::Point2f dstPoints[4]);

/**
 * @brief Zoom/scale image
 * @param src Source image
 * @param dst Destination image
 * @param scale Scale factor (>1.0 = zoom in, <1.0 = zoom out)
 * @param interpolation Interpolation method (default: INTER_LINEAR)
 */
void zoom(const cv::Mat& src, cv::Mat& dst, double scale,
          int interpolation = cv::INTER_LINEAR);

/**
 * @brief Resize image to specific dimensions
 * @param src Source image
 * @param dst Destination image
 * @param width New width
 * @param height New height
 * @param interpolation Interpolation method (default: INTER_LINEAR)
 */
void resize(const cv::Mat& src, cv::Mat& dst, 
            int width, int height,
            int interpolation = cv::INTER_LINEAR);

/**
 * @brief Flip image horizontally
 * @param src Source image
 * @param dst Destination image
 */
void flipHorizontal(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Flip image vertically
 * @param src Source image
 * @param dst Destination image
 */
void flipVertical(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Flip image both horizontally and vertically
 * @param src Source image
 * @param dst Destination image
 */
void flipBoth(const cv::Mat& src, cv::Mat& dst);

/**
 * @brief Crop image to specified region
 * @param src Source image
 * @param dst Destination image
 * @param x Starting X coordinate
 * @param y Starting Y coordinate
 * @param width Width of crop region
 * @param height Height of crop region
 */
void crop(const cv::Mat& src, cv::Mat& dst, 
          int x, int y, int width, int height);

/**
 * @brief Crop image using ROI (Region of Interest)
 * @param src Source image
 * @param dst Destination image
 * @param roi Rectangle defining the region of interest
 */
void cropROI(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi);

/**
 * @brief Warp image using a custom transformation matrix
 * @param src Source image
 * @param dst Destination image
 * @param transformMatrix 2x3 transformation matrix for affine transform
 */
void warpAffine(const cv::Mat& src, cv::Mat& dst, const cv::Mat& transformMatrix);

/**
 * @brief Warp image using perspective transformation matrix
 * @param src Source image
 * @param dst Destination image
 * @param transformMatrix 3x3 perspective transformation matrix
 */
void warpPerspective(const cv::Mat& src, cv::Mat& dst, const cv::Mat& transformMatrix);

} // namespace ImageTransforms

#endif // IMAGETRANSFORMS_H
