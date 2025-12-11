#include "ImageMetrics.h"
#include <cmath>

bool ImageMetrics::validateImages(const cv::Mat& img1, const cv::Mat& img2) {
    if (img1.empty() || img2.empty()) return false;
    if (img1.size() != img2.size()) return false;
    if (img1.type() != img2.type()) return false;
    return true;
}

ImageMetrics::MetricsResult ImageMetrics::calculateMetrics(const cv::Mat& original, const cv::Mat& processed) {
    MetricsResult result;
    result.isValid = false;
    
    if (!validateImages(original, processed)) {
        result.errorMessage = "Images must have same dimensions and type";
        return result;
    }
    
    result.mse = calculateMSE(original, processed);
    result.rmse = calculateRMSE(original, processed);
    result.snr = calculateSNR(original, processed);
    result.psnr = calculatePSNR(original, processed);
    result.isValid = true;
    
    return result;
}

double ImageMetrics::calculateMSE(const cv::Mat& original, const cv::Mat& processed) {
    if (!validateImages(original, processed)) return -1.0;
    
    cv::Mat diff;
    cv::subtract(original, processed, diff);
    diff = diff.mul(diff); // Square the differences
    
    cv::Scalar sumScalar = cv::sum(diff);
    double sum = sumScalar[0] + sumScalar[1] + sumScalar[2];
    
    int totalPixels = original.rows * original.cols * original.channels();
    return sum / totalPixels;
}

double ImageMetrics::calculateRMSE(const cv::Mat& original, const cv::Mat& processed) {
    double mse = calculateMSE(original, processed);
    if (mse < 0) return -1.0;
    return std::sqrt(mse);
}

double ImageMetrics::calculateSNR(const cv::Mat& original, const cv::Mat& processed) {
    if (!validateImages(original, processed)) return -1.0;
    
    // Convert to double for precision
    cv::Mat origDouble, procDouble;
    original.convertTo(origDouble, CV_64F);
    processed.convertTo(procDouble, CV_64F);
    
    // Calculate noise (difference)
    cv::Mat noise;
    cv::subtract(origDouble, procDouble, noise);
    
    // Calculate power of original signal
    cv::Mat origSquared = origDouble.mul(origDouble);
    cv::Scalar origPowerScalar = cv::sum(origSquared);
    double origPower = origPowerScalar[0] + origPowerScalar[1] + origPowerScalar[2];
    
    // Calculate power of noise
    cv::Mat noiseSquared = noise.mul(noise);
    cv::Scalar noisePowerScalar = cv::sum(noiseSquared);
    double noisePower = noisePowerScalar[0] + noisePowerScalar[1] + noisePowerScalar[2];
    
    if (noisePower == 0) return INFINITY;
    
    // SNR = Signal Power / Noise Power
    double snr = origPower / noisePower;
    
    // Convert to dB
    return 10.0 * std::log10(snr);
}

double ImageMetrics::calculatePSNR(const cv::Mat& original, const cv::Mat& processed, double L) {
    double mse = calculateMSE(original, processed);
    
    if (mse < 0) return -1.0;
    if (mse == 0) return INFINITY;
    
    // PSNR = 10 * log10(L^2 / MSE)
    return 10.0 * std::log10((L * L) / mse);
}

QString ImageMetrics::formatMetrics(const MetricsResult& result) {
    if (!result.isValid) {
        return QString("Error: %1").arg(result.errorMessage);
    }
    
    QString formatted;
    formatted += QString("MSE:  %1\n").arg(result.mse, 0, 'f', 4);
    formatted += QString("RMSE: %1\n").arg(result.rmse, 0, 'f', 4);
    formatted += QString("SNR:  %1 dB\n").arg(result.snr, 0, 'f', 2);
    
    if (std::isinf(result.psnr)) {
        formatted += QString("PSNR: ? (identical images)");
    } else {
        formatted += QString("PSNR: %1 dB").arg(result.psnr, 0, 'f', 2);
    }
    
    return formatted;
}
