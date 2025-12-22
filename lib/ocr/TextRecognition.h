#pragma once
#ifndef TEXTRECOGNITION_H
#define TEXTRECOGNITION_H

#include <string>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>

/**
 * @brief OCR (Optical Character Recognition) Engine
 * 
 * Modern C++ wrapper using EasyOCR (Python-based) for text recognition from images.
 * Features automatic resource management, preprocessing, and confidence scoring.
 * 
 * Usage Example:
 *   TextRecognition ocr;
 *   if (ocr.initialize()) {
 *       auto result = ocr.recognizeText(image);
 *       if (result.success) {
 *           std::cout << "Text: " << result.text << std::endl;
 *           std::cout << "Confidence: " << result.confidence << "%" << std::endl;
 *       }
 *   }
 */
class TextRecognition {
public:
    /**
     * @brief OCR Recognition Result
     */
    struct OCRResult {
        std::string text;                      // Recognized text
        float confidence;                      // Overall confidence (0-100)
        std::vector<cv::Rect> boundingBoxes;   // Bounding boxes for detected text regions
        std::vector<std::string> words;        // Individual recognized words
        std::vector<float> wordConfidences;    // Confidence for each word
        bool success;                          // true if recognition succeeded
        std::string errorMessage;              // Error message if success = false
        
        OCRResult() : confidence(0.0f), success(false) {}
    };
    
    /**
     * @brief Image preprocessing options
     */
    struct PreprocessOptions {
        bool convertToGrayscale = true;
        bool applyThreshold = true;
        bool denoiseImage = true;
        bool enhanceContrast = true;
        int thresholdBlockSize = 11;
        double thresholdC = 2.0;
        
        PreprocessOptions() = default;
    };
    
    /**
     * @brief Constructor
     */
    TextRecognition();
    
    /**
     * @brief Destructor - automatically cleans up Tesseract resources
     */
    ~TextRecognition();
    
    // Prevent copying (Tesseract API is not copyable)
    TextRecognition(const TextRecognition&) = delete;
    TextRecognition& operator=(const TextRecognition&) = delete;
    
    /**
     * @brief Initialize OCR engine
     * @param tessDataPath Path to tessdata folder (language data)
     *                     If empty, searches in: ./tessdata, ../tessdata, ./vcpkg/installed/x64-windows/share/tessdata
     * @param language Language code (e.g., "eng", "ara", "fra", "eng+ara" for multiple)
     * @return true if initialization successful
     */
    bool initialize(const std::string& tessDataPath = "", 
                   const std::string& language = "eng");
    
    /**
     * @brief Recognize text from image
     * @param image Input image (grayscale or color)
     * @param preprocess Apply preprocessing for better accuracy
     * @return OCR result with recognized text and metadata
     */
    OCRResult recognizeText(const cv::Mat& image, bool preprocess = true);
    
    /**
     * @brief Recognize text with custom preprocessing options
     * @param image Input image
     * @param options Custom preprocessing options
     * @return OCR result
     */
    OCRResult recognizeText(const cv::Mat& image, const PreprocessOptions& options);
    
    /**
     * @brief Preprocess image for better OCR accuracy
     * @param input Input image
     * @param output Preprocessed image
     * @param options Preprocessing options
     */
    static void preprocessImage(const cv::Mat& input, cv::Mat& output, 
                                const PreprocessOptions& options = PreprocessOptions());
    
    /**
     * @brief Check if OCR engine is initialized and ready
     * @return true if initialized
     */
    bool isInitialized() const;
    
    /**
     * @brief Get last error message
     * @return Error message string
     */
    std::string getLastError() const;
    
    /**
     * @brief Get EasyOCR version information
     * @return Version string
     */
    static std::string getOCRVersion();
    
    /**
     * @brief Set Python executable path
     * @param path Path to Python executable
     */
    void setPythonPath(const std::string& path);
    
private:
    bool initialized;
    std::string lastError;
    std::string pythonPath;
    std::string language;
    
    void setLastError(const std::string& error);
    bool checkPythonAvailability();
};

#endif // TEXTRECOGNITION_H
