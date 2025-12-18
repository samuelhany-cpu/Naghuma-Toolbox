#include "ImageProcessor.h"
#include <QImage>

void ImageProcessor::convertToGrayscale(const cv::Mat& src, cv::Mat& dst) {
    if (src.channels() == 3) {
        cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
    } else {
        dst = src.clone();
    }
}

void ImageProcessor::applyBinaryThreshold(const cv::Mat& src, cv::Mat& dst, int threshold) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    cv::threshold(gray, dst, threshold, 255, cv::THRESH_BINARY);
}

void ImageProcessor::applyGaussianBlur(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    // Ensure kernel size is odd
    if (kernelSize % 2 == 0) kernelSize++;
    cv::GaussianBlur(src, dst, cv::Size(kernelSize, kernelSize), 0);
}

void ImageProcessor::detectEdges(const cv::Mat& src, cv::Mat& dst, double threshold1, double threshold2) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    cv::Canny(gray, dst, threshold1, threshold2);
}

void ImageProcessor::invertColors(const cv::Mat& src, cv::Mat& dst) {
    dst = 255 - src;
}

void ImageProcessor::equalizeHistogram(const cv::Mat& src, cv::Mat& dst) {
    if (src.channels() == 3) {
        cv::Mat ycrcb;
        cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);
        
        std::vector<cv::Mat> channels;
        cv::split(ycrcb, channels);
        cv::equalizeHist(channels[0], channels[0]);
        cv::merge(channels, ycrcb);
        
        cv::cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);
    } else {
        cv::equalizeHist(src, dst);
    }
}

void ImageProcessor::applyOtsuThreshold(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void ImageProcessor::adjustBrightnessContrast(const cv::Mat& src, cv::Mat& dst, int brightness, int contrast) {
    // Convert brightness from [-100, 100] to actual value
    double alpha = (contrast + 100.0) / 100.0;  // Contrast gain (0.0 to 2.0)
    int beta = brightness;                       // Brightness offset (-100 to 100)
    
    // Apply the formula: dst(x,y) = alpha * src(x,y) + beta
    src.convertTo(dst, -1, alpha, beta);
}

void ImageProcessor::applyErosion(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, 
        cv::Size(kernelSize, kernelSize));
    cv::erode(src, dst, element);
}

void ImageProcessor::applyDilation(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, 
        cv::Size(kernelSize, kernelSize));
    cv::dilate(src, dst, element);
}

void ImageProcessor::applyOpening(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, 
        cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(src, dst, cv::MORPH_OPEN, element);
}

void ImageProcessor::applyClosing(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, 
        cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(src, dst, cv::MORPH_CLOSE, element);
}

void ImageProcessor::applyMorphGradient(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, 
        cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(src, dst, cv::MORPH_GRADIENT, element);
}

void ImageProcessor::applyFFT(const cv::Mat& src, cv::Mat& magnitude, cv::Mat& phase) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Expand to optimal size
    cv::Mat padded;
    int m = cv::getOptimalDFTSize(gray.rows);
    int n = cv::getOptimalDFTSize(gray.cols);
    cv::copyMakeBorder(gray, padded, 0, m - gray.rows, 0, n - gray.cols, 
                      cv::BORDER_CONSTANT, cv::Scalar::all(0));
    
    // Prepare for DFT
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complexImg;
    cv::merge(planes, 2, complexImg);
    
    // Perform DFT
    cv::dft(complexImg, complexImg);
    
    // Split into magnitude and phase
    cv::split(complexImg, planes);
    cv::magnitude(planes[0], planes[1], magnitude);
    cv::phase(planes[0], planes[1], phase);
    
    // Switch to logarithmic scale for visualization
    magnitude += cv::Scalar::all(1);
    cv::log(magnitude, magnitude);
    cv::normalize(magnitude, magnitude, 0, 255, cv::NORM_MINMAX);
    magnitude.convertTo(magnitude, CV_8U);
}

