#pragma once
#include <opencv2/opencv.hpp>
#include <QString>
#include <vector>
namespace ColorProcessor {
void equalizeChannels(const cv::Mat& src, cv::Mat& dst);
void grayLevelSlicing(const cv::Mat& src, cv::Mat& dst, int minLevel, int maxLevel, int highlightValue, bool preserveBackground);
void bitPlaneSlicing(const cv::Mat& src, cv::Mat& dst, int bitPlane);
void autoWhiteBalance(const cv::Mat& src, cv::Mat& dst);
void colorBalance(const cv::Mat& src, cv::Mat& dst, double redGain, double greenGain, double blueGain);
void temperatureTint(const cv::Mat& src, cv::Mat& dst, double temperature, double tint);
void gammaCorrection(const cv::Mat& src, cv::Mat& dst, double gamma);
void applyPseudocolor(const cv::Mat& src, cv::Mat& dst, int colormap);
std::vector<QString> getColormapNames();
}