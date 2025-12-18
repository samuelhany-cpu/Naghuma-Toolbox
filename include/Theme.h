#ifndef THEME_H
#define THEME_H

#include <QString>

namespace Theme {
    // Color constants - Wisteria & Soft Teal Palette
    constexpr const char* BG_PRIMARY = "#1E1E24";        // Deep Gunmetal
    constexpr const char* BG_SECONDARY = "#2B2D42";      // Slate Grey
    constexpr const char* FG_PRIMARY = "#EDF2F4";        // Bone White
    constexpr const char* FG_SECONDARY = "#B8BCC4";      // Dimmed white
    constexpr const char* ACCENT_PRIMARY = "#A594F9";    // Wisteria (soft purple)
    constexpr const char* ACCENT_SECONDARY = "#73D2DE";  // Soft Teal
    constexpr const char* SUCCESS_COLOR = "#73D2DE";     // Soft Teal
    constexpr const char* INFO_COLOR = "#A594F9";        // Wisteria
    constexpr const char* WARNING_COLOR = "#F4A261";     // Warm orange
    constexpr const char* ERROR_COLOR = "#E76F51";       // Warm red
    constexpr const char* HOVER_GLOW = "rgba(165, 148, 249, 0.3)";

    // Animation durations (in ms)
    constexpr int ANIMATION_FAST = 150;
    constexpr int ANIMATION_NORMAL = 250;
    constexpr int ANIMATION_SLOW = 350;

