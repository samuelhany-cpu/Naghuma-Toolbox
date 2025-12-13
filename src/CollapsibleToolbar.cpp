#include "CollapsibleToolbar.h"
#include "Theme.h"
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <QDebug>

CollapsibleToolbar::CollapsibleToolbar(QWidget *parent)
    : QWidget(parent),
      expanded(false),
      collapsedWidth(60),
      expandedWidth(200) {
    
    // Initialize Font Awesome
    if (!FontAwesomeIcon::initialize()) {
        qWarning() << "Failed to initialize Font Awesome!";
    }
    
    setupUI();
    setExpanded(false);
}

CollapsibleToolbar::~CollapsibleToolbar() {
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
    
    // Toggle button at top with Font Awesome icon
    toggleButton = new QPushButton(this);
    
    // Create menu/bars icon (0xf0c9)
    QIcon menuIcon = FontAwesomeIcon::create(0xf0c9, QColor(232, 121, 249), 24);
    toggleButton->setIcon(menuIcon);
    toggleButton->setIconSize(QSize(24, 24));
    toggleButton->setToolTip("Expand/Collapse Toolbar");
    toggleButton->setFixedSize(50, 50);
    toggleButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(232, 121, 249, 0.15);
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
                                         std::function<void()> callback, int iconCode) {
    QPushButton *button = new QPushButton(this);
    
    // Set Font Awesome icon using helper class
    if (iconCode > 0) {
        QIcon icon = FontAwesomeIcon::create(iconCode, QColor(232, 121, 249), 24);
        button->setIcon(icon);
        button->setIconSize(QSize(24, 24));
        button->setToolTip(tooltip);
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
            border: 1px solid rgba(91, 75, 115, 0.5);
            border-radius: 10px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 rgba(232, 121, 249, 0.3),
                                       stop:1 rgba(192, 38, 211, 0.3));
            border: 1px solid rgba(232, 121, 249, 0.6);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                       stop:0 #e879f9, stop:1 #c026d3);
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
        
        // Change to close icon (0xf00d)
        QIcon closeIcon = FontAwesomeIcon::create(0xf00d, QColor(232, 121, 249), 24);
        toggleButton->setIcon(closeIcon);
        
        // Show full text on buttons with animation
        QTimer::singleShot(Theme::ANIMATION_NORMAL / 2, this, [this]() {
            updateButtonStyles();
        });
    } else {
        // Collapse animation
        animateWidth(collapsedWidth);
        
        // Change to menu icon (0xf0c9)
        QIcon menuIcon = FontAwesomeIcon::create(0xf0c9, QColor(232, 121, 249), 24);
        toggleButton->setIcon(menuIcon);
        
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
        int iconCode = button->property("iconCode").toInt();
        
        if (expanded) {
            // Show full text with icon
            QString fullText = button->property("fullText").toString();
            button->setText(fullText);
            
            button->setFixedSize(expandedWidth - 20, 45);
            
            button->setStyleSheet(R"(
                QPushButton {
                    background-color: rgba(45, 37, 71, 0.6);
                    border: 1px solid rgba(91, 75, 115, 0.5);
                    border-radius: 10px;
                    text-align: left;
                    padding-left: 40px;
                }
                QPushButton:hover {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                               stop:0 rgba(232, 121, 249, 0.3),
                                               stop:1 rgba(192, 38, 211, 0.3));
                    border: 1px solid rgba(232, 121, 249, 0.6);
                }
                QPushButton:pressed {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                               stop:0 #e879f9, stop:1 #c026d3);
                }
            )");
        } else {
            // Show icon only
            button->setText("");
            button->setFixedSize(50, 50);
            
            button->setStyleSheet(R"(
                QPushButton {
                    background-color: rgba(45, 37, 71, 0.6);
                    border: 1px solid rgba(91, 75, 115, 0.5);
                    border-radius: 10px;
                }
                QPushButton:hover {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                               stop:0 rgba(232, 121, 249, 0.3),
                                               stop:1 rgba(192, 38, 211, 0.3));
                    border: 1px solid rgba(232, 121, 249, 0.6);
                }
                QPushButton:pressed {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                               stop:0 #e879f9, stop:1 #c026d3);
                }
            )");
        }
    }
}
