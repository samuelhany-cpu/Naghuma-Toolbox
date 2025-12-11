#ifndef TRANSFORMDIALOG_H
#define TRANSFORMDIALOG_H

#include <QDialog>
#include <QObject>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <opencv2/opencv.hpp>

class ImageCanvas;

// Base class for transform dialogs with live preview
class TransformDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransformDialog(const cv::Mat& sourceImage, QWidget *parent = nullptr);
    virtual ~TransformDialog();

    cv::Mat getTransformedImage() const { return transformedImage; }
    bool wasApplied() const { return applied; }

signals:
    void previewUpdated(const cv::Mat& preview);

protected:
    virtual void applyTransform() = 0;
    virtual void updatePreview() = 0;
    
    void setupBaseUI(const QString& title);
    void emitPreview(const cv::Mat& preview);
    
    cv::Mat sourceImage;
    cv::Mat transformedImage;
    bool applied;

private slots:
    void onApplyClicked();
    void onCancelClicked();

private:
    QPushButton *applyButton;
    QPushButton *cancelButton;
};

// Translation Dialog
class TranslationDialog : public TransformDialog {
    Q_OBJECT

public:
    explicit TranslationDialog(const cv::Mat& sourceImage, QWidget *parent = nullptr);
    
    int getTranslationX() const { return tx; }
    int getTranslationY() const { return ty; }

protected:
    void applyTransform() override;
    void updatePreview() override;

private:
    QSlider *sliderX;
    QSlider *sliderY;
    QSpinBox *spinBoxX;
    QSpinBox *spinBoxY;
    int tx, ty;
};

// Rotation Dialog
class RotationDialog : public TransformDialog {
    Q_OBJECT

public:
    explicit RotationDialog(const cv::Mat& sourceImage, QWidget *parent = nullptr);
    
    double getAngle() const { return angle; }

protected:
    void applyTransform() override;
    void updatePreview() override;

private:
    QSlider *angleSlider;
    QDoubleSpinBox *angleSpinBox;
    double angle;
};

// Zoom Dialog
class ZoomDialog : public TransformDialog {
    Q_OBJECT

public:
    explicit ZoomDialog(const cv::Mat& sourceImage, QWidget *parent = nullptr);
    
    double getScale() const { return scale; }

protected:
    void applyTransform() override;
    void updatePreview() override;

private:
    QSlider *zoomSlider;
    QDoubleSpinBox *zoomSpinBox;
    double scale;
};

// Skew Dialog
class SkewDialog : public TransformDialog {
    Q_OBJECT

public:
    explicit SkewDialog(const cv::Mat& sourceImage, QWidget *parent = nullptr);
    
    double getSkewX() const { return skewX; }
    double getSkewY() const { return skewY; }

protected:
    void applyTransform() override;
    void updatePreview() override;

private:
    QSlider *skewXSlider;
    QSlider *skewYSlider;
    QDoubleSpinBox *skewXSpinBox;
    QDoubleSpinBox *skewYSpinBox;
    double skewX, skewY;
};

#endif // TRANSFORMDIALOG_H
