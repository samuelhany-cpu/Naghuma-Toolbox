#include "ImageTransforms.h"

namespace ImageTransforms {

void translate(const cv::Mat& src, cv::Mat& dst, int tx, int ty) {
    cv::Mat M = (cv::Mat_<float>(2, 3) << 1, 0, tx, 0, 1, ty);
    cv::warpAffine(src, dst, M, src.size());
}

void rotate(const cv::Mat& src, cv::Mat& dst, double angle, double scale) {
    cv::Point2f center(src.cols / 2.0f, src.rows / 2.0f);
    cv::Mat M = cv::getRotationMatrix2D(center, angle, scale);
    cv::warpAffine(src, dst, M, src.size());
}

void rotateAroundPoint(const cv::Mat& src, cv::Mat& dst, 
                       double angle, int centerX, int centerY, 
                       double scale) {
    cv::Point2f center(centerX, centerY);
    cv::Mat M = cv::getRotationMatrix2D(center, angle, scale);
    cv::warpAffine(src, dst, M, src.size());
}

void skew(const cv::Mat& src, cv::Mat& dst, float shearX, float shearY) {
    int rows = src.rows;
    int cols = src.cols;
    
    cv::Point2f srcTri[3];
    cv::Point2f dstTri[3];
    
    srcTri[0] = cv::Point2f(0, 0);
    srcTri[1] = cv::Point2f(cols - 1.0f, 0);
    srcTri[2] = cv::Point2f(0, rows - 1.0f);
    
    dstTri[0] = cv::Point2f(0, 0);
    dstTri[1] = cv::Point2f(cols - 1.0f + shearX * rows, shearY * cols);
    dstTri[2] = cv::Point2f(shearX * rows, rows - 1.0f + shearY * cols);
    
    cv::Mat warpMat = cv::getAffineTransform(srcTri, dstTri);
    cv::warpAffine(src, dst, warpMat, src.size());
}

void affineTransform(const cv::Mat& src, cv::Mat& dst,
                     const cv::Point2f srcPoints[3],
                     const cv::Point2f dstPoints[3]) {
    cv::Mat warpMat = cv::getAffineTransform(srcPoints, dstPoints);
    cv::warpAffine(src, dst, warpMat, src.size());
}

void perspectiveTransform(const cv::Mat& src, cv::Mat& dst,
                          const cv::Point2f srcPoints[4],
                          const cv::Point2f dstPoints[4]) {
    cv::Mat warpMat = cv::getPerspectiveTransform(srcPoints, dstPoints);
    cv::warpPerspective(src, dst, warpMat, src.size());
}

void zoom(const cv::Mat& src, cv::Mat& dst, double scale, int interpolation) {
    cv::resize(src, dst, cv::Size(), scale, scale, interpolation);
}

void resize(const cv::Mat& src, cv::Mat& dst, 
            int width, int height, int interpolation) {
    cv::resize(src, dst, cv::Size(width, height), 0, 0, interpolation);
}

void flipHorizontal(const cv::Mat& src, cv::Mat& dst) {
    cv::flip(src, dst, 1);
}

void flipVertical(const cv::Mat& src, cv::Mat& dst) {
    cv::flip(src, dst, 0);
}

void flipBoth(const cv::Mat& src, cv::Mat& dst) {
    cv::flip(src, dst, -1);
}

void crop(const cv::Mat& src, cv::Mat& dst, 
          int x, int y, int width, int height) {
    // Validate crop region
    x = std::max(0, std::min(x, src.cols - 1));
    y = std::max(0, std::min(y, src.rows - 1));
    width = std::min(width, src.cols - x);
    height = std::min(height, src.rows - y);
    
    cv::Rect roi(x, y, width, height);
    dst = src(roi).clone();
}

void cropROI(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi) {
    dst = src(roi).clone();
}

void warpAffine(const cv::Mat& src, cv::Mat& dst, const cv::Mat& transformMatrix) {
    cv::warpAffine(src, dst, transformMatrix, src.size());
}

void warpPerspective(const cv::Mat& src, cv::Mat& dst, const cv::Mat& transformMatrix) {
    cv::warpPerspective(src, dst, transformMatrix, src.size());
}

} // namespace ImageTransforms
