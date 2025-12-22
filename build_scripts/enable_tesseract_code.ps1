# ============================================================================
# ENABLE TESSERACT CODE - UNCOMMENT AND ACTIVATE
# ============================================================================

Write-Host ""
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  ENABLING TESSERACT OCR CODE                          " -ForegroundColor Cyan
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# Load configuration
$configFile = "tesseract_config.json"

if (-not (Test-Path $configFile)) {
    Write-Host "? Configuration file not found!" -ForegroundColor Red
    Write-Host "  Please run: .\setup_tesseract.ps1 first" -ForegroundColor Yellow
    exit 1
}

$config = Get-Content $configFile | ConvertFrom-Json
Write-Host "? Loaded Tesseract configuration" -ForegroundColor Green
Write-Host ""

# Step 1: Update TextRecognition.cpp
Write-Host "Step 1: Updating TextRecognition.cpp..." -ForegroundColor Yellow
Write-Host ""

$cppFile = "lib\ocr\TextRecognition.cpp"

if (-not (Test-Path $cppFile)) {
    Write-Host "? TextRecognition.cpp not found!" -ForegroundColor Red
    exit 1
}

# Read the current file
$content = Get-Content $cppFile -Raw

# Create backup
$backupFile = "$cppFile.backup"
Copy-Item $cppFile $backupFile -Force
Write-Host "  ? Created backup: $backupFile" -ForegroundColor Gray

# Replace the file with enabled Tesseract code
$newContent = @'
#include "TextRecognition.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>

bool TextRecognition::initialized = false;
static tesseract::TessBaseAPI* tessApi = nullptr;

bool TextRecognition::initialize(const std::string& tessDataPath, 
                                 const std::string& language) {
    try {
        if (tessApi != nullptr) {
            tessApi->End();
            delete tessApi;
        }
        
        tessApi = new tesseract::TessBaseAPI();
        
        // Use environment variable if tessDataPath is empty
        const char* dataPath = tessDataPath.empty() ? nullptr : tessDataPath.c_str();
        
        if (tessApi->Init(dataPath, language.c_str()) != 0) {
            std::cerr << "Failed to initialize Tesseract OCR" << std::endl;
            std::cerr << "Language: " << language << std::endl;
            std::cerr << "Data path: " << (dataPath ? dataPath : "using TESSDATA_PREFIX env var") << std::endl;
            delete tessApi;
            tessApi = nullptr;
            return false;
        }
        
        // Configure for better accuracy
        tessApi->SetPageSegMode(tesseract::PSM_AUTO);
        
        initialized = true;
        std::cout << "Tesseract OCR initialized successfully" << std::endl;
        std::cout << "Language: " << language << std::endl;
        std::cout << "Version: " << tessApi->Version() << std::endl;
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "OCR initialization error: " << e.what() << std::endl;
        if (tessApi) {
            delete tessApi;
            tessApi = nullptr;
        }
        return false;
    }
}

TextRecognition::OCRResult TextRecognition::recognizeText(const cv::Mat& image, 
                                                          bool preprocess) {
    OCRResult result;
    
    if (!initialized || tessApi == nullptr) {
        result.text = "[OCR Not Initialized]\n\nTo enable OCR:\n"
                     "1. Check TESSDATA_PREFIX environment variable\n"
                     "2. Ensure language data files are installed\n"
                     "3. Restart application";
        result.success = false;
        result.confidence = 0.0f;
        return result;
    }
    
    try {
        cv::Mat processedImage;
        
        if (preprocess) {
            preprocessImage(image, processedImage);
        } else {
            // Convert to grayscale if needed
            if (image.channels() == 3) {
                cv::cvtColor(image, processedImage, cv::COLOR_BGR2GRAY);
            } else {
                processedImage = image.clone();
            }
        }
        
        // Set image for Tesseract
        tessApi->SetImage(processedImage.data, 
                         processedImage.cols, 
                         processedImage.rows, 
                         processedImage.channels(), 
                         processedImage.step);
        
        // Recognize text
        char* outText = tessApi->GetUTF8Text();
        
        if (outText != nullptr) {
            result.text = std::string(outText);
            delete[] outText;
        } else {
            result.text = "[No text detected]";
        }
        
        // Get confidence
        result.confidence = static_cast<float>(tessApi->MeanTextConf());
        result.success = true;
        
        // Additional info
        std::cout << "OCR completed successfully" << std::endl;
        std::cout << "Confidence: " << result.confidence << "%" << std::endl;
        std::cout << "Text length: " << result.text.length() << " characters" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "OCR recognition error: " << e.what() << std::endl;
        result.text = "[OCR Error]\n\n" + std::string(e.what());
        result.success = false;
        result.confidence = 0.0f;
    }
    
    return result;
}

void TextRecognition::preprocessImage(const cv::Mat& input, cv::Mat& output) {
    // Convert to grayscale if needed
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Denoise
    cv::Mat denoised;
    cv::fastNlMeansDenoising(gray, denoised, 10, 7, 21);
    
    // Adaptive thresholding for better text extraction
    cv::adaptiveThreshold(denoised, output, 255, 
                         cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
                         cv::THRESH_BINARY, 11, 2);
}

bool TextRecognition::isInitialized() {
    return initialized && (tessApi != nullptr);
}

std::vector<std::string> TextRecognition::getSupportedLanguages() {
    return {"eng", "ara", "fra", "deu", "spa", "chi_sim", "chi_tra", "jpn"};
}

void TextRecognition::cleanup() {
    if (tessApi != nullptr) {
        tessApi->End();
        delete tessApi;
        tessApi = nullptr;
    }
    initialized = false;
    std::cout << "Tesseract OCR cleaned up" << std::endl;
}
'@

# Write the new content
Set-Content -Path $cppFile -Value $newContent -NoNewline

Write-Host "? TextRecognition.cpp updated with Tesseract code" -ForegroundColor Green
Write-Host ""

Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host "  TESSERACT CODE ENABLED                                " -ForegroundColor Green
Write-Host "????????????????????????????????????????????????????????" -ForegroundColor Green
Write-Host ""

Write-Host "?? Next Steps:" -ForegroundColor Cyan
Write-Host "  1. Run: .\configure_visual_studio.ps1" -ForegroundColor White
Write-Host "     (Configure include/library paths)" -ForegroundColor Gray
Write-Host ""
Write-Host "  2. Rebuild project in Visual Studio" -ForegroundColor White
Write-Host "     (Build ? Rebuild Solution)" -ForegroundColor Gray
Write-Host ""
Write-Host "  3. Test OCR with real images!" -ForegroundColor White
Write-Host ""
