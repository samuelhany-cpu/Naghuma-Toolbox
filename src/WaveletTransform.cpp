#include "WaveletTransform.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

// Get wavelet filter coefficients
void WaveletTransform::getWaveletFilters(WaveletType type, std::vector<double>& lowpass, std::vector<double>& highpass) {
    lowpass.clear();
    highpass.clear();
    
    switch (type) {
        case HAAR:
            lowpass = {0.7071067811865476, 0.7071067811865476};
            highpass = {0.7071067811865476, -0.7071067811865476};
            break;
        case DB2:
            lowpass = {0.482962913145, 0.836516303738, 0.224143868042, -0.129409522551};
            highpass.resize(4);
            for (size_t i = 0; i < 4; i++) {
                highpass[i] = ((i % 2) == 0 ? 1 : -1) * lowpass[3 - i];
            }
            break;
        case DB4:
            lowpass = {0.230377813309, 0.714846570553, 0.630880767930, -0.027983769417,
                       -0.187034811719, 0.030841381836, 0.032883011667, -0.010597401785};
            highpass.resize(8);
            for (size_t i = 0; i < 8; i++) {
                highpass[i] = ((i % 2) == 0 ? 1 : -1) * lowpass[7 - i];
            }
            break;
        case DB6:
            lowpass = {0.111540743350, 0.494623890398, 0.751133908021, 0.315250351709,
                       -0.226264693965, -0.129766867567, 0.097501605587, 0.027522865530,
                       -0.031582039317, 0.000553842201, 0.004777257511, -0.001077301085};
            highpass.resize(12);
            for (size_t i = 0; i < 12; i++) {
                highpass[i] = ((i % 2) == 0 ? 1 : -1) * lowpass[11 - i];
            }
            break;
    }
}

// 1D convolution with downsampling
void WaveletTransform::convolve1D(const std::vector<double>& signal, const std::vector<double>& filter,
                                  std::vector<double>& output, int stride) {
    int signalLen = signal.size();
    int filterLen = filter.size();
    int outputLen = (signalLen + stride - 1) / stride;
    output.resize(outputLen);
    
    for (int i = 0; i < outputLen; i++) {
        double sum = 0.0;
        int signalPos = i * stride;
        
        for (int j = 0; j < filterLen; j++) {
            int pos = signalPos - j;
            if (pos < 0) pos = -pos;
            if (pos >= signalLen) pos = 2 * signalLen - pos - 2;
            if (pos >= 0 && pos < signalLen) {
                sum += signal[pos] * filter[j];
            }
        }
        output[i] = sum;
    }
}

// Upsample signal by 2
void WaveletTransform::upsample(const std::vector<double>& input, std::vector<double>& output) {
    output.resize(input.size() * 2);
    for (size_t i = 0; i < input.size(); i++) {
        output[i * 2] = input[i];
        output[i * 2 + 1] = 0.0;
    }
}

// 2D Discrete Wavelet Transform
void WaveletTransform::dwt2D(const cv::Mat& input, cv::Mat& approx, cv::Mat& horiz,
                             cv::Mat& vert, cv::Mat& diag, WaveletType type) {
    // IMPORTANT: Ensure input is grayscale (1-channel)
    cv::Mat grayInput;
    if (input.channels() == 3) {
        cv::cvtColor(input, grayInput, cv::COLOR_BGR2GRAY);
    } else if (input.channels() == 4) {
        cv::cvtColor(input, grayInput, cv::COLOR_BGRA2GRAY);
    } else {
        grayInput = input.clone();
    }
    
    // Extra safety check
    if (grayInput.channels() != 1) {
        throw std::runtime_error("Input must be single-channel grayscale image!");
    }
    
    cv::Mat floatImg;
    grayInput.convertTo(floatImg, CV_64F);
    
    // Verify conversion
    if (floatImg.channels() != 1) {
        throw std::runtime_error("Float conversion failed - still multi-channel!");
    }
    
    int rows = floatImg.rows;
    int cols = floatImg.cols;
    
    std::vector<double> lowpass, highpass;
    getWaveletFilters(type, lowpass, highpass);
    
    int halfCols = (cols + 1) / 2;
    cv::Mat temp1(rows, halfCols, CV_64F);
    cv::Mat temp2(rows, halfCols, CV_64F);
    
    // Apply filters to rows
    for (int i = 0; i < rows; i++) {
        std::vector<double> row(cols);
        for (int j = 0; j < cols; j++) {
            row[j] = floatImg.at<double>(i, j);
        }
        
        std::vector<double> low, high;
        convolve1D(row, lowpass, low, 2);
        convolve1D(row, highpass, high, 2);
        
        // Safely copy with bounds checking
        int actualSize = std::min((int)low.size(), halfCols);
        for (int j = 0; j < actualSize; j++) {
            temp1.at<double>(i, j) = low[j];
            temp2.at<double>(i, j) = high[j];
        }
    }
    
    // Apply filters to columns
    int halfRows = (rows + 1) / 2;
    
    approx = cv::Mat(halfRows, halfCols, CV_64F);
    horiz = cv::Mat(halfRows, halfCols, CV_64F);
    vert = cv::Mat(halfRows, halfCols, CV_64F);
    diag = cv::Mat(halfRows, halfCols, CV_64F);
    
    for (int j = 0; j < halfCols; j++) {
        std::vector<double> col1(rows), col2(rows);
        for (int i = 0; i < rows; i++) {
            col1[i] = temp1.at<double>(i, j);
            col2[i] = temp2.at<double>(i, j);
        }
        
        std::vector<double> ll, lh, hl, hh;
        convolve1D(col1, lowpass, ll, 2);
        convolve1D(col1, highpass, lh, 2);
        convolve1D(col2, lowpass, hl, 2);
        convolve1D(col2, highpass, hh, 2);
        
        // Safely copy with bounds checking
        int actualRowSize = std::min((int)ll.size(), halfRows);
        for (int i = 0; i < actualRowSize; i++) {
            approx.at<double>(i, j) = ll[i];
            horiz.at<double>(i, j) = lh[i];
            vert.at<double>(i, j) = hl[i];
            diag.at<double>(i, j) = hh[i];
        }
    }
}

