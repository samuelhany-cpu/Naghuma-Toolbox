#ifndef THEME_H
#define THEME_H

#include <QString>

namespace Theme {
    // Color constants
    constexpr const char* BG_PRIMARY = "#1a0a1f";
    constexpr const char* BG_SECONDARY = "#1f1535";
    constexpr const char* FG_PRIMARY = "#f3e8ff";
    constexpr const char* FG_SECONDARY = "#c4b5fd";
    constexpr const char* ACCENT_PRIMARY = "#e879f9";
    constexpr const char* ACCENT_SECONDARY = "#c026d3";
    constexpr const char* SUCCESS_COLOR = "#98d8e8";
    constexpr const char* INFO_COLOR = "#c4b5fd";
    constexpr const char* WARNING_COLOR = "#fb7185";
    constexpr const char* ERROR_COLOR = "#f87171";
    constexpr const char* HOVER_GLOW = "rgba(232, 121, 249, 0.3)";

    // Animation durations (in ms)
    constexpr int ANIMATION_FAST = 150;
    constexpr int ANIMATION_NORMAL = 250;
    constexpr int ANIMATION_SLOW = 350;

    // Main application stylesheet (Qt-compatible only, no CSS3)
    const char* const MAIN_STYLESHEET = R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #1a0a1f, stop:0.3 #251e35, 
                                       stop:0.7 #1f1535, stop:1 #1a0a1f);
        }
        
        QWidget {
            background-color: #1f1535;
            color: #f3e8ff;
            font-family: 'Segoe UI', -apple-system, sans-serif;
            font-size: 10pt;
        }
        
        /* Enhanced Button Styles */
        QPushButton {
            background-color: rgba(45, 37, 71, 0.8);
            color: #c4b5fd;
            border: 1px solid rgba(61, 50, 80, 0.6);
            border-radius: 10px;
            padding: 12px 24px;
            font-weight: 500;
        }
        
        QPushButton:hover {
            background-color: rgba(91, 75, 115, 0.6);
            color: #e879f9;
            border: 1px solid rgba(232, 121, 249, 0.5);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #e879f9, stop:1 #c026d3);
            color: #ffffff;
            border: none;
        }
        
        QPushButton.accent {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #e879f9, stop:1 #c026d3);
            color: #ffffff;
            font-weight: 600;
            border: none;
            padding: 12px 28px;
        }
        
        QPushButton.accent:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #f0abfc, stop:1 #e879f9);
        }
        
        QPushButton:disabled {
            background-color: rgba(45, 37, 71, 0.4);
            color: rgba(196, 181, 253, 0.4);
            border: 1px solid rgba(61, 50, 80, 0.3);
        }
        
        /* Form Elements */
        QLineEdit, QSpinBox, QDoubleSpinBox {
            background-color: rgba(45, 37, 71, 0.5);
            color: #f3e8ff;
            border: 2px solid rgba(91, 75, 115, 0.5);
            border-radius: 8px;
            padding: 10px 14px;
        }
        
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #e879f9;
            background-color: rgba(232, 121, 249, 0.08);
        }
        
        /* Enhanced Slider */
        QSlider::groove:horizontal {
            background: rgba(45, 37, 71, 0.6);
            height: 10px;
            border-radius: 5px;
        }
        
        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #e879f9, stop:1 #c026d3);
            width: 22px;
            height: 22px;
            margin: -6px 0;
            border-radius: 11px;
            border: 2px solid rgba(255, 255, 255, 0.3);
        }
        
        QSlider::handle:horizontal:hover {
            background: #f0abfc;
        }
        
        QSlider::sub-page:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #e879f9, stop:1 #c026d3);
            border-radius: 5px;
        }
        
        /* Progress Bar */
        QProgressBar {
            background-color: rgba(45, 37, 71, 0.6);
            border: none;
            border-radius: 8px;
            height: 14px;
            text-align: center;
            color: #ffffff;
            font-weight: 500;
        }
        
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #e879f9, stop:1 #c026d3);
            border-radius: 8px;
        }
        
        QLabel {
            background-color: transparent;
            color: #c4b5fd;
        }
        
        /* Enhanced GroupBox */
        QGroupBox {
            border: 2px solid rgba(232, 121, 249, 0.35);
            border-radius: 14px;
            margin-top: 22px;
            padding: 24px 20px 20px 20px;
            font-weight: 600;
            color: #e879f9;
            background-color: rgba(45, 37, 71, 0.25);
        }
        
        QGroupBox:hover {
            border: 2px solid rgba(232, 121, 249, 0.5);
            background-color: rgba(45, 37, 71, 0.35);
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 16px;
            background-color: #1f1535;
            font-size: 11pt;
        }
        
        /* Status Bar */
        QStatusBar {
            background: rgba(26, 10, 31, 0.95);
            color: #f3e8ff;
            border-top: 2px solid rgba(232, 121, 249, 0.25);
            padding: 6px;
        }
        
        /* Menu Bar */
        QMenuBar {
            background: rgba(31, 21, 53, 0.95);
            color: #f3e8ff;
            spacing: 6px;
            padding: 10px 8px;
            border-bottom: 2px solid rgba(232, 121, 249, 0.25);
        }
        
        QMenuBar::item {
            padding: 10px 20px;
            background: transparent;
            border-radius: 8px;
        }
        
        QMenuBar::item:selected {
            background-color: rgba(232, 121, 249, 0.15);
            color: #e879f9;
        }
        
        QMenuBar::item:pressed {
            background-color: rgba(232, 121, 249, 0.25);
        }
        
        /* Menu */
        QMenu {
            background-color: rgba(37, 30, 53, 0.98);
            border: 2px solid rgba(91, 75, 115, 0.5);
            border-radius: 12px;
            padding: 10px;
        }
        
        QMenu::item {
            padding: 12px 40px;
            color: #f3e8ff;
            border-radius: 8px;
        }
        
        QMenu::item:selected {
            background-color: rgba(232, 121, 249, 0.2);
            color: #e879f9;
        }
        
        QMenu::separator {
            height: 1px;
            background: rgba(232, 121, 249, 0.2);
            margin: 6px 14px;
        }
        
        /* Toolbar */
        QToolBar {
            background: rgba(26, 10, 31, 0.95);
            border-bottom: 2px solid rgba(232, 121, 249, 0.25);
            spacing: 12px;
            padding: 14px;
        }
        
        QToolBar::separator {
            background: rgba(232, 121, 249, 0.3);
            width: 2px;
            margin: 0 8px;
        }
        
        /* Text Edit */
        QTextEdit {
            background-color: rgba(45, 37, 71, 0.5);
            color: #f3e8ff;
            border: 2px solid rgba(91, 75, 115, 0.5);
            border-radius: 10px;
            padding: 14px;
            selection-background-color: rgba(232, 121, 249, 0.4);
        }
        
        QTextEdit:focus {
            border: 2px solid rgba(232, 121, 249, 0.6);
        }
        
        /* Dialog Styling */
        QMessageBox {
            background-color: #1f1535;
        }
        
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #1a0a1f, stop:1 #251e35);
        }
        
        /* Radio Buttons */
        QRadioButton {
            spacing: 8px;
            color: #c4b5fd;
        }
        
        QRadioButton::indicator {
            width: 20px;
            height: 20px;
            border-radius: 10px;
            border: 2px solid rgba(196, 181, 253, 0.5);
            background-color: rgba(45, 37, 71, 0.5);
        }
        
        QRadioButton::indicator:checked {
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5,
                                       fx:0.5, fy:0.5,
                                       stop:0 #e879f9, stop:0.6 #c026d3, stop:1 transparent);
            border: 2px solid #e879f9;
        }
        
        QRadioButton::indicator:hover {
            border: 2px solid #e879f9;
        }
        
        /* Combo Box */
        QComboBox {
            background-color: rgba(45, 37, 71, 0.8);
            color: #f3e8ff;
            border: 2px solid rgba(91, 75, 115, 0.5);
            border-radius: 8px;
            padding: 8px 14px;
            min-width: 120px;
        }
        
        QComboBox:hover {
            border: 2px solid rgba(232, 121, 249, 0.5);
        }
        
        QComboBox::drop-down {
            border: none;
            width: 30px;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #e879f9;
        }
        
        QComboBox QAbstractItemView {
            background-color: rgba(37, 30, 53, 0.98);
            border: 2px solid rgba(232, 121, 249, 0.5);
            border-radius: 8px;
            selection-background-color: rgba(232, 121, 249, 0.3);
            color: #f3e8ff;
            padding: 4px;
        }
        
        /* Custom object styles */
        QLabel#imageTitle {
            font-size: 14pt;
            font-weight: 600;
            color: #e879f9;
            padding: 8px;
        }
        
        QLabel#processedTitle {
            font-size: 14pt;
            font-weight: 600;
            color: #c026d3;
            padding: 8px;
        }
        
        QLabel#infoLabel {
            color: #c4b5fd;
            padding: 8px;
            font-size: 9pt;
            font-weight: 500;
        }
        
        QLabel#metricsLabel {
            background-color: rgba(45, 37, 71, 0.7);
            color: #98d8e8;
            padding: 12px;
            font-size: 10pt;
            font-weight: 600;
            border: 1px solid rgba(232, 121, 249, 0.3);
            border-radius: 8px;
            font-family: 'Consolas', monospace;
        }
        
        QWidget#imageContainer {
            background-color: rgba(45, 37, 71, 0.2);
            border: 2px solid rgba(232, 121, 249, 0.35);
            border-radius: 16px;
        }
        
        QWidget#imageContainer:hover {
            border: 2px solid rgba(232, 121, 249, 0.5);
        }
        
        QWidget#processedContainer {
            background-color: rgba(45, 37, 71, 0.2);
            border: 2px solid rgba(192, 38, 211, 0.4);
            border-radius: 16px;
        }
        
        QWidget#processedContainer:hover {
            border: 2px solid rgba(192, 38, 211, 0.6);
        }
        
        /* Scrollbar Styling */
        QScrollBar:vertical {
            background: rgba(45, 37, 71, 0.3);
            width: 12px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #e879f9, stop:1 #c026d3);
            border-radius: 6px;
            min-height: 20px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: #f0abfc;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        
        QScrollBar:horizontal {
            background: rgba(45, 37, 71, 0.3);
            height: 12px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                       stop:0 #e879f9, stop:1 #c026d3);
            border-radius: 6px;
            min-width: 20px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background: #f0abfc;
        }
        
        /* Tab Widget */
        QTabWidget::pane {
            border: 2px solid rgba(232, 121, 249, 0.3);
            border-radius: 10px;
            background-color: rgba(45, 37, 71, 0.2);
        }
        
        QTabBar::tab {
            background-color: rgba(45, 37, 71, 0.6);
            color: #c4b5fd;
            padding: 10px 20px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            margin-right: 4px;
        }
        
        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #e879f9, stop:1 #c026d3);
            color: #ffffff;
        }
        
        QTabBar::tab:hover:!selected {
            background-color: rgba(91, 75, 115, 0.6);
        }
    )";
}

#endif // THEME_H
