#include "TextRecognition.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <array>
#include <memory>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

namespace fs = std::filesystem;

// OCR.space API configuration
const std::string OCR_SPACE_API_KEY = "K87899142388957"; // Free tier API key
const std::string OCR_SPACE_API_URL = "https://api.ocr.space/parse/image";

// Helper function to upload image to OCR.space and get result
namespace {
    std::string uploadImageToOCRSpace(const std::string& imagePath, const std::string& apiKey) {
        std::string result;
        
        // Read image file as base64
        std::ifstream file(imagePath, std::ios::binary);
        if (!file.is_open()) {
            return "{\"success\": false, \"error\": \"Failed to open image file\"}";
        }
        
        // Read file content
        std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        // Use curl to upload (simpler approach)
        std::string tempResponseFile = (fs::temp_directory_path() / "ocr_response.json").string();
        std::string command = "curl -X POST \"" + std::string(OCR_SPACE_API_URL) + "\" "
            + "-H \"apikey: " + apiKey + "\" "
            + "-F \"file=@" + imagePath + "\" "
            + "-F \"language=eng\" "
            + "-F \"isOverlayRequired=false\" "
            + "-o \"" + tempResponseFile + "\" 2>&1";
        
        std::cout << "[OCR] Uploading to OCR.space..." << std::endl;
        system(command.c_str());
        
        // Read response
        std::ifstream responseFile(tempResponseFile);
        if (responseFile.is_open()) {
            std::stringstream buffer;
            buffer << responseFile.rdbuf();
            result = buffer.str();
            responseFile.close();
            
            // Clean up temp file
            fs::remove(tempResponseFile);
        }
        
        return result;
    }

    // JSON parsing helper (simple parser for our use case)
    std::string extractJsonValue(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return "";
        
        pos = json.find(":", pos);
        if (pos == std::string::npos) return "";
        
        pos = json.find("\"", pos);
        if (pos == std::string::npos) return "";
        
        size_t end = json.find("\"", pos + 1);
        if (end == std::string::npos) return "";
        
        return json.substr(pos + 1, end - pos - 1);
    }
    
    double extractJsonNumber(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return 0.0;
        
        pos = json.find(":", pos);
        if (pos == std::string::npos) return 0.0;
        
        // Skip whitespace
        pos++;
        while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        
        size_t end = pos;
        while (end < json.length() && (isdigit(json[end]) || json[end] == '.' || json[end] == '-')) end++;
        
        std::string numStr = json.substr(pos, end - pos);
        return std::stod(numStr);
    }
    
    bool extractJsonBool(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return false;
        
        pos = json.find(":", pos);
        if (pos == std::string::npos) return false;
        
        return json.find("true", pos) != std::string::npos;
    }
}

// ============================================================================
// CONSTRUCTOR & DESTRUCTOR
// ============================================================================

TextRecognition::TextRecognition() 
    : initialized(false), pythonPath(""), language("eng") {
}

TextRecognition::~TextRecognition() {
    // No cleanup needed
}

// ============================================================================
// INITIALIZATION
// ============================================================================

bool TextRecognition::checkPythonAvailability() {
    // Not needed for OCR.space cloud API
    return true;
}

bool TextRecognition::initialize(const std::string& tessDataPath, const std::string& language) {
    try {
        this->language = language.empty() ? "eng" : language;
        initialized = true;
        
        std::cout << "[OCR] OCR.space cloud API initialized successfully" << std::endl;
        std::cout << "[OCR] Language: " << this->language << std::endl;
        
        return true;
        
    } catch (const std::exception& e) {
        setLastError(std::string("Exception during initialization: ") + e.what());
        return false;
    }
}

// ============================================================================
// TEXT RECOGNITION
// ============================================================================

TextRecognition::OCRResult TextRecognition::recognizeText(const cv::Mat& image, bool preprocess) {
    PreprocessOptions options;
    if (preprocess) {
        return recognizeText(image, options);
    } else {
        // No preprocessing
        PreprocessOptions noPreprocess;
        noPreprocess.convertToGrayscale = false;
        noPreprocess.applyThreshold = false;
        noPreprocess.denoiseImage = false;
        noPreprocess.enhanceContrast = false;
        return recognizeText(image, noPreprocess);
    }
}

