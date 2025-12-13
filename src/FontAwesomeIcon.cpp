#include "FontAwesomeIcon.h"
#include <QDebug>
#include <QApplication>

// Static member initialization
bool FontAwesomeIcon::initialized = false;
QString FontAwesomeIcon::fontFamily = "";
int FontAwesomeIcon::fontId = -1;

bool FontAwesomeIcon::initialize() {
    if (initialized) {
        return true;
    }
    
    // Load Font Awesome from resources
    fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/fa-solid-900.ttf");
    
    if (fontId == -1) {
        qWarning() << "Failed to load Font Awesome font from resources!";
        return false;
    }
    
    // Get font family name
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    
    if (fontFamilies.isEmpty()) {
        qWarning() << "Font Awesome loaded but no font families found!";
        return false;
    }
    
    fontFamily = fontFamilies.at(0);
    initialized = true;
    
    qDebug() << "Font Awesome initialized successfully!";
    qDebug() << "Font family:" << fontFamily;
    qDebug() << "Font ID:" << fontId;
    
    return true;
}

bool FontAwesomeIcon::isReady() {
    return initialized && fontId != -1 && !fontFamily.isEmpty();
}

QString FontAwesomeIcon::getFontFamily() {
    return fontFamily;
}

QPixmap FontAwesomeIcon::renderIcon(int code, const QFont& font, const QColor& color, int size) {
    // Create a pixmap with transparency
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    
    // Setup painter
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    // Set font and color
    painter.setFont(font);
    painter.setPen(color);
    
    // Draw the character centered
    QString iconChar = QString(QChar(code));
    QFontMetrics fm(font);
    
    // Calculate center position
    int width = fm.horizontalAdvance(iconChar);
    int height = fm.height();
    int x = (size - width) / 2;
    int y = (size + height / 2) / 2;
    
    painter.drawText(x, y, iconChar);
    painter.end();
    
    return pixmap;
}

QIcon FontAwesomeIcon::create(int code, const QColor& color, int size) {
    if (!initialized) {
        initialize();
    }
    
    if (!isReady()) {
        qWarning() << "Font Awesome not ready! Cannot create icon for code:" << QString::number(code, 16);
        return QIcon();
    }
    
    // Create font with proper size
    QFont font(fontFamily);
    font.setPixelSize(size * 0.8); // Make icon slightly smaller than box
    font.setStyleStrategy(QFont::PreferAntialias);
    
    // Render the icon
    QPixmap normalPixmap = renderIcon(code, font, color, size);
    
    // Create a brighter version for active/hover state
    QColor activeColor = color.lighter(120);
    QPixmap activePixmap = renderIcon(code, font, activeColor, size);
    
    // Create QIcon with different states
    QIcon icon;
    icon.addPixmap(normalPixmap, QIcon::Normal, QIcon::Off);
    icon.addPixmap(activePixmap, QIcon::Active, QIcon::Off);
    icon.addPixmap(activePixmap, QIcon::Selected, QIcon::Off);
    
    return icon;
}
