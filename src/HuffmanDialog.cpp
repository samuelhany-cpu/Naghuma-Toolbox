#include "HuffmanDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QApplication>
#include <sstream>
#include <iomanip>

HuffmanDialog::HuffmanDialog(const cv::Mat& image, QWidget *parent)
    : QDialog(parent), originalImage(image.clone()), applied(false), encoded(false) {
    
    setWindowTitle("Huffman Coding - Lossless Compression");
    setMinimumSize(700, 600);
    setupUI();
}

HuffmanDialog::~HuffmanDialog() {
}

void HuffmanDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *title = new QLabel("Huffman Coding Compression");
    title->setStyleSheet("font-size: 14pt; font-weight: bold; color: #e879f9; padding: 10px;");
    mainLayout->addWidget(title);
    
    // Info label
    QLabel *info = new QLabel(
        "Huffman coding is a lossless compression algorithm that assigns variable-length codes "
        "based on symbol frequencies. More frequent symbols get shorter codes."
    );
    info->setWordWrap(true);
    info->setStyleSheet("color: #c4b5fd; padding: 10px; background: rgba(43, 45, 66, 0.3); border-radius: 5px;");
    mainLayout->addWidget(info);
    
    // Status label
    statusLabel = new QLabel("Status: Ready to encode");
    statusLabel->setStyleSheet("color: #73D2DE; padding: 5px; font-weight: bold;");
    mainLayout->addWidget(statusLabel);
    
    // Metrics display
    metricsLabel = new QLabel();
    metricsLabel->setStyleSheet(
        "background: rgba(43, 45, 66, 0.5); "
        "color: #A594F9; "
        "padding: 15px; "
        "border-radius: 8px; "
        "font-family: 'Consolas', monospace;"
    );
    metricsLabel->setVisible(false);
    mainLayout->addWidget(metricsLabel);
    
    // Result text area
    resultText = new QTextEdit();
    resultText->setReadOnly(true);
    resultText->setStyleSheet(
        "background: rgba(43, 45, 66, 0.4); "
        "color: #EDF2F4; "
        "border: 2px solid rgba(165, 148, 249, 0.3); "
        "border-radius: 10px; "
        "padding: 10px; "
        "font-family: 'Consolas', monospace; "
        "font-size: 10pt;"
    );
    resultText->setMaximumHeight(200);
    mainLayout->addWidget(resultText);
    
    // Progress bar
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    progressBar->setStyleSheet(
        "QProgressBar { "
        "   border: 2px solid rgba(165, 148, 249, 0.3); "
        "   border-radius: 5px; "
        "   text-align: center; "
        "   background: rgba(43, 45, 66, 0.5); "
        "   color: #A594F9; "
        "} "
        "QProgressBar::chunk { "
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #A594F9, stop:1 #73D2DE); "
        "   border-radius: 3px; "
        "}"
    );
    mainLayout->addWidget(progressBar);
    
    // Buttons layout
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    
    encodeButton = new QPushButton("1. Encode Image");
    encodeButton->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #A594F9, stop:1 #73D2DE); "
        "color: white; "
        "border: none; "
        "padding: 10px 20px; "
        "border-radius: 8px; "
        "font-weight: bold;"
    );
    connect(encodeButton, &QPushButton::clicked, this, &HuffmanDialog::onEncodeClicked);
    buttonsLayout->addWidget(encodeButton);
    
    decodeButton = new QPushButton("2. Decode Image");
    decodeButton->setEnabled(false);
    decodeButton->setStyleSheet(
        "background: rgba(115, 210, 222, 0.5); "
        "color: white; "
        "border: none; "
        "padding: 10px 20px; "
        "border-radius: 8px; "
        "font-weight: bold;"
    );
    connect(decodeButton, &QPushButton::clicked, this, &HuffmanDialog::onDecodeClicked);
    buttonsLayout->addWidget(decodeButton);
    
    mainLayout->addLayout(buttonsLayout);
    
    // Info buttons
    QHBoxLayout *infoButtonsLayout = new QHBoxLayout();
    
    showTreeButton = new QPushButton("Show Huffman Tree");
    showTreeButton->setEnabled(false);
    showTreeButton->setStyleSheet(
        "background: rgba(165, 148, 249, 0.2); "
        "color: #A594F9; "
        "border: 1px solid rgba(165, 148, 249, 0.5); "
        "padding: 8px 16px; "
        "border-radius: 6px;"
    );
    connect(showTreeButton, &QPushButton::clicked, this, &HuffmanDialog::onShowTreeClicked);
    infoButtonsLayout->addWidget(showTreeButton);
    
    showCodeTableButton = new QPushButton("Show Code Table");
    showCodeTableButton->setEnabled(false);
    showCodeTableButton->setStyleSheet(
        "background: rgba(165, 148, 249, 0.2); "
        "color: #A594F9; "
        "border: 1px solid rgba(165, 148, 249, 0.5); "
        "padding: 8px 16px; "
        "border-radius: 6px;"
    );
    connect(showCodeTableButton, &QPushButton::clicked, this, &HuffmanDialog::onShowCodeTableClicked);
    infoButtonsLayout->addWidget(showCodeTableButton);
    
    mainLayout->addLayout(infoButtonsLayout);
    
    // Bottom buttons
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    
    applyButton = new QPushButton("Apply & Close");
    applyButton->setEnabled(false);
    applyButton->setProperty("class", "accent");
    applyButton->setMinimumWidth(120);
    connect(applyButton, &QPushButton::clicked, this, &HuffmanDialog::onApplyClicked);
    bottomLayout->addWidget(applyButton);
    
    QPushButton *cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumWidth(100);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    bottomLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(bottomLayout);
}