TextRecognition::OCRResult TextRecognition::recognizeText(const cv::Mat& image, const PreprocessOptions& options) {
    OCRResult result;
    
    if (!initialized) {
        result.errorMessage = "OCR engine not initialized";
        return result;
    }
    
    if (image.empty()) {
        result.errorMessage = "Input image is empty";
        return result;
    }
    
    try {
        // Preprocess image
        cv::Mat processedImage;
        preprocessImage(image, processedImage, options);
        
        // Save to temporary file for upload
        std::string tempPath = (fs::temp_directory_path() / "naghuma_ocr_temp.png").string();
        cv::imwrite(tempPath, processedImage);
        
        std::cout << "[OCR] Uploading image to OCR.space..." << std::endl;
        
        // Upload to OCR.space
        std::string output = uploadImageToOCRSpace(tempPath, OCR_SPACE_API_KEY);
        
        std::cout << "[OCR] Response received" << std::endl;
        
        // Parse OCR.space JSON response
        // Response format: {"ParsedResults":[{"ParsedText":"..."}],"OCRExitCode":1,"IsErroredOnProcessing":false}
        
        bool isError = extractJsonBool(output, "IsErroredOnProcessing");
        
        if (!isError && output.find("ParsedResults") != std::string::npos) {
            // Extract text from ParsedResults array
            size_t textPos = output.find("\"ParsedText\":");
            if (textPos != std::string::npos) {
                size_t startQuote = output.find("\"", textPos + 13);
                size_t endQuote = output.find("\"", startQuote + 1);
                
                if (startQuote != std::string::npos && endQuote != std::string::npos) {
                    result.text = output.substr(startQuote + 1, endQuote - startQuote - 1);
                    
                    // Replace \r\n with actual newlines
                    size_t pos = 0;
                    while ((pos = result.text.find("\\r\\n", pos)) != std::string::npos) {
                        result.text.replace(pos, 4, "\n");
                        pos += 1;
                    }
                    
                    result.confidence = 95.0f; // OCR.space doesn't provide confidence
                    result.success = true;
                }
            }
        } else {
            result.errorMessage = "OCR processing failed or returned error";
            std::cerr << "[OCR] Response: " << output << std::endl;
        }
        
        // Clean up temp file
        try {
            fs::remove(tempPath);
        } catch (...) {}
        
        return result;
        
    } catch (const std::exception& e) {
        result.errorMessage = std::string("Exception during recognition: ") + e.what();
        return result;
    } catch (...) {
        result.errorMessage = "Unknown exception during OCR";
        return result;
    }
}

// ============================================================================
// IMAGE PREPROCESSING
// ============================================================================

void TextRecognition::preprocessImage(const cv::Mat& input, cv::Mat& output, 
                                     const PreprocessOptions& options) {
    if (input.empty()) {
        output = input.clone();
        return;
    }
    
    cv::Mat working = input.clone();
    
    // 1. Convert to grayscale
    if (options.convertToGrayscale && working.channels() == 3) {
        cv::cvtColor(working, working, cv::COLOR_BGR2GRAY);
    }
    
    // 2. Enhance contrast
    if (options.enhanceContrast) {
        cv::Mat enhanced;
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
        clahe->setClipLimit(2.0);
        clahe->setTilesGridSize(cv::Size(8, 8));
        clahe->apply(working, enhanced);
        working = enhanced;
    }
    
    // 3. Denoise
    if (options.denoiseImage) {
        cv::Mat denoised;
        cv::fastNlMeansDenoising(working, denoised, 10, 7, 21);
        working = denoised;
    }
    
    // 4. Apply adaptive threshold
    if (options.applyThreshold) {
        cv::Mat thresholded;
        cv::adaptiveThreshold(working, thresholded, 255, 
                            cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
                            cv::THRESH_BINARY, 
                            options.thresholdBlockSize, 
                            options.thresholdC);
        working = thresholded;
    }
    
    output = working;
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

bool TextRecognition::isInitialized() const {
    return initialized;
}

std::string TextRecognition::getLastError() const {
    return lastError;
}

void TextRecognition::setLastError(const std::string& error) {
    lastError = error;
    std::cerr << "[OCR Error] " << error << std::endl;
}

std::string TextRecognition::getOCRVersion() {
    return "OCR.space Cloud API v3.50";
}

void TextRecognition::setPythonPath(const std::string& path) {
    // Not used for cloud API
}