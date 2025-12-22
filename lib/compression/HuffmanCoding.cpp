#include "HuffmanCoding.h"
#include <queue>
#include <cmath>
#include <algorithm>

// Comparator for priority queue (min-heap by frequency)
struct CompareNode {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency;  // Min-heap
    }
};

HuffmanResult HuffmanCoding::encode(const cv::Mat& image) {
    HuffmanResult result;
    
    // Ensure grayscale
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }
    
    // Build frequency table
    result.frequencies = buildFrequencyTable(gray);
    
    // Build Huffman tree
    result.root = buildHuffmanTree(result.frequencies);
    
    // Generate code table
    generateCodeTable(result.root, "", result.codeTable);
    
    // Encode data
    result.encodedData = encodeData(gray, result.codeTable);
    
    // Calculate metrics
    int totalPixels = gray.rows * gray.cols;
    result.originalSize = totalPixels * 8;  // 8 bits per pixel
    result.compressedSize = result.encodedData.size();
    result.originalEntropy = calculateEntropy(result.frequencies, totalPixels);
    result.averageCodeLength = calculateAverageCodeLength(result.codeTable, result.frequencies, totalPixels);
    result.compressionRatio = static_cast<double>(result.originalSize) / result.compressedSize;
    result.efficiency = result.originalEntropy / result.averageCodeLength;
    
    return result;
}

cv::Mat HuffmanCoding::decode(const HuffmanResult& result, int rows, int cols) {
    cv::Mat decoded(rows, cols, CV_8U);
    
    auto currentNode = result.root;
    int pixelIndex = 0;
    
    for (bool bit : result.encodedData) {
        // Traverse tree
        currentNode = bit ? currentNode->right : currentNode->left;
        
        // Reached leaf - output value
        if (currentNode->isLeaf()) {
            int row = pixelIndex / cols;
            int col = pixelIndex % cols;
            decoded.at<uchar>(row, col) = static_cast<uchar>(currentNode->value);
            pixelIndex++;
            currentNode = result.root;  // Reset to root
            
            if (pixelIndex >= rows * cols) break;
        }
    }
    
    return decoded;
}

std::map<int, int> HuffmanCoding::buildFrequencyTable(const cv::Mat& image) {
    std::map<int, int> frequencies;
    
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int value = static_cast<int>(image.at<uchar>(i, j));
            frequencies[value]++;
        }
    }
    
    return frequencies;
}

std::shared_ptr<HuffmanNode> HuffmanCoding::buildHuffmanTree(const std::map<int, int>& frequencies) {
    // Priority queue for building tree (min-heap by frequency)
    std::priority_queue<
        std::shared_ptr<HuffmanNode>,
        std::vector<std::shared_ptr<HuffmanNode>>,
        CompareNode
    > pq;
    
    // Create leaf nodes for each symbol
    for (const auto& pair : frequencies) {
        pq.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }
    
    // Build tree bottom-up
    while (pq.size() > 1) {
        // Extract two nodes with minimum frequency
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        
        // Create internal node with combined frequency
        auto parent = std::make_shared<HuffmanNode>(-1, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        pq.push(parent);
    }
    
    return pq.top();  // Root of Huffman tree
}

void HuffmanCoding::generateCodeTable(
    const std::shared_ptr<HuffmanNode>& node,
    const std::string& code,
    std::map<int, std::string>& codeTable
) {
    if (!node) return;
    
    // Leaf node - store code
    if (node->isLeaf()) {
        codeTable[node->value] = code.empty() ? "0" : code;  // Handle single-symbol case
        return;
    }
    
    // Traverse left (0) and right (1)
    generateCodeTable(node->left, code + "0", codeTable);
    generateCodeTable(node->right, code + "1", codeTable);
}

std::vector<bool> HuffmanCoding::encodeData(const cv::Mat& image, const std::map<int, std::string>& codeTable) {
    std::vector<bool> encoded;
    
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int value = static_cast<int>(image.at<uchar>(i, j));
            const std::string& code = codeTable.at(value);
            
            for (char bit : code) {
                encoded.push_back(bit == '1');
            }
        }
    }
    
    return encoded;
}

double HuffmanCoding::calculateEntropy(const std::map<int, int>& frequencies, int totalPixels) {
    double entropy = 0.0;
    
    for (const auto& pair : frequencies) {
        double probability = static_cast<double>(pair.second) / totalPixels;
        if (probability > 0) {
            entropy -= probability * std::log2(probability);
        }
    }
    
    return entropy;
}

double HuffmanCoding::calculateAverageCodeLength(
    const std::map<int, std::string>& codeTable,
    const std::map<int, int>& frequencies,
    int totalPixels
) {
    double avgLength = 0.0;
    
    for (const auto& pair : codeTable) {
        int value = pair.first;
        int codeLength = pair.second.length();
        double probability = static_cast<double>(frequencies.at(value)) / totalPixels;
        avgLength += probability * codeLength;
    }
    
    return avgLength;
}

std::vector<std::string> HuffmanCoding::getTreeVisualization(const std::shared_ptr<HuffmanNode>& root) {
    std::vector<std::string> lines;
    
    if (!root) return lines;
    
    // Simple tree visualization (depth-first)
    std::function<void(const std::shared_ptr<HuffmanNode>&, const std::string&, bool)> traverse;
    traverse = [&](const std::shared_ptr<HuffmanNode>& node, const std::string& prefix, bool isLeft) {
        if (!node) return;
        
        std::string line = prefix;
        line += isLeft ? "???0: " : "???1: ";
        
        if (node->isLeaf()) {
            line += "Value=" + std::to_string(node->value) + " (freq=" + std::to_string(node->frequency) + ")";
        } else {
            line += "Internal (freq=" + std::to_string(node->frequency) + ")";
        }
        
        lines.push_back(line);
        
        if (!node->isLeaf()) {
            std::string newPrefix = prefix + (isLeft ? "?   " : "    ");
            if (node->left) traverse(node->left, newPrefix, true);
            if (node->right) traverse(node->right, newPrefix, false);
        }
    };
    
    lines.push_back("Root (freq=" + std::to_string(root->frequency) + ")");
    if (root->left) traverse(root->left, "", true);
    if (root->right) traverse(root->right, "", false);
    
    return lines;
}
