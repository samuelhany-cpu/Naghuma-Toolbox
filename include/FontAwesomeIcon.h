#ifndef FONTAWESOMEICON_H
#define FONTAWESOMEICON_H

#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QFontDatabase>
#include <QColor>
#include <QString>

/**
 * @brief Helper class to create QIcon objects from Font Awesome glyphs
 * 
 * This class properly renders Font Awesome icons as QPixmap objects,
 * which can then be used as QIcon in QPushButtons and other widgets.
 */
class FontAwesomeIcon {
public:
    /**
     * @brief Initialize Font Awesome font system
     * Loads the Font Awesome font from resources
     * @return true if font loaded successfully, false otherwise
     */
    static bool initialize();
    
    /**
     * @brief Create QIcon from Font Awesome character code
     * @param code Unicode character code (e.g., 0xf07c for folder-open)
     * @param color Icon color
     * @param size Icon size in pixels
     * @return QIcon object with rendered Font Awesome glyph
     */
    static QIcon create(int code, const QColor& color = Qt::white, int size = 24);
    
    /**
     * @brief Check if Font Awesome is initialized and ready
     * @return true if font is loaded and ready to use
     */
    static bool isReady();
    
    /**
     * @brief Get the font family name
     * @return Font family name string
     */
    static QString getFontFamily();

private:
    static bool initialized;
    static QString fontFamily;
    static int fontId;
    
    /**
     * @brief Render Font Awesome character to QPixmap
     * @param code Character code
     * @param font QFont to use for rendering
     * @param color Color to render
     * @param size Size in pixels
     * @return Rendered QPixmap
     */
    static QPixmap renderIcon(int code, const QFont& font, const QColor& color, int size);
};

#endif // FONTAWESOMEICON_H
