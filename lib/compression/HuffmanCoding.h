#ifndef HUFFMANCODING_H
#define HUFFMANCODING_H

#include <opencv2/opencv.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>

// Huffman tree node
struct HuffmanNode {
    int value;          // Pixel value (0-255), -1 for internal nodes
    int frequency;      // Frequency count
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;
    
    HuffmanNode(int val, int freq) 
        : value(val), frequency(freq), left(nullptr), right(nullptr) {}
    
    bool isLeaf() const { return left == nullptr && right == nullptr; }
};

// Huffman coding results
struct HuffmanResult {
    std::map<int, std::string> codeTable;    // Value -> Binary code
    std::map<int, int> frequencies;          // Value -> Frequency
    std::vector<bool> encodedData;           // Encoded bitstream
    double originalEntropy;                  // H(X) = -? p(x)log2(p(x))
    double averageCodeLength;                // L = ? p(x)l(x)
    double compressionRatio;                 // Original bits / Encoded bits
    double efficiency;                       // H(X) / L
    size_t originalSize;                     // In bits
    size_t compressedSize;                   // In bits
    std::shared_ptr<HuffmanNode> root;       // Huffman tree root
};

class HuffmanCoding {
public:
    // Build Huffman tree and encode grayscale image
    static HuffmanResult encode(const cv::Mat& image);
    
    // Decode bitstream back to image
    static cv::Mat decode(const HuffmanResult& result, int rows, int cols);
    
    // Calculate entropy H(X) = -? p(x)log2(p(x))
    static double calculateEntropy(const std::map<int, int>& frequencies, int totalPixels);
    
    // Calculate average code length L = ? p(x)l(x)
    static double calculateAverageCodeLength(
        const std::map<int, std::string>& codeTable,
        const std::map<int, int>& frequencies,
        int totalPixels
    );
    
    // Get tree visualization data (for display purposes)
    static std::vector<std::string> getTreeVisualization(const std::shared_ptr<HuffmanNode>& root);
    
private:
    // Build frequency table from image
    static std::map<int, int> buildFrequencyTable(const cv::Mat& image);
    
    // Build Huffman tree from frequencies
    static std::shared_ptr<HuffmanNode> buildHuffmanTree(const std::map<int, int>& frequencies);
    
    // Generate code table from tree
    static void generateCodeTable(
        const std::shared_ptr<HuffmanNode>& node,
        const std::string& code,
        std::map<int, std::string>& codeTable
    );
    
    // Encode image data using code table
    static std::vector<bool> encodeData(const cv::Mat& image, const std::map<int, std::string>& codeTable);
};

#endif // HUFFMANCODING_H