    // Main application stylesheet (Qt-compatible only, no CSS3)
    const char* const MAIN_STYLESHEET = R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #1E1E24, stop:0.3 #252530, 
                                       stop:0.7 #1E1E24, stop:1 #1E1E24);
        }
        
        QWidget {
            background-color: #2B2D42;
            color: #EDF2F4;
            font-family: 'Segoe UI', -apple-system, sans-serif;
            font-size: 10pt;
        }
        
        /* Enhanced Button Styles */
        QPushButton {
            background-color: #3A3D52;
            color: #EDF2F4;
            border: 1px solid #4A4D62;
            border-radius: 10px;
            padding: 12px 24px;
            font-weight: 500;
        }
        
        QPushButton:hover {
            background-color: #4A4D62;
            color: #A594F9;
            border: 1px solid rgba(165, 148, 249, 0.5);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            color: #ffffff;
            border: none;
        }
        
        QPushButton.accent {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            color: #ffffff;
            font-weight: 600;
            border: none;
            padding: 12px 28px;
        }
        
        QPushButton.accent:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #B8A8FF, stop:1 #8DE0EA);
        }
        
        QPushButton:disabled {
            background-color: rgba(43, 45, 66, 0.4);
            color: rgba(237, 242, 244, 0.4);
            border: 1px solid rgba(74, 77, 98, 0.3);
        }
        
        /* Form Elements */
        QLineEdit, QSpinBox, QDoubleSpinBox {
            background-color: #3A3D52;
            color: #EDF2F4;
            border: 2px solid #4A4D62;
            border-radius: 8px;
            padding: 10px 14px;
        }
        
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #A594F9;
            background-color: rgba(165, 148, 249, 0.08);
        }
        
        /* Enhanced Slider */
        QSlider::groove:horizontal {
            background: #3A3D52;
            height: 10px;
            border-radius: 5px;
        }
        
        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            width: 22px;
            height: 22px;
            margin: -6px 0;
            border-radius: 11px;
            border: 2px solid rgba(255, 255, 255, 0.3);
        }
        
        QSlider::handle:horizontal:hover {
            background: #B8A8FF;
        }
        
        QSlider::sub-page:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            border-radius: 5px;
        }
        
        /* Progress Bar */
        QProgressBar {
            background-color: #3A3D52;
            border: none;
            border-radius: 8px;
            height: 14px;
            text-align: center;
            color: #ffffff;
            font-weight: 500;
        }
        
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            border-radius: 8px;
        }
        
        QLabel {
            background-color: transparent;
            color: #EDF2F4;
        }
        
        /* Enhanced GroupBox */
        QGroupBox {
            border: 2px solid rgba(165, 148, 249, 0.35);
            border-radius: 14px;
            margin-top: 22px;
            padding: 24px 20px 20px 20px;
            font-weight: 600;
            color: #A594F9;
            background-color: rgba(43, 45, 66, 0.25);
        }
        
        QGroupBox:hover {
            border: 2px solid rgba(165, 148, 249, 0.5);
            background-color: rgba(43, 45, 66, 0.35);
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 16px;
            background-color: #1E1E24;
            font-size: 11pt;
        }
        
        /* Status Bar */
        QStatusBar {
            background: rgba(30, 30, 36, 0.95);
            color: #EDF2F4;
            border-top: 2px solid rgba(165, 148, 249, 0.25);
            padding: 6px;
        }
        
        /* Menu Bar */
        QMenuBar {
            background: rgba(43, 45, 66, 0.95);
            color: #EDF2F4;
            spacing: 6px;
            padding: 10px 8px;
            border-bottom: 2px solid rgba(165, 148, 249, 0.25);
        }
        
        QMenuBar::item {
            padding: 10px 20px;
            background: transparent;
            border-radius: 8px;
        }
        
        QMenuBar::item:selected {
            background-color: rgba(165, 148, 249, 0.15);
            color: #A594F9;
        }
        
        QMenuBar::item:pressed {
            background-color: rgba(165, 148, 249, 0.25);
        }
        
        /* Menu */
        QMenu {
            background-color: rgba(43, 45, 66, 0.98);
            border: 2px solid rgba(74, 77, 98, 0.5);
            border-radius: 12px;
            padding: 10px;
        }
        
        QMenu::item {
            padding: 12px 40px;
            color: #EDF2F4;
            border-radius: 8px;
        }
        
        QMenu::item:selected {
            background-color: rgba(165, 148, 249, 0.2);
            color: #A594F9;
        }
        
        QMenu::separator {
            height: 1px;
            background: rgba(165, 148, 249, 0.2);
            margin: 6px 14px;
        }
        
        /* Toolbar */
        QToolBar {
            background: rgba(43, 45, 66, 0.95);
            border-bottom: 2px solid rgba(165, 148, 249, 0.25);
            spacing: 12px;
            padding: 14px;
        }
        
        QToolBar::separator {
            background: rgba(165, 148, 249, 0.3);
            width: 2px;
            margin: 0 8px;
        }
        
        /* Text Edit */
        QTextEdit {
            background-color: #3A3D52;
            color: #EDF2F4;
            border: 2px solid #4A4D62;
            border-radius: 10px;
            padding: 14px;
            selection-background-color: rgba(165, 148, 249, 0.4);
        }
        
        QTextEdit:focus {
            border: 2px solid rgba(165, 148, 249, 0.6);
        }
        
        /* Dialog Styling */
        QMessageBox {
            background-color: #1E1E24;
        }
        
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #1E1E24, stop:1 #2B2D42);
        }
        
        /* Radio Buttons */
        QRadioButton {
            spacing: 8px;
            color: #EDF2F4;
        }
        
        QRadioButton::indicator {
            width: 20px;
            height: 20px;
            border-radius: 10px;
            border: 2px solid rgba(184, 188, 196, 0.5);
            background-color: #3A3D52;
        }
        
        QRadioButton::indicator:checked {
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5,
                                       fx:0.5, fy:0.5,
                                       stop:0 #A594F9, stop:0.6 #73D2DE, stop:1 transparent);
            border: 2px solid #A594F9;
        }
        
        QRadioButton::indicator:hover {
            border: 2px solid #A594F9;
        }
        
        /* Combo Box */
        QComboBox {
            background-color: #3A3D52;
            color: #EDF2F4;
            border: 2px solid #4A4D62;
            border-radius: 8px;
            padding: 8px 14px;
            min-width: 120px;
        }
        
        QComboBox:hover {
            border: 2px solid rgba(165, 148, 249, 0.5);
        }
        
        QComboBox::drop-down {
            border: none;
            width: 30px;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #A594F9;
        }
        
        QComboBox QAbstractItemView {
            background-color: rgba(43, 45, 66, 0.98);
            border: 2px solid rgba(165, 148, 249, 0.5);
            border-radius: 8px;
            selection-background-color: rgba(165, 148, 249, 0.3);
            color: #EDF2F4;
            padding: 4px;
        }
        
        /* Custom object styles */
        QLabel#imageTitle {
            font-size: 14pt;
            font-weight: 600;
            color: #A594F9;
            padding: 8px;
        }
        
        QLabel#processedTitle {
            font-size: 14pt;
            font-weight: 600;
            color: #73D2DE;
            padding: 8px;
        }
        
        QLabel#infoLabel {
            color: #B8BCC4;
            padding: 8px;
            font-size: 9pt;
            font-weight: 500;
        }
        
        QLabel#metricsLabel {
            background-color: rgba(43, 45, 66, 0.7);
            color: #73D2DE;
            padding: 12px;
            font-size: 10pt;
            font-weight: 600;
            border: 1px solid rgba(165, 148, 249, 0.3);
            border-radius: 8px;
            font-family: 'Consolas', monospace;
        }
        
        QWidget#imageContainer {
            background-color: rgba(43, 45, 66, 0.2);
            border: 2px solid rgba(165, 148, 249, 0.35);
            border-radius: 16px;
        }
        
        QWidget#imageContainer:hover {
            border: 2px solid rgba(165, 148, 249, 0.5);
        }
        
        QWidget#processedContainer {
            background-color: rgba(43, 45, 66, 0.2);
            border: 2px solid rgba(115, 210, 222, 0.4);
            border-radius: 16px;
        }
        
        QWidget#processedContainer:hover {
            border: 2px solid rgba(115, 210, 222, 0.6);
        }
        
        /* Scrollbar Styling */
        QScrollBar:vertical {
            background: rgba(43, 45, 66, 0.3);
            width: 12px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            border-radius: 6px;
            min-height: 20px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: #B8A8FF;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        
        QScrollBar:horizontal {
            background: rgba(43, 45, 66, 0.3);
            height: 12px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            border-radius: 6px;
            min-width: 20px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background: #B8A8FF;
        }
        
        /* Tab Widget */
        QTabWidget::pane {
            border: 2px solid rgba(165, 148, 249, 0.3);
            border-radius: 10px;
            background-color: rgba(43, 45, 66, 0.2);
        }
        
        QTabBar::tab {
            background-color: #3A3D52;
            color: #B8BCC4;
            padding: 10px 20px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            margin-right: 4px;
        }
        
        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #A594F9, stop:1 #73D2DE);
            color: #ffffff;
        }
        
        QTabBar::tab:hover:!selected {
            background-color: #4A4D62;
        }
    )";
}

#endif // THEME_H
