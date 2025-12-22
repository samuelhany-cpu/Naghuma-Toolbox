#pragma once
#ifndef OCRDIALOG_H
#define OCRDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QCheckBox>
#include <QComboBox>
#include <opencv2/opencv.hpp>
#include "../lib/ocr/TextRecognition.h"

/**
 * @brief OCR Dialog for text recognition from images
 * 
 * Modern Qt dialog for OCR operations with:
 * - Real-time preview
 * - Preprocessing controls
 * - Confidence metrics
 * - Word-level highlighting
 * - Export capabilities
 */
class OCRDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit OCRDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~OCRDialog();
    
    // Get recognized text
    QString getRecognizedText() const;
    
    // Get OCR confidence
    float getConfidence() const;
    
private slots:
    void onRecognizeClicked();
    void onPreprocessChanged();
    void onCopyTextClicked();
    void onSaveTextClicked();
    void onExportWithBoxesClicked();
    
private:
    void setupUI();
    void updatePreview();
    void displayResults(const TextRecognition::OCRResult& result);
    void drawBoundingBoxes(cv::Mat& image, const std::vector<cv::Rect>& boxes);
    
    // Original image
    cv::Mat originalImage;
    cv::Mat preprocessedImage;
    
    // OCR engine
    TextRecognition ocrEngine;
    
    // OCR results
    QString recognizedText;
    float confidence;
    TextRecognition::OCRResult lastResult;
    
    // UI Components
    QLabel* previewLabel;
    QTextEdit* resultText;
    QLabel* confidenceLabel;
    QProgressBar* confidenceBar;
    
    QPushButton* recognizeButton;
    QPushButton* copyButton;
    QPushButton* saveButton;
    QPushButton* exportButton;
    QPushButton* closeButton;
    
    // Preprocessing controls
    QCheckBox* grayscaleCheck;
    QCheckBox* thresholdCheck;
    QCheckBox* denoiseCheck;
    QCheckBox* contrastCheck;
    
    // Language selection
    QComboBox* languageCombo;
    
    // Status
    QLabel* statusLabel;
    
    // Theme colors (Catppuccin-inspired dark theme)
    const QString primaryColor = "#89B4FA";      // Soft blue
    const QString secondaryColor = "#CDD6F4";    // Light text
    const QString accentSuccess = "#A6E3A1";     // Mint green
    const QString accentDanger = "#F38BA8";      // Coral red
    const QString backgroundColor = "#1E1E2E";   // Deep dark
    const QString panelColor = "#2A2A3E";        // Panel background
    const QString hoverColor = "#363651";        // Hover state
};

#endif // OCRDIALOG_H
