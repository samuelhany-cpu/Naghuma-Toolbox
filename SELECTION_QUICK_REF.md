# Selection Tool - Quick Reference Card

## 🚀 Quick Start
1. Load image (Ctrl+O)
2. Press **Ctrl+S** to enable Selection Mode
3. Choose method from **Analysis → Selection Tool**
4. Make selection
5. Apply filter - affects selection only!

---

## 📐 5 Selection Methods

| Method | Best For | How to Use | Icon |
|--------|----------|------------|------|
| **Rectangle** | Simple areas, geometric shapes | Click & drag | 📐 |
| **Polygon** | Complex shapes, object outlines | Click points, right-click to close | ✂️ |
| **Magic Wand** | Colors, backgrounds, skies | Click color, set tolerance (0-255) | 🪄 |
| **Threshold** | Brightness ranges, shadows/highlights | Set min/max intensity (0-255) | 📊 |
| **Edge-Based** | Objects with clear edges | Click inside object | 🎯 |

---

## ⌨️ Keyboard Shortcuts
- **Ctrl+S** - Toggle Selection Mode
- **Right-click** - Close polygon (in Polygon mode)
- **Esc** - Clear selection

---

## 📍 Menu Location
**Analysis → Selection Tool**
- Toggle Selection Mode (Ctrl+S)
- Rectangle Selection
- Polygon Selection
- Magic Wand
- Threshold Selection
- Edge-Based Selection
- Clear Selection

---

## 💡 Quick Tips

### Magic Wand Tolerance:
- **10-20**: Very similar colors only
- **30-50**: Balanced (recommended)
- **60-100**: Broad color range

### Threshold Ranges:
- **Shadows**: 0-80
- **Midtones**: 80-180
- **Highlights**: 180-255

### Best Practices:
- ✅ Start with automatic methods (Magic Wand, Edge-Based)
- ✅ Use Polygon for manual refinement
- ✅ Clear selection between different areas
- ✅ Check green overlay to confirm selection

---

## 🎨 Example Workflows

### Blur Background:
1. Ctrl+S → Magic Wand → Click background
2. Filters → Gaussian Blur
3. Done! Background blurred, subject sharp

### Enhance Object:
1. Ctrl+S → Edge-Based → Click object
2. Filters → Sharpen
3. Done! Object enhanced

### Fix Shadows:
1. Ctrl+S → Threshold → Min=0, Max=80
2. Adjust → Brightness +40
3. Done! Shadows lifted

---

## ✅ Visual Feedback
- **Green overlay** = Selected area
- **No overlay** = No selection active
- **Status bar** = Shows method and status

---

## 🔧 Troubleshooting

| Problem | Solution |
|---------|----------|
| Can't see selection | Check for green overlay, enable Selection Mode |
| Magic Wand selects too much | Lower tolerance (try 20-30) |
| Edge-Based doesn't work | Increase contrast first, or use Polygon |
| Filter affects whole image | Ensure selection visible before applying |

---

## 📖 Full Documentation
See **SELECTION_TOOL_GUIDE.md** for complete instructions

---

**Selection Tool Status: ✅ ACTIVE**