void ImageProcessor::applyLowPassFilter(const cv::Mat& src, cv::Mat& dst, int radius) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Expand to optimal size
    cv::Mat padded;
    int m = cv::getOptimalDFTSize(gray.rows);
    int n = cv::getOptimalDFTSize(gray.cols);
    cv::copyMakeBorder(gray, padded, 0, m - gray.rows, 0, n - gray.cols, 
                      cv::BORDER_CONSTANT, cv::Scalar::all(0));
    
    // Perform DFT
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complexImg;
    cv::merge(planes, 2, complexImg);
    cv::dft(complexImg, complexImg);
    
    // Create low-pass filter mask
    cv::Mat mask = cv::Mat::zeros(complexImg.size(), CV_32F);
    cv::Point center(mask.cols / 2, mask.rows / 2);
    cv::circle(mask, center, radius, cv::Scalar(1), -1);
    
    // Apply filter in frequency domain
    cv::Mat maskPlanes[] = {mask, mask};
    cv::Mat maskComplex;
    cv::merge(maskPlanes, 2, maskComplex);
    cv::mulSpectrums(complexImg, maskComplex, complexImg, 0);
    
    // Inverse DFT
    cv::idft(complexImg, complexImg);
    cv::split(complexImg, planes);
    cv::normalize(planes[0], dst, 0, 255, cv::NORM_MINMAX);
    dst.convertTo(dst, CV_8U);
    dst = dst(cv::Rect(0, 0, gray.cols, gray.rows));
}

void ImageProcessor::applyHighPassFilter(const cv::Mat& src, cv::Mat& dst, int radius) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Expand to optimal size
    cv::Mat padded;
    int m = cv::getOptimalDFTSize(gray.rows);
    int n = cv::getOptimalDFTSize(gray.cols);
    cv::copyMakeBorder(gray, padded, 0, m - gray.rows, 0, n - gray.cols, 
                      cv::BORDER_CONSTANT, cv::Scalar::all(0));
    
    // Perform DFT
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complexImg;
    cv::merge(planes, 2, complexImg);
    cv::dft(complexImg, complexImg);
    
    // Create high-pass filter mask (inverse of low-pass)
    cv::Mat mask = cv::Mat::ones(complexImg.size(), CV_32F);
    cv::Point center(mask.cols / 2, mask.rows / 2);
    cv::circle(mask, center, radius, cv::Scalar(0), -1);
    
    // Apply filter in frequency domain
    cv::Mat maskPlanes[] = {mask, mask};
    cv::Mat maskComplex;
    cv::merge(maskPlanes, 2, maskComplex);
    cv::mulSpectrums(complexImg, maskComplex, complexImg, 0);
    
    // Inverse DFT
    cv::idft(complexImg, complexImg);
    cv::split(complexImg, planes);
    cv::normalize(planes[0], dst, 0, 255, cv::NORM_MINMAX);
    dst.convertTo(dst, CV_8U);
    dst = dst(cv::Rect(0, 0, gray.cols, gray.rows));
}

void ImageProcessor::flipHorizontal(const cv::Mat& src, cv::Mat& dst) {
    cv::flip(src, dst, 1);
}

void ImageProcessor::flipVertical(const cv::Mat& src, cv::Mat& dst) {
    cv::flip(src, dst, 0);
}

void ImageProcessor::flipBoth(const cv::Mat& src, cv::Mat& dst) {
    cv::flip(src, dst, -1);
}

void ImageProcessor::applySkew(const cv::Mat& src, cv::Mat& dst) {
    applySkew(src, dst, 0.3, 0.0);
}

void ImageProcessor::applySkew(const cv::Mat& src, cv::Mat& dst, double skewX, double skewY) {
    int rows = src.rows;
    int cols = src.cols;
    
    cv::Point2f srcTri[3];
    cv::Point2f dstTri[3];
    
    srcTri[0] = cv::Point2f(0, 0);
    srcTri[1] = cv::Point2f(cols - 1.0f, 0);
    srcTri[2] = cv::Point2f(0, rows - 1.0f);
    
    dstTri[0] = cv::Point2f(cols * skewX, rows * skewY);
    dstTri[1] = cv::Point2f(cols - 1.0f + cols * skewX, rows * skewY);
    dstTri[2] = cv::Point2f(0, rows - 1.0f);
    
    cv::Mat warpMat = cv::getAffineTransform(srcTri, dstTri);
    cv::warpAffine(src, dst, warpMat, src.size());
}

