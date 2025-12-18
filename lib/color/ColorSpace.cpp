#include "ColorSpace.h"
#include <QString>
#include <cmath>
#include <algorithm>

namespace ColorSpace {

// ============================================================================
// RGB <-> HSV Conversions
// ============================================================================

void RGBtoHSV(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // OpenCV uses BGR format by default
    cv::cvtColor(src, dst, cv::COLOR_BGR2HSV);
}

void HSVtoRGB(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // Convert HSV to BGR (OpenCV format)
    cv::cvtColor(src, dst, cv::COLOR_HSV2BGR);
}

// ============================================================================
// RGB <-> LAB Conversions
// ============================================================================

void RGBtoLAB(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // OpenCV uses BGR format by default
    cv::cvtColor(src, dst, cv::COLOR_BGR2Lab);
}

void LABtoRGB(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // Convert LAB to BGR (OpenCV format)
    cv::cvtColor(src, dst, cv::COLOR_Lab2BGR);
}

// ============================================================================
// RGB <-> YCbCr Conversions
// ============================================================================

void RGBtoYCbCr(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // OpenCV uses BGR format by default
    cv::cvtColor(src, dst, cv::COLOR_BGR2YCrCb);
}

void YCbCrtoRGB(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // Convert YCrCb to BGR (OpenCV format)
    cv::cvtColor(src, dst, cv::COLOR_YCrCb2BGR);
}

// ============================================================================
// RGB <-> HSI Conversions
// ============================================================================

void RGBtoHSI(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // HSI conversion (manual implementation as OpenCV doesn't have built-in HSI)
    dst = cv::Mat(src.size(), src.type());
    
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            cv::Vec3b bgr = src.at<cv::Vec3b>(y, x);
            float b = bgr[0] / 255.0f;
            float g = bgr[1] / 255.0f;
            float r = bgr[2] / 255.0f;
            
            // Intensity
            float intensity = (r + g + b) / 3.0f;
            
            // Saturation
            float min_val = std::min({r, g, b});
            float saturation = (intensity > 0) ? (1.0f - min_val / intensity) : 0.0f;
            
            // Hue
            float hue = 0.0f;
            if (saturation > 0.0f) {
                float numerator = 0.5f * ((r - g) + (r - b));
                float denominator = std::sqrt((r - g) * (r - g) + (r - b) * (g - b));
                
                if (denominator > 0.0f) {
                    float theta = std::acos(std::clamp(numerator / denominator, -1.0f, 1.0f));
                    hue = (b <= g) ? theta : (2.0f * CV_PI - theta);
                }
            }
            
            // Convert to 0-255 range
            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(
                static_cast<uchar>(hue * 255.0f / (2.0f * CV_PI)),
                static_cast<uchar>(saturation * 255.0f),
                static_cast<uchar>(intensity * 255.0f)
            );
        }
    }
}

void HSItoRGB(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // HSI to RGB conversion (manual implementation)
    dst = cv::Mat(src.size(), src.type());
    
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            cv::Vec3b hsi = src.at<cv::Vec3b>(y, x);
            float h = hsi[0] * 2.0f * CV_PI / 255.0f;  // Convert to radians
            float s = hsi[1] / 255.0f;
            float i = hsi[2] / 255.0f;
            
            float r, g, b;
            
            // RG sector (0° <= H < 120°)
            if (h < 2.0f * CV_PI / 3.0f) {
                b = i * (1.0f - s);
                r = i * (1.0f + s * std::cos(h) / std::cos(CV_PI / 3.0f - h));
                g = 3.0f * i - (r + b);
            }
            // GB sector (120° <= H < 240°)
            else if (h < 4.0f * CV_PI / 3.0f) {
                h = h - 2.0f * CV_PI / 3.0f;
                r = i * (1.0f - s);
                g = i * (1.0f + s * std::cos(h) / std::cos(CV_PI / 3.0f - h));
                b = 3.0f * i - (r + g);
            }
            // BR sector (240° <= H < 360°)
            else {
                h = h - 4.0f * CV_PI / 3.0f;
                g = i * (1.0f - s);
                b = i * (1.0f + s * std::cos(h) / std::cos(CV_PI / 3.0f - h));
                r = 3.0f * i - (g + b);
            }
            
            // Clamp values to [0, 1] and convert to 0-255
            r = std::clamp(r, 0.0f, 1.0f);
            g = std::clamp(g, 0.0f, 1.0f);
            b = std::clamp(b, 0.0f, 1.0f);
            
            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(
                static_cast<uchar>(b * 255.0f),
                static_cast<uchar>(g * 255.0f),
                static_cast<uchar>(r * 255.0f)
            );
        }
    }
}

// ============================================================================
// RGB <-> Grayscale Conversions
// ============================================================================

void RGBtoGray(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // OpenCV uses BGR format, convert to grayscale
    cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
}

void GraytoRGB(const cv::Mat& src, cv::Mat& dst) {
    if (src.empty()) return;
    
    // Convert grayscale to BGR (duplicate channels)
    cv::cvtColor(src, dst, cv::COLOR_GRAY2BGR);
}

// ============================================================================
// Channel Extraction
// ============================================================================

void extractChannels(const cv::Mat& src, std::vector<cv::Mat>& channels) {
    if (src.empty()) return;
    
    cv::split(src, channels);
}

void mergeChannels(const std::vector<cv::Mat>& channels, cv::Mat& dst) {
    if (channels.empty()) return;
    
    cv::merge(channels, dst);
}

// ============================================================================
// Utility Functions
// ============================================================================

QString getColorSpaceName(int code) {
    switch (code) {
        case cv::COLOR_BGR2HSV: return "HSV";
        case cv::COLOR_BGR2Lab: return "LAB";
        case cv::COLOR_BGR2YCrCb: return "YCbCr";
        case cv::COLOR_BGR2GRAY: return "Grayscale";
        default: return "Unknown";
    }
}

std::vector<QString> getChannelNames(const QString& colorSpace) {
    if (colorSpace == "RGB" || colorSpace == "BGR") {
        return {"Blue", "Green", "Red"};
    } else if (colorSpace == "HSV") {
        return {"Hue", "Saturation", "Value"};
    } else if (colorSpace == "LAB") {
        return {"Lightness", "a (green-red)", "b (blue-yellow)"};
    } else if (colorSpace == "YCbCr") {
        return {"Y (Luminance)", "Cb (Blue-diff)", "Cr (Red-diff)"};
    } else if (colorSpace == "HSI") {
        return {"Hue", "Saturation", "Intensity"};
    } else if (colorSpace == "Grayscale") {
        return {"Gray"};
    }
    
    return {"Channel 0", "Channel 1", "Channel 2"};
}

} // namespace ColorSpace
