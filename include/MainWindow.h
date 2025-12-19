#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include <opencv2/opencv.hpp>
#include <functional>

class ImageCanvas;
class RightSidebarWidget;
class CropTool;
class CollapsibleToolbar;
class ROIManager;
class RectangleROI;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void loadImage();
    void saveImage();
    void resetImage();
    void useProcessedImage();
    void undoLastOperation();  // NEW: Undo functionality
    
    // Information
    void showImageInfo();
    void showPixelInfo();
    void showImageStats();
    void showImageMetrics();
    
    // Geometric Transformations
    void applyTranslation();
    void applyRotation();
    void applySkew();
    void applyZoom();
    void applyFlipX();
    void applyFlipY();
    void applyFlipXY();
    
    // Crop Tool
    void toggleCropMode();
    void applyCrop();
    void cancelCrop();
    void onCropMousePress(const QPoint& pos);
    void onCropMouseMove(const QPoint& pos);
    void onCropMouseRelease(const QPoint& pos);
    
    // Zoom and View
    void zoomIn();
    void zoomOut();
    void fitToWindow();
    void actualSize();
    void onZoomChanged(double level);
    
    // ROI Tools - NEW
    void toggleROIMode();
    void showROIStatistics();
    void saveROIs();
    void loadROIs();
    void onROIMousePress(const QPoint& pos);
    void onROIMouseMove(const QPoint& pos);
    void onROIMouseRelease(const QPoint& pos);
    
    // Compression
    void applyCompression();
    
    // Auto Enhancement
    void applyAutoEnhancement();
    
    // Noise Removal
    void applyNoiseRemoval();
    
    // Histogram & Thresholding
    void showHistogram();
    void applyHistogramEqualization();
    void applyOtsuThresholding();
    
    // Image Processing
    void applyBrightnessContrast();
    void convertToGrayscale();
    void applyBinaryThreshold();
    void applyGaussianBlur();
    void applyEdgeDetection();
    void invertColors();
    
    // Filters
    void applyLaplacianFilter();
    void applySobelCombinedFilter();
    void applyTraditionalFilter();
    void applyPyramidalFilter();
    void applyCircularFilter();
    void applyConeFilter();
    
    // Phase 13: Basic Edge Detectors
    void applyPrewittEdge();
    void applyPrewittX();
    void applyPrewittY();
    void applyRobertsCross();
    void applyLoG();
    void applyDoG();
    
    // Phase 14: Color Space Conversions
    void showColorConversionDialog();
    
    // Phase 15: Color Processing Operations
    void applyChannelEqualization();
    void applyAutoWhiteBalance();
    void applyGammaCorrection();
    void applyPseudocolor();
    void applyGrayLevelSlicing();
    void applyBitPlaneSlicing();
    
    // Phase 16: Image Segmentation - Thresholding
    void showThresholdingDialog();
    
    // Phase 17: Advanced Segmentation - NEW
    void showAdvancedSegmentationDialog();
    
    // Phase 19: Feature Detection - NEW
    void showFeatureDetectionDialog();
    
    // Phase 19: Advanced Frequency Filters - NEW
    void showFrequencyFilterDialog();

    // Morphology Operations
    void applyErosion();
    void applyDilation();
    void applyOpening();
    void applyClosing();
    void applyMorphGradient();
    
    // FFT Operations
    void applyLowPassFilter();
    void applyHighPassFilter();
    void showFFTSpectrum();
    
    // Layer management
    void onLayerRemoveRequested(int layerIndex);
    void onLayersRemoveRequested(const QList<int>& layerIndices);
    void updateUndoButtonState();  // NEW: Update undo button enabled state

private:
    // UI Setup
    void setupUI();
    void createMenuBar();
    void createToolBar();
    void createCentralWidget();
    void createStatusBar();
    
    // Event handlers
    void keyPressEvent(QKeyEvent *event) override;
    
    // Helper Functions
    void updateDisplay();
    void updateStatus(const QString& message, const QString& type = "info", int progress = -1);
    void finalizeProcessing(const QString& layerName, const QString& layerType);
    void updateMetricsDisplay();
    
    // Common Operation Helpers
    bool checkImageLoaded(const QString& operation = "perform this operation");
    void applySimpleFilter(
        std::function<void(const cv::Mat&, cv::Mat&)> filterFunc,
        std::function<cv::Mat(const cv::Mat&)> operationFunc,
        const QString& layerName,
        const QString& layerType,
        const QString& successMessage
    );

    // UI Components
    CollapsibleToolbar *leftToolbar;
    ImageCanvas *originalCanvas;
    ImageCanvas *processedCanvas;
    
    QLabel *originalInfoLabel;
    QLabel *processedInfoLabel;
    QLabel *metricsLabel;
    QLabel *statusLabel;
    QLabel *zoomLabel;  // NEW: Zoom level display
    QProgressBar *progressBar;
    
    RightSidebarWidget *rightSidebar;
    
    QPushButton *undoButton;  // NEW: Undo button reference
    
    // Image data
    cv::Mat originalImage;
    cv::Mat currentImage;
    cv::Mat processedImage;
    QString imagePath;
    
    // Processing state
    bool imageLoaded;
    bool recentlyProcessed;
    
    // Crop tool
    CropTool *cropTool;
    bool cropMode;
    cv::Mat cropPreviewImage;
    
    // ROI tool - NEW
    ROIManager *roiManager;
    bool roiMode;
    QPoint roiStartPoint;
    QPoint roiEndPoint;
    bool roiSelecting;
    RectangleROI *currentROI;


};

#endif // MAINWINDOW_H
