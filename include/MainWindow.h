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
#include <opencv2/opencv.hpp>

class ImageCanvas;
class RightSidebarWidget;
class BrushTool;

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
    
    // Brush Tool
    void showBrushSettings();
    void applyBrushEffect();
    void toggleDrawingMode();
    void onCanvasMousePress(const QPoint& pos);
    void onCanvasMouseMove(const QPoint& pos);
    void onCanvasMouseRelease(const QPoint& pos);
    
    // Layer management
    void onLayerRemoveRequested(int layerIndex);
    void onLayersRemoveRequested(const QList<int>& layerIndices);

private:
    void setupUI();
    void createMenuBar();
    void createToolBar();
    void createCentralWidget();
    void createStatusBar();
    
    void updateDisplay();
    void updateStatus(const QString& message, const QString& type = "info", int progress = -1);
    void finalizeProcessing(const QString& layerName, const QString& layerType);
    void updateMetricsDisplay();

    // UI Components
    ImageCanvas *originalCanvas;
    ImageCanvas *processedCanvas;
    
    QLabel *originalInfoLabel;
    QLabel *processedInfoLabel;
    QLabel *metricsLabel;
    QLabel *statusLabel;
    QProgressBar *progressBar;
    
    RightSidebarWidget *rightSidebar;
    
    // Brush Tool
    BrushTool *brushTool;
    
    // Drawing mode
    bool drawingMode;
    cv::Mat drawingCanvas;
    
    // Image data
    cv::Mat originalImage;
    cv::Mat currentImage;
    cv::Mat processedImage;
    QString imagePath;
    
    // Processing state
    bool imageLoaded;
    bool recentlyProcessed;
};

#endif // MAINWINDOW_H