void HuffmanDialog::onEncodeClicked() {
    statusLabel->setText("Status: Encoding...");
    progressBar->setVisible(true);
    progressBar->setValue(0);
    QApplication::processEvents();
    
    try {
        // Encode image
        progressBar->setValue(30);
        result = HuffmanCoding::encode(originalImage);
        
        progressBar->setValue(70);
        
        // Store encoded image (just for visualization - still grayscale)
        if (originalImage.channels() == 3) {
            cv::cvtColor(originalImage, encodedImage, cv::COLOR_BGR2GRAY);
        } else {
            encodedImage = originalImage.clone();
        }
        
        progressBar->setValue(100);
        
        encoded = true;
        statusLabel->setText("Status: Encoding complete!");
        updateMetricsDisplay();
        
        // Enable buttons
        decodeButton->setEnabled(true);
        showTreeButton->setEnabled(true);
        showCodeTableButton->setEnabled(true);
        
        resultText->setText("? Huffman tree built successfully\n"
                           "? Code table generated\n"
                           "? Image data encoded\n\n"
                           "Click 'Decode Image' to verify lossless compression.");
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Encoding failed: %1").arg(e.what()));
        statusLabel->setText("Status: Encoding failed");
    }
    
    progressBar->setVisible(false);
}

void HuffmanDialog::onDecodeClicked() {
    statusLabel->setText("Status: Decoding...");
    progressBar->setVisible(true);
    progressBar->setValue(0);
    QApplication::processEvents();
    
    try {
        // Decode image
        progressBar->setValue(50);
        decodedImage = HuffmanCoding::decode(result, originalImage.rows, originalImage.cols);
        
        progressBar->setValue(100);
        
        // Verify lossless
        cv::Mat originalGray;
        if (originalImage.channels() == 3) {
            cv::cvtColor(originalImage, originalGray, cv::COLOR_BGR2GRAY);
        } else {
            originalGray = originalImage.clone();
        }
        
        bool isLossless = cv::countNonZero(originalGray != decodedImage) == 0;
        
        if (isLossless) {
            statusLabel->setText("Status: ? Lossless compression verified!");
            resultText->setText("? Image decoded successfully\n"
                               "? Lossless compression confirmed (100% identical to original)\n\n"
                               "You can now apply and close, or view the code table/tree.");
            applyButton->setEnabled(true);
        } else {
            statusLabel->setText("Status: Warning - Decoding mismatch!");
            resultText->setText("? Decoded image differs from original\n"
                               "This should not happen with Huffman coding.");
        }
        
        emit previewUpdated(decodedImage);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Decoding failed: %1").arg(e.what()));
        statusLabel->setText("Status: Decoding failed");
    }
    
    progressBar->setVisible(false);
}

void HuffmanDialog::onShowTreeClicked() {
    displayTree();
}

void HuffmanDialog::onShowCodeTableClicked() {
    displayCodeTable();
}

void HuffmanDialog::onApplyClicked() {
    applied = true;
    accept();
}