void ImageProcessor::translate(const cv::Mat& src, cv::Mat& dst, int tx, int ty) {
    cv::Mat M = (cv::Mat_<float>(2, 3) << 1, 0, tx, 0, 1, ty);
    cv::warpAffine(src, dst, M, src.size());
}

void ImageProcessor::rotate(const cv::Mat& src, cv::Mat& dst, double angle) {
    cv::Point2f center(src.cols / 2.0f, src.rows / 2.0f);
    cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(src, dst, M, src.size());
}

void ImageProcessor::zoom(const cv::Mat& src, cv::Mat& dst, double scale) {
    cv::resize(src, dst, cv::Size(), scale, scale, cv::INTER_LINEAR);
}

// ============================================================================
// AUTO ENHANCEMENT ALGORITHMS
// ============================================================================

void ImageProcessor::applyAdaptiveHistogramEqualization(const cv::Mat& src, cv::Mat& dst) {
    if (src.channels() == 3) {
        // Convert to LAB color space for better results
        cv::Mat lab;
        cv::cvtColor(src, lab, cv::COLOR_BGR2Lab);
        
        // Split channels
        std::vector<cv::Mat> labChannels;
        cv::split(lab, labChannels);
        
        // Apply CLAHE (Contrast Limited Adaptive Histogram Equalization) to L channel
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
        clahe->setClipLimit(2.0);
        clahe->setTilesGridSize(cv::Size(8, 8));
        clahe->apply(labChannels[0], labChannels[0]);
        
        // Merge channels back
        cv::merge(labChannels, lab);
        
        // Convert back to BGR
        cv::cvtColor(lab, dst, cv::COLOR_Lab2BGR);
    } else {
        // For grayscale images, apply CLAHE directly
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
        clahe->setClipLimit(2.0);
        clahe->setTilesGridSize(cv::Size(8, 8));
        clahe->apply(src, dst);
    }
}

void ImageProcessor::applyContrastStretching(const cv::Mat& src, cv::Mat& dst) {
    if (src.channels() == 3) {
        // Process each channel separately
        std::vector<cv::Mat> channels;
        cv::split(src, channels);
        
        for (int i = 0; i < 3; i++) {
            double minVal, maxVal;
            cv::minMaxLoc(channels[i], &minVal, &maxVal);
            
            // Apply contrast stretching
            if (maxVal - minVal > 0) {
                channels[i].convertTo(channels[i], CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
            }
        }
        
        cv::merge(channels, dst);
    } else {
        // For grayscale
        double minVal, maxVal;
        cv::minMaxLoc(src, &minVal, &maxVal);
        
        // Apply contrast stretching
        if (maxVal - minVal > 0) {
            src.convertTo(dst, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
        } else {
            dst = src.clone();
        }
    }
}

// ============================================================================
// NOISE REMOVAL ALGORITHMS
// ============================================================================

void ImageProcessor::applyGaussianNoiseRemoval(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    // Ensure kernel size is odd and at least 3
    if (kernelSize % 2 == 0) kernelSize++;
    if (kernelSize < 3) kernelSize = 3;
    
    // Apply Gaussian blur for noise removal
    cv::GaussianBlur(src, dst, cv::Size(kernelSize, kernelSize), 0);
}

void ImageProcessor::applyMedianFilter(const cv::Mat& src, cv::Mat& dst, int kernelSize) {
    // Ensure kernel size is odd and at least 3
    if (kernelSize % 2 == 0) kernelSize++;
    if (kernelSize < 3) kernelSize = 3;
    
    // Apply median filter - excellent for salt & pepper noise
    cv::medianBlur(src, dst, kernelSize);
}

void ImageProcessor::applyBilateralFilter(const cv::Mat& src, cv::Mat& dst, int diameter, double sigmaColor, double sigmaSpace) {
    // Bilateral filter - preserves edges while removing noise
    // diameter: Diameter of each pixel neighborhood
    // sigmaColor: Filter sigma in the color space
    // sigmaSpace: Filter sigma in the coordinate space
    
    if (diameter < 1) diameter = 9;
    if (sigmaColor < 1) sigmaColor = 75;
    if (sigmaSpace < 1) sigmaSpace = 75;
    
    cv::bilateralFilter(src, dst, diameter, sigmaColor, sigmaSpace);
}

// ============================================================================
// PHASE 16: THRESHOLDING & SEGMENTATION
// ============================================================================

void ImageProcessor::applySimpleThreshold(const cv::Mat& src, cv::Mat& dst, int threshold, int maxValue) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    cv::threshold(gray, dst, threshold, maxValue, cv::THRESH_BINARY);
}

void ImageProcessor::applyAdaptiveThreshold(const cv::Mat& src, cv::Mat& dst, int maxValue, 
                                             int blockSize, int C, bool useGaussian) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Ensure block size is odd
    if (blockSize % 2 == 0) blockSize++;
    if (blockSize < 3) blockSize = 3;
    
    int adaptiveMethod = useGaussian ? cv::ADAPTIVE_THRESH_GAUSSIAN_C : cv::ADAPTIVE_THRESH_MEAN_C;
    
    cv::adaptiveThreshold(gray, dst, maxValue, adaptiveMethod, 
                         cv::THRESH_BINARY, blockSize, C);
}

