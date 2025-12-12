#ifndef COLLAPSIBLETOOLBAR_H
#define COLLAPSIBLETOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QIcon>
#include <functional>

class CollapsibleToolbar : public QWidget {
    Q_OBJECT

public:
    explicit CollapsibleToolbar(QWidget *parent = nullptr);
    ~CollapsibleToolbar();

    // Add tool buttons with icon support
    QPushButton* addTool(const QString& text, const QString& tooltip, 
                        std::function<void()> callback, const QString& iconPath = "");
    void addSeparator();
    
    // Toolbar state
    bool isExpanded() const { return expanded; }
    void setExpanded(bool expand);
    
    // Styling
    void setToolbarWidth(int width) { collapsedWidth = 60; expandedWidth = width; }

signals:
    void expandedChanged(bool expanded);

private slots:
    void toggleExpanded();

private:
    void setupUI();
    void animateWidth(int targetWidth);
    void updateButtonStyles();

    // UI Components
    QPushButton *toggleButton;
    QVBoxLayout *mainLayout;
    QWidget *toolsContainer;
    QVBoxLayout *toolsLayout;
    
    QList<QPushButton*> toolButtons;
    QPropertyAnimation *widthAnimation;

    // State
    bool expanded;
    int collapsedWidth;
    int expandedWidth;
};

#endif // COLLAPSIBLETOOLBAR_H