void HuffmanDialog::updateMetricsDisplay() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    
    oss << "?? Huffman Coding Metrics\n";
    oss << "???????????????????????????????????????\n\n";
    
    oss << "Original Size:        " << result.originalSize << " bits\n";
    oss << "Compressed Size:      " << result.compressedSize << " bits\n";
    oss << "Compression Ratio:    " << std::setprecision(2) << result.compressionRatio << ":1\n";
    oss << "Space Savings:        " << std::setprecision(1) 
        << (100.0 * (1.0 - 1.0/result.compressionRatio)) << "%\n\n";
    
    oss << "Original Entropy H(X): " << std::setprecision(4) << result.originalEntropy << " bits/symbol\n";
    oss << "Avg Code Length L:     " << result.averageCodeLength << " bits/symbol\n";
    oss << "Coding Efficiency:     " << std::setprecision(2) << (result.efficiency * 100) << "%\n\n";
    
    oss << "Unique Symbols:       " << result.frequencies.size() << " / 256 possible\n";
    oss << "Code Table Entries:   " << result.codeTable.size();
    
    metricsLabel->setText(QString::fromStdString(oss.str()));
    metricsLabel->setVisible(true);
}

void HuffmanDialog::displayCodeTable() {
    QDialog *tableDialog = new QDialog(this);
    tableDialog->setWindowTitle("Huffman Code Table");
    tableDialog->setMinimumSize(500, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(tableDialog);
    
    QLabel *title = new QLabel("Huffman Code Assignment");
    title->setStyleSheet("font-size: 12pt; font-weight: bold; color: #e879f9; padding: 10px;");
    layout->addWidget(title);
    
    QTextEdit *tableText = new QTextEdit();
    tableText->setReadOnly(true);
    tableText->setStyleSheet(
        "background: rgba(43, 45, 66, 0.5); "
        "color: #EDF2F4; "
        "padding: 10px; "
        "font-family: 'Consolas', monospace; "
        "font-size: 9pt;"
    );
    
    std::ostringstream oss;
    oss << "Value | Frequency | Code        | Length\n";
    oss << "------+-----------+-------------+-------\n";
    
    // Sort by frequency (descending)
    std::vector<std::pair<int, int>> sortedFreq(result.frequencies.begin(), result.frequencies.end());
    std::sort(sortedFreq.begin(), sortedFreq.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (const auto& pair : sortedFreq) {
        int value = pair.first;
        int freq = pair.second;
        const std::string& code = result.codeTable.at(value);
        
        oss << std::setw(5) << value << " | "
            << std::setw(9) << freq << " | "
            << std::setw(11) << std::left << code << std::right << " | "
            << std::setw(6) << code.length() << "\n";
    }
    
    tableText->setText(QString::fromStdString(oss.str()));
    layout->addWidget(tableText);
    
    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setProperty("class", "accent");
    connect(closeBtn, &QPushButton::clicked, tableDialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    tableDialog->exec();
}

void HuffmanDialog::displayTree() {
    QDialog *treeDialog = new QDialog(this);
    treeDialog->setWindowTitle("Huffman Tree Visualization");
    treeDialog->setMinimumSize(600, 700);
    
    QVBoxLayout *layout = new QVBoxLayout(treeDialog);
    
    QLabel *title = new QLabel("Huffman Tree Structure");
    title->setStyleSheet("font-size: 12pt; font-weight: bold; color: #e879f9; padding: 10px;");
    layout->addWidget(title);
    
    QLabel *info = new QLabel(
        "Tree shows how symbols are assigned codes based on frequencies.\n"
        "Left branches = 0, Right branches = 1"
    );
    info->setWordWrap(true);
    info->setStyleSheet("color: #c4b5fd; padding: 5px;");
    layout->addWidget(info);
    
    QTextEdit *treeText = new QTextEdit();
    treeText->setReadOnly(true);
    treeText->setStyleSheet(
        "background: rgba(43, 45, 66, 0.5); "
        "color: #EDF2F4; "
        "padding: 10px; "
        "font-family: 'Consolas', monospace; "
        "font-size: 9pt;"
    );
    
    auto treeLines = HuffmanCoding::getTreeVisualization(result.root);
    QString treeStr;
    for (const auto& line : treeLines) {
        treeStr += QString::fromStdString(line) + "\n";
    }
    
    // Limit display if tree is too large
    if (treeLines.size() > 100) {
        treeStr = "Tree is very large (" + QString::number(treeLines.size()) + " nodes).\n";
        treeStr += "Showing first 100 lines...\n\n";
        for (size_t i = 0; i < 100; i++) {
            treeStr += QString::fromStdString(treeLines[i]) + "\n";
        }
    }
    
    treeText->setText(treeStr);
    layout->addWidget(treeText);
    
    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setProperty("class", "accent");
    connect(closeBtn, &QPushButton::clicked, treeDialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    treeDialog->exec();
}
#include "moc_HuffmanDialog.cpp"