// 2D Inverse Discrete Wavelet Transform
void WaveletTransform::idwt2D(const cv::Mat& approx, const cv::Mat& horiz,
                              const cv::Mat& vert, const cv::Mat& diag,
                              cv::Mat& recon, WaveletType type) {
    // Calculate target reconstruction size
    int targetRows = approx.rows * 2;
    int targetCols = approx.cols * 2;
    
    std::vector<double> lowpass, highpass;
    getWaveletFilters(type, lowpass, highpass);
    
    std::vector<double> lowpassRev(lowpass.rbegin(), lowpass.rend());
    std::vector<double> highpassRev(highpass.rbegin(), highpass.rend());
    
    // Temporary matrices for column reconstruction
    cv::Mat temp1(targetRows, approx.cols, CV_64F, cv::Scalar(0));
    cv::Mat temp2(targetRows, approx.cols, CV_64F, cv::Scalar(0));
    
    // Reconstruct columns
    for (int j = 0; j < approx.cols; j++) {
        std::vector<double> ll(approx.rows), lh(horiz.rows);
        std::vector<double> hl(vert.rows), hh(diag.rows);
        
        for (int i = 0; i < approx.rows; i++) {
            ll[i] = approx.at<double>(i, j);
            lh[i] = horiz.at<double>(i, j);
            hl[i] = vert.at<double>(i, j);
            hh[i] = diag.at<double>(i, j);
        }
        
        std::vector<double> llUp, lhUp, hlUp, hhUp;
        upsample(ll, llUp);
        upsample(lh, lhUp);
        upsample(hl, hlUp);
        upsample(hh, hhUp);
        
        std::vector<double> col1, col2;
        convolve1D(llUp, lowpassRev, col1, 1);
        convolve1D(lhUp, highpassRev, col2, 1);
        
        // Safely copy with bounds checking
        int safeRows = std::min(targetRows, std::min((int)col1.size(), (int)col2.size()));
        for (int i = 0; i < safeRows; i++) {
            temp1.at<double>(i, j) = col1[i] + col2[i];
        }
        
        convolve1D(hlUp, lowpassRev, col1, 1);
        convolve1D(hhUp, highpassRev, col2, 1);
        
        safeRows = std::min(targetRows, std::min((int)col1.size(), (int)col2.size()));
        for (int i = 0; i < safeRows; i++) {
            temp2.at<double>(i, j) = col1[i] + col2[i];
        }
    }
    
    // Reconstruct rows
    recon = cv::Mat(targetRows, targetCols, CV_64F, cv::Scalar(0));
    
    for (int i = 0; i < targetRows; i++) {
        std::vector<double> row1(approx.cols), row2(approx.cols);
        for (int j = 0; j < approx.cols; j++) {
            row1[j] = temp1.at<double>(i, j);
            row2[j] = temp2.at<double>(i, j);
        }
        
        std::vector<double> row1Up, row2Up;
        upsample(row1, row1Up);
        upsample(row2, row2Up);
        
        std::vector<double> recon1, recon2;
        convolve1D(row1Up, lowpassRev, recon1, 1);
        convolve1D(row2Up, highpassRev, recon2, 1);
        
        // Safely copy with bounds checking
        int safeCols = std::min(targetCols, std::min((int)recon1.size(), (int)recon2.size()));
        for (int j = 0; j < safeCols; j++) {
            recon.at<double>(i, j) = recon1[j] + recon2[j];
        }
    }
}

