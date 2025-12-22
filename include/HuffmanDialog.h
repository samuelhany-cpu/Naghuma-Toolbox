#ifndef HUFFMANDIALOG_H
#define HUFFMANDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <opencv2/opencv.hpp>
#include "../lib/compression/HuffmanCoding.h"

class HuffmanDialog : public QDialog {
    Q_OBJECT

public:
    explicit HuffmanDialog(const cv::Mat& image, QWidget *parent = nullptr);
    ~HuffmanDialog();

    cv::Mat getEncodedImage() const { return encodedImage; }
    cv::Mat getDecodedImage() const { return decodedImage; }
    bool wasApplied() const { return applied; }
    const HuffmanResult& getResult() const { return result; }

signals:
    void previewUpdated(const cv::Mat& preview);

private slots:
    void onEncodeClicked();
    void onDecodeClicked();
    void onShowTreeClicked();
    void onShowCodeTableClicked();
    void onApplyClicked();

private:
    void setupUI();
    void updateMetricsDisplay();
    void displayCodeTable();
    void displayTree();

    cv::Mat originalImage;
    cv::Mat encodedImage;
    cv::Mat decodedImage;
    HuffmanResult result;
    bool applied;
    bool encoded;

    // UI elements
    QLabel* statusLabel;
    QLabel* metricsLabel;
    QTextEdit* resultText;
    QPushButton* encodeButton;
    QPushButton* decodeButton;
    QPushButton* showTreeButton;
    QPushButton* showCodeTableButton;
    QPushButton* applyButton;
    QProgressBar* progressBar;
};

#endif // HUFFMANDIALOG_H