double ImageProcessor::computeOtsuThreshold(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    double threshold = cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    return threshold;
}

void ImageProcessor::applyMultiLevelOtsu(const cv::Mat& src, cv::Mat& dst, int levels) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // For multi-level Otsu, we need to compute multiple thresholds
    // This is a simplified implementation using histogram analysis
    
    // Calculate histogram
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::Mat hist;
    cv::calcHist(&gray, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);
    
    // Normalize histogram
    hist /= gray.total();
    
    // Find thresholds using Otsu's method extended to multiple levels
    std::vector<int> thresholds;
    
    if (levels == 2) {
        // Standard Otsu
        double threshold = cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        thresholds.push_back(static_cast<int>(threshold));
    } else {
        // Multi-level: divide into equal ranges as approximation
        int step = 255 / levels;
        for (int i = 1; i < levels; i++) {
            thresholds.push_back(i * step);
        }
    }
    
    // Apply multi-level thresholding
    dst = cv::Mat::zeros(gray.size(), CV_8U);
    
    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            int pixel = gray.at<uchar>(i, j);
            int level = 0;
            
            for (size_t k = 0; k < thresholds.size(); k++) {
                if (pixel > thresholds[k]) {
                    level++;
                }
            }
            
            // Map level to gray value
            dst.at<uchar>(i, j) = (255 / levels) * level;
        }
    }
}

void ImageProcessor::applyLocalThreshold(const cv::Mat& src, cv::Mat& dst, int blockSize, int C) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Ensure block size is odd
    if (blockSize % 2 == 0) blockSize++;
    if (blockSize < 3) blockSize = 3;
    
    // Use adaptive threshold with mean method for local thresholding
    cv::adaptiveThreshold(gray, dst, 255, cv::ADAPTIVE_THRESH_MEAN_C, 
                         cv::THRESH_BINARY, blockSize, C);
}

void ImageProcessor::applyVariableThreshold(const cv::Mat& src, cv::Mat& dst, double k) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Calculate mean and standard deviation
    cv::Scalar mean, stddev;
    cv::meanStdDev(gray, mean, stddev);
    
    // Variable threshold: T = mean + k * stddev
    double threshold = mean[0] + k * stddev[0];
    
    // Clamp threshold to valid range
    if (threshold < 0) threshold = 0;
    if (threshold > 255) threshold = 255;
    
    cv::threshold(gray, dst, threshold, 255, cv::THRESH_BINARY);
}
