#include "CollapsibleToolbar.h"
#include "Theme.h"
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <QFontDatabase>
#include <QDebug>

CollapsibleToolbar::CollapsibleToolbar(QWidget *parent)
    : QWidget(parent),
      expanded(false),
      collapsedWidth(60),
      expandedWidth(200) {
    
    loadFontAwesome();
    setupUI();
    setExpanded(false);
}

CollapsibleToolbar::~CollapsibleToolbar() {
}

void CollapsibleToolbar::loadFontAwesome() {
    // Load Font Awesome from resources
    int fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/fa-solid-900.ttf");
    
    qDebug() << "Font Awesome loading - Font ID:" << fontId;
    
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        qDebug() << "Available font families:" << fontFamilies;
        
        if (!fontFamilies.isEmpty()) {
            QString fontFamily = fontFamilies.at(0);
            qDebug() << "Using font family:" << fontFamily;
            
            iconFont = QFont(fontFamily);
            iconFont.setPixelSize(20);
            iconFont.setWeight(QFont::Normal);
            iconFont.setStyleStrategy(QFont::PreferAntialias);
            
            qDebug() << "Icon font configured:" << iconFont.family() << iconFont.pixelSize();
        } else {
            qWarning() << "Font Awesome loaded but no families found!";
        }
    } else {
        qWarning() << "Failed to load Font Awesome font from resources!";
        // List all application fonts for debugging
        QStringList allFonts = QFontDatabase::families();
        qDebug() << "Total system fonts:" << allFonts.count();
    }
}

void CollapsibleToolbar::setupUI() {
    setMinimumWidth(collapsedWidth);
    setMaximumWidth(collapsedWidth);
    
    setStyleSheet(R"(
        CollapsibleToolbar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                       stop:0 rgba(31, 21, 53, 0.95),
                                       stop:1 rgba(26, 10, 31, 0.95));
            border-right: 2px solid rgba(232, 121, 249, 0.25);
        }
    )");
    
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Toggle button at top with simple icon
    toggleButton = new QPushButton(this);
    
    // Use simple mathematical symbol that works everywhere
    QFont btnFont("Arial", 16, QFont::Bold);
    toggleButton->setFont(btnFont);
    toggleButton->setText("?");  // Triple bar / hamburger menu
    toggleButton->setToolTip("Expand/Collapse Toolbar");
    toggleButton->setFixedSize(50, 50);
    toggleButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(232, 121, 249, 0.15);
            color: #e879f9;
            border: none;
            border-radius: 25px;
        }
        QPushButton:hover {
            background-color: rgba(232, 121, 249, 0.3);
        }
        QPushButton:pressed {
            background-color: rgba(232, 121, 249, 0.5);
        }
    )");
    
    connect(toggleButton, &QPushButton::clicked, this, &CollapsibleToolbar::toggleExpanded);
    mainLayout->addWidget(toggleButton, 0, Qt::AlignHCenter);
    
    mainLayout->addSpacing(10);
    
    // Tools container
    toolsContainer = new QWidget(this);
    toolsLayout = new QVBoxLayout(toolsContainer);
    toolsLayout->setContentsMargins(5, 5, 5, 5);
    toolsLayout->setSpacing(8);
    
    mainLayout->addWidget(toolsContainer);
    mainLayout->addStretch();
    
    // Width animation
    widthAnimation = new QPropertyAnimation(this, "maximumWidth");
    widthAnimation->setDuration(Theme::ANIMATION_NORMAL);
    widthAnimation->setEasingCurve(QEasingCurve::InOutCubic);
}

QPushButton* CollapsibleToolbar::addTool(const QString& text, const QString& tooltip,
                                         std::function<void()> callback, const QString& iconCode) {
    QPushButton *button = new QPushButton(this);
    
    // Use simple text icons (single letters)
    if (!iconCode.isEmpty()) {
        QFont btnFont("Arial", 14, QFont::Bold);
        button->setFont(btnFont);
        button->setText(iconCode);
        button->setToolTip(tooltip);
        
        qDebug() << "Button" << text << "- Icon:" << iconCode;
    } else {
        button->setText(text.left(1));  // Fallback to first letter
        button->setToolTip(tooltip);
    }
    
    button->setFixedSize(50, 50);
    button->setProperty("fullText", text);
    button->setProperty("iconCode", iconCode);
    
    // Updated stylesheet
    button->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(45, 37, 71, 0.6);
            color: #e879f9;
            border: 1px solid rgba(91, 75, 115, 0.5);
            border-radius: 10px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 rgba(232, 121, 249, 0.3),
                                       stop:1 rgba(192, 38, 211, 0.3));
            color: #ffffff;
            border: 1px solid rgba(232, 121, 249, 0.6);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #e879f9, stop:1 #c026d3);
            color: #ffffff;
        }
    )");
    
    connect(button, &QPushButton::clicked, [callback]() {
        if (callback) callback();
    });
    
    toolsLayout->addWidget(button, 0, Qt::AlignHCenter);
    toolButtons.append(button);
    
    return button;
}

