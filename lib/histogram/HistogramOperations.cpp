#include "HistogramOperations.h"
#include <algorithm>
#include <numeric>

namespace HistogramOperations {

int calculateHistogram(const cv::Mat& src, std::vector<int>& histogram) {
    histogram.clear();
    histogram.resize(256, 0);
    
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    int maxFreq = 0;
    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            uchar pixel = gray.at<uchar>(i, j);
            histogram[pixel]++;
            if (histogram[pixel] > maxFreq) {
                maxFreq = histogram[pixel];
            }
        }
    }
    
    return maxFreq;
}

int calculateColorHistogram(const cv::Mat& src,
                            std::vector<int>& histB,
                            std::vector<int>& histG,
                            std::vector<int>& histR) {
    histB.clear(); histB.resize(256, 0);
    histG.clear(); histG.resize(256, 0);
    histR.clear(); histR.resize(256, 0);
    
    int maxFreq = 0;
    
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(i, j);
            histB[pixel[0]]++;
            histG[pixel[1]]++;
            histR[pixel[2]]++;
            
            maxFreq = std::max(maxFreq, std::max({histB[pixel[0]], 
                                                  histG[pixel[1]], 
                                                  histR[pixel[2]]}));
        }
    }
    
    return maxFreq;
}

void equalizeHistogram(const cv::Mat& src, cv::Mat& dst, bool preserveColor) {
    if (src.channels() == 1) {
        cv::equalizeHist(src, dst);
    } else if (preserveColor) {
        // Convert to YCrCb and equalize only Y channel
        cv::Mat ycrcb;
        cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);
        
        std::vector<cv::Mat> channels;
        cv::split(ycrcb, channels);
        cv::equalizeHist(channels[0], channels[0]);
        cv::merge(channels, ycrcb);
        
        cv::cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);
    } else {
        // Equalize each channel independently
        std::vector<cv::Mat> channels;
        cv::split(src, channels);
        
        for (auto& channel : channels) {
            cv::equalizeHist(channel, channel);
        }
        
        cv::merge(channels, dst);
    }
}

double otsuThreshold(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    double threshold = cv::threshold(gray, dst, 0, 255, 
                                    cv::THRESH_BINARY | cv::THRESH_OTSU);
    return threshold;
}

void adaptiveHistogramEqualization(const cv::Mat& src, cv::Mat& dst,
                                  double clipLimit,
                                  int tileSize) {
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clipLimit, cv::Size(tileSize, tileSize));
    
    if (src.channels() == 1) {
        clahe->apply(src, dst);
    } else {
        // Convert to Lab and apply to L channel
        cv::Mat lab;
        cv::cvtColor(src, lab, cv::COLOR_BGR2Lab);
        
        std::vector<cv::Mat> channels;
        cv::split(lab, channels);
        clahe->apply(channels[0], channels[0]);
        cv::merge(channels, lab);
        
        cv::cvtColor(lab, dst, cv::COLOR_Lab2BGR);
    }
}

void applyThreshold(const cv::Mat& src, cv::Mat& dst,
                   double threshold,
                   double maxValue,
                   int thresholdType) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    cv::threshold(gray, dst, threshold, maxValue, thresholdType);
}

void adaptiveThreshold(const cv::Mat& src, cv::Mat& dst,
                      double maxValue,
                      int method,
                      int type,
                      int blockSize,
                      double C) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Ensure block size is odd and >= 3
    if (blockSize % 2 == 0) {
        blockSize++;
    }
    if (blockSize < 3) {
        blockSize = 3;
    }
    
    cv::adaptiveThreshold(gray, dst, maxValue, method, type, blockSize, C);
}

void calculateStatistics(const cv::Mat& src,
                        double& mean,
                        double& stddev,
                        double& minVal,
                        double& maxVal) {
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Calculate min and max
    cv::minMaxLoc(gray, &minVal, &maxVal);
    
    // Calculate mean and standard deviation
    cv::Scalar meanScalar, stddevScalar;
    cv::meanStdDev(gray, meanScalar, stddevScalar);
    mean = meanScalar[0];
    stddev = stddevScalar[0];
}

void histogramStretching(const cv::Mat& src, cv::Mat& dst) {
    double minVal, maxVal;
    cv::minMaxLoc(src, &minVal, &maxVal);
    
    // Stretch to full range [0, 255]
    src.convertTo(dst, -1, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
}

void histogramMatching(const cv::Mat& src, const cv::Mat& ref, cv::Mat& dst) {
    // Calculate CDFs for both images
    std::vector<int> srcHist, refHist;
    calculateHistogram(src, srcHist);
    calculateHistogram(ref, refHist);
    
    // Calculate cumulative distribution functions
    std::vector<float> srcCDF(256), refCDF(256);
    int srcTotal = src.rows * src.cols;
    int refTotal = ref.rows * ref.cols;
    
    srcCDF[0] = (float)srcHist[0] / srcTotal;
    refCDF[0] = (float)refHist[0] / refTotal;
    
    for (int i = 1; i < 256; i++) {
        srcCDF[i] = srcCDF[i-1] + (float)srcHist[i] / srcTotal;
        refCDF[i] = refCDF[i-1] + (float)refHist[i] / refTotal;
    }
    
    // Create lookup table
    std::vector<uchar> lookupTable(256);
    for (int i = 0; i < 256; i++) {
        float value = srcCDF[i];
        int j = 0;
        while (j < 255 && refCDF[j] < value) {
            j++;
        }
        lookupTable[i] = j;
    }
    
    // Apply lookup table
    cv::Mat gray;
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    dst = gray.clone();
    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            dst.at<uchar>(i, j) = lookupTable[gray.at<uchar>(i, j)];
        }
    }
}

void gammaCorrection(const cv::Mat& src, cv::Mat& dst, double gamma) {
    // Create lookup table
    cv::Mat lookupTable(1, 256, CV_8U);
    uchar* p = lookupTable.ptr();
    for (int i = 0; i < 256; i++) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }
    
    // Apply lookup table
    cv::LUT(src, lookupTable, dst);
}

void adjustBrightnessContrast(const cv::Mat& src, cv::Mat& dst,
                             double alpha,
                             double beta) {
    src.convertTo(dst, -1, alpha, beta);
}

} // namespace HistogramOperations
