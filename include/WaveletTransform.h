#ifndef WAVELETTRANSFORM_H
#define WAVELETTRANSFORM_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class WaveletTransform {
public:
    enum WaveletType { HAAR, DB2, DB4, DB6 };
    enum ThresholdMethod { SOFT, HARD };
    
    static void dwt2D(const cv::Mat& input, cv::Mat& approx, cv::Mat& horiz, 
                      cv::Mat& vert, cv::Mat& diag, WaveletType type = HAAR);
    static void idwt2D(const cv::Mat& approx, const cv::Mat& horiz, 
                       const cv::Mat& vert, const cv::Mat& diag, 
                       cv::Mat& recon, WaveletType type = HAAR);
    static void denoise(const cv::Mat& input, cv::Mat& output, double threshold, 
                        ThresholdMethod method = SOFT, int levels = 3, WaveletType type = HAAR);
    static cv::Mat visualizeDecomposition(const cv::Mat& approx, const cv::Mat& horiz, 
                                          const cv::Mat& vert, const cv::Mat& diag);
    static std::string getWaveletName(WaveletType type);
    
private:
    static void getWaveletFilters(WaveletType type, std::vector<double>& lowpass, 
                                  std::vector<double>& highpass);
    static void convolve1D(const std::vector<double>& signal, const std::vector<double>& filter, 
                          std::vector<double>& output, int stride = 1);
    static void upsample(const std::vector<double>& input, std::vector<double>& output);
    static void applyThreshold(cv::Mat& coeffs, double threshold, ThresholdMethod method);
};

#endif // WAVELETTRANSFORM_H