// Apply threshold to coefficients
void WaveletTransform::applyThreshold(cv::Mat& coeffs, double threshold, ThresholdMethod method) {
    for (int i = 0; i < coeffs.rows; i++) {
        for (int j = 0; j < coeffs.cols; j++) {
            double val = coeffs.at<double>(i, j);
            double absVal = std::abs(val);
            
            if (method == SOFT) {
                if (absVal < threshold) {
                    coeffs.at<double>(i, j) = 0.0;
                } else {
                    coeffs.at<double>(i, j) = (val > 0 ? 1 : -1) * (absVal - threshold);
                }
            } else {
                if (absVal < threshold) {
                    coeffs.at<double>(i, j) = 0.0;
                }
            }
        }
    }
}

// Wavelet denoising
void WaveletTransform::denoise(const cv::Mat& input, cv::Mat& output, double threshold,
                               ThresholdMethod method, int levels, WaveletType type) {
    // Ensure input is grayscale
    cv::Mat grayInput;
    if (input.channels() == 3) {
        cv::cvtColor(input, grayInput, cv::COLOR_BGR2GRAY);
    } else if (input.channels() == 4) {
        cv::cvtColor(input, grayInput, cv::COLOR_BGRA2GRAY);
    } else {
        grayInput = input.clone();
    }
    
    cv::Mat current = grayInput.clone();
    std::vector<std::vector<cv::Mat>> decompositions;
    std::vector<cv::Size> originalSizes;  // Store original size at each level
    
    // Multi-level decomposition
    for (int level = 0; level < levels; level++) {
        originalSizes.push_back(current.size());  // Store size before decomposition
        
        cv::Mat ap, hz, vt, dg;
        dwt2D(current, ap, hz, vt, dg, type);
        
        // Apply thresholding to detail coefficients
        applyThreshold(hz, threshold, method);
        applyThreshold(vt, threshold, method);
        applyThreshold(dg, threshold, method);
        
        std::vector<cv::Mat> levelData = {ap, hz, vt, dg};
        decompositions.push_back(levelData);
        
        current = ap;
    }
    
    // Reconstruct from coarsest to finest
    cv::Mat rc = decompositions[levels - 1][0].clone();
    
    // Reconstruct level by level from coarsest to finest
    for (int level = levels - 1; level >= 0; level--) {
        cv::Mat hz = decompositions[level][1];
        cv::Mat vt = decompositions[level][2];
        cv::Mat dg = decompositions[level][3];
        
        cv::Mat reconstructed;
        idwt2D(rc, hz, vt, dg, reconstructed, type);
        
        // Crop/pad to original size at this level
        cv::Size targetSize = originalSizes[level];
        if (reconstructed.rows != targetSize.height || reconstructed.cols != targetSize.width) {
            // Crop if larger
            int cropRows = std::min(reconstructed.rows, targetSize.height);
            int cropCols = std::min(reconstructed.cols, targetSize.width);
            cv::Rect roi(0, 0, cropCols, cropRows);
            reconstructed = reconstructed(roi).clone();
            
            // Pad if smaller (shouldn't happen, but just in case)
            if (reconstructed.rows < targetSize.height || reconstructed.cols < targetSize.width) {
                cv::Mat padded(targetSize, reconstructed.type(), cv::Scalar(0));
                reconstructed.copyTo(padded(cv::Rect(0, 0, reconstructed.cols, reconstructed.rows)));
                reconstructed = padded;
            }
        }
        
        rc = reconstructed;
    }
    
    // Convert back to original type
    rc.convertTo(output, input.type());
}

// Visualize wavelet decomposition
cv::Mat WaveletTransform::visualizeDecomposition(const cv::Mat& approx, const cv::Mat& horiz,
                                                const cv::Mat& vert, const cv::Mat& diag) {
    cv::Mat normAp, normHz, normVt, normDg;
    
    cv::normalize(approx, normAp, 0, 255, cv::NORM_MINMAX);
    cv::normalize(horiz, normHz, 0, 255, cv::NORM_MINMAX);
    cv::normalize(vert, normVt, 0, 255, cv::NORM_MINMAX);
    cv::normalize(diag, normDg, 0, 255, cv::NORM_MINMAX);
    
    normAp.convertTo(normAp, CV_8U);
    normHz.convertTo(normHz, CV_8U);
    normVt.convertTo(normVt, CV_8U);
    normDg.convertTo(normDg, CV_8U);
    
    // Create 2x2 grid
    cv::Mat top, bottom, result;
    cv::hconcat(normAp, normHz, top);
    cv::hconcat(normVt, normDg, bottom);
    cv::vconcat(top, bottom, result);
    
    return result;
}

// Get wavelet name
std::string WaveletTransform::getWaveletName(WaveletType type) {
    switch (type) {
        case HAAR: return "Haar";
        case DB2: return "Daubechies-2";
        case DB4: return "Daubechies-4";
        case DB6: return "Daubechies-6";
        default: return "Unknown";
    }
}