void CollapsibleToolbar::addSeparator() {
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("QFrame { background: rgba(232, 121, 249, 0.3); max-height: 2px; margin: 4px 10px; }");
    toolsLayout->addWidget(separator);
}

void CollapsibleToolbar::toggleExpanded() {
    setExpanded(!expanded);
}

void CollapsibleToolbar::setExpanded(bool expand) {
    if (expanded == expand) return;
    
    expanded = expand;
    
    if (expanded) {
        // Expand animation
        animateWidth(expandedWidth);
        toggleButton->setText("×");  // Multiplication sign for close
        
        // Show full text on buttons with animation
        QTimer::singleShot(Theme::ANIMATION_NORMAL / 2, this, [this]() {
            updateButtonStyles();
        });
    } else {
        // Collapse animation
        animateWidth(collapsedWidth);
        toggleButton->setText("?");  // Triple bar for menu
        
        // Show icons only
        updateButtonStyles();
    }
    
    emit expandedChanged(expanded);
}

void CollapsibleToolbar::animateWidth(int targetWidth) {
    widthAnimation->stop();
    widthAnimation->setStartValue(maximumWidth());
    widthAnimation->setEndValue(targetWidth);
    widthAnimation->start();
    
    // Also animate minimum width
    setMinimumWidth(targetWidth);
}

void CollapsibleToolbar::updateButtonStyles() {
    for (QPushButton *button : toolButtons) {
        QString iconCode = button->property("iconCode").toString();
        
        if (expanded) {
            // Show full text with icon
            QString fullText = button->property("fullText").toString();
            
            if (!iconCode.isEmpty()) {
                button->setText(iconCode + "  " + fullText);
                
                // Use regular font for text, keep icon font
                QFont btnFont = iconFont;
                btnFont.setPixelSize(12);
                button->setFont(btnFont);
            } else {
                button->setText(fullText);
            }
            
            button->setFixedSize(expandedWidth - 20, 45);
            
            button->setStyleSheet(R"(
                QPushButton {
                    background-color: rgba(45, 37, 71, 0.6);
                    color: #e879f9;
                    border: 1px solid rgba(91, 75, 115, 0.5);
                    border-radius: 10px;
                    text-align: left;
                    padding-left: 15px;
                }
                QPushButton:hover {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                               stop:0 rgba(232, 121, 249, 0.3),
                                               stop:1 rgba(192, 38, 211, 0.3));
                    color: #ffffff;
                    border: 1px solid rgba(232, 121, 249, 0.6);
                }
                QPushButton:pressed {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                               stop:0 #e879f9, stop:1 #c026d3);
                    color: #ffffff;
                }
            )");
        } else {
            // Show icon only
            if (!iconCode.isEmpty()) {
                QFont btnFont = iconFont;
                btnFont.setPixelSize(18);
                button->setFont(btnFont);
                button->setText(iconCode);
            }
            
            button->setFixedSize(50, 50);
            
            button->setStyleSheet(R"(
                QPushButton {
                    background-color: rgba(45, 37, 71, 0.6);
                    color: #e879f9;
                    border: 1px solid rgba(91, 75, 115, 0.5);
                    border-radius: 10px;
                }
                QPushButton:hover {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                               stop:0 rgba(232, 121, 249, 0.3),
                                               stop:1 rgba(192, 38, 211, 0.3));
                    color: #ffffff;
                    border: 1px solid rgba(232, 121, 249, 0.6);
                }
                QPushButton:pressed {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                               stop:0 #e879f9, stop:1 #c026d3);
                    color: #ffffff;
                }
            )");
        }
    }
}
