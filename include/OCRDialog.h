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
    
    // Status
    QLabel* statusLabel;
    
    // Theme colors
    const QString primaryColor = "#e879f9";
    const QString secondaryColor = "#c4b5fd";
    const QString backgroundColor = "#1e1b29";
    const QString panelColor = "#2a2438";
};

#endif // OCRDIALOG_H
