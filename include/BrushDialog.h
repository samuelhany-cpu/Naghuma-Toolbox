#ifndef BRUSHDIALOG_H
#define BRUSHDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QColorDialog>
#include <QCheckBox>
#include <QLabel>
#include "BrushTool.h"

class BrushDialog : public QDialog {
    Q_OBJECT

public:
    explicit BrushDialog(BrushTool* brushTool, QWidget *parent = nullptr);
    ~BrushDialog();

    int getBrushSize() const { return brushTool->getBrushSize(); }
    int getBrushOpacity() const { return brushTool->getBrushOpacity(); }
    QColor getBrushColor() const { return brushTool->getBrushColor(); }
    bool isEraserMode() const { return brushTool->isEraserMode(); }
    QString getSelectedFilter() const { return selectedFilter; }

signals:
    void previewUpdated(const cv::Mat& preview);

private slots:
    void onBrushTypeChanged(int index);
    void onBrushSizeChanged(int value);
    void onOpacityChanged(int value);
    void onColorClicked();
    void onEraserToggled(bool checked);
    void onFilterChanged(int index);

private:
    void setupUI();
    void updateBrushPreview();

    BrushTool* brushTool;
    
    QComboBox *brushTypeCombo;
    QSlider *sizeSlider;
    QSpinBox *sizeSpinBox;
    QSlider *opacitySlider;
    QSpinBox *opacitySpinBox;
    QPushButton *colorButton;
    QCheckBox *eraserCheckBox;
    QComboBox *filterCombo;
    QLabel *previewLabel;
    
    QString selectedFilter;
};

#endif // BRUSHDIALOG_H
