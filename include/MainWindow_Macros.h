// MainWindow_Macros.h - Helper macros for code reduction
#ifndef MAINWINDOW_MACROS_H
#define MAINWINDOW_MACROS_H

// Macro for creating menu actions (reduces 8 lines to 1)
#define ADD_MENU_ACTION(menu, text, slot) \
    connect((menu)->addAction(text), &QAction::triggered, this, &MainWindow::slot)

// Macro for simple filters that don't need parameters
#define IMPLEMENT_SIMPLE_FILTER(funcName, processorFunc, layerName, layerType, successMsg) \
void MainWindow::funcName() { \
    if (!checkImageLoaded("apply filter")) return; \
    processorFunc(currentImage, processedImage); \
    recentlyProcessed = true; \
    updateDisplay(); \
    if (!processedImage.empty()) { \
        currentImage = processedImage.clone(); \
        auto operation = [](const cv::Mat& input) { \
            cv::Mat result; \
            processorFunc(input, result); \
            return result; \
        }; \
        rightSidebar->addLayer(layerName, layerType, processedImage, operation); \
        rightSidebar->updateHistogram(processedImage); \
    } \
    updateStatus(successMsg, "success"); \
}

// Macro for simple transforms
#define IMPLEMENT_SIMPLE_TRANSFORM(funcName, transformFunc, layerName, successMsg) \
void MainWindow::funcName() { \
    applySimpleTransform(transformFunc, layerName, successMsg); \
}

#endif // MAINWINDOW_MACROS_H
