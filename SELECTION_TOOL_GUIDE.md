# Selection Tool - Complete Guide

## Overview
The Selection Tool allows you to apply filters and effects to specific areas of your image using 5 different selection methods. Each method is optimized for different use cases.

---

## How to Use

### Basic Workflow:
1. **Load an image** (File → Open or Ctrl+O)
2. **Enable Selection Mode** (Analysis → Selection Tool → Toggle Selection Mode or Ctrl+S, or click "Select" button in toolbar)
3. **Choose a selection method** from the Analysis → Selection Tool menu
4. **Create your selection** (method-specific - see below)
5. **Apply any filter/effect** - it will only affect the selected area!

---

## 5 Selection Methods

### 1. **Rectangle Selection** 📐
**Best for:** Simple rectangular areas, cropping regions, geometric selections

**How to use:**
- Analysis → Selection Tool → Rectangle Selection
- Click and drag to draw a rectangle
- Release mouse to complete selection
- Green outline shows selected area

**Use cases:**
- Selecting faces in portraits
- Isolating rectangular objects
- Quick region selection

---

### 2. **Polygon Selection** ✂️
**Best for:** Complex shapes, irregular objects, precise manual selection

**How to use:**
- Analysis → Selection Tool → Polygon Selection
- **Left-click** to add points to polygon
- Move mouse to see preview line
- **Right-click** or **double-click** to close polygon
- Green outline shows polygon boundary

**Use cases:**
- Tracing object outlines
- Selecting buildings, vehicles, people
- Free-form irregular selections

**Tips:**
- Click along object edges for precise selection
- Add more points for smoother curves
- Right-click to finalize selection

---

### 3. **Magic Wand** 🪄
**Best for:** Selecting areas by color similarity, backgrounds, skies, solid color regions

**How to use:**
- Analysis → Selection Tool → Magic Wand
- Click on any color in the image
- Enter tolerance value (0-255):
  - **Low (10-30)**: Select only very similar colors
  - **Medium (30-50)**: Balanced selection
  - **High (50-100)**: Select broader color range
- Selected pixels shown in green overlay

**Use cases:**
- Removing backgrounds
- Selecting sky/clouds
- Isolating solid-color objects
- Color-based masking

**Tips:**
- Use lower tolerance for distinct colors
- Click on representative color sample
- Combine with other methods for complex selections

---

### 4. **Threshold Selection** 📊
**Best for:** Brightness-based selection, isolating specific intensity ranges

**How to use:**
- Analysis → Selection Tool → Threshold Selection
- Enter **minimum intensity** (0-255)
- Enter **maximum intensity** (0-255)
- All pixels within range are selected
- Green overlay shows selected region

**Use cases:**
- Selecting shadows (0-100)
- Selecting highlights (180-255)
- Isolating midtones (100-150)
- Brightness-based masking

**Examples:**
- **Darken only shadows**: Min=0, Max=80, apply brightness decrease
- **Enhance highlights**: Min=200, Max=255, apply contrast
- **Select midtones**: Min=100, Max=180

---

### 5. **Edge-Based Selection** 🎯
**Best for:** Objects with clear edges, intelligent object detection

**How to use:**
- Analysis → Selection Tool → Edge-Based Selection
- Click **inside** the object you want to select
- Algorithm detects edges using Canny edge detection
- Flood-fills within detected boundaries
- Selected object shown in green

**Use cases:**
- Selecting distinct objects
- Objects with clear boundaries
- Automatic object isolation

**Tips:**
- Click in center of object
- Works best with high-contrast edges
- Adjust image contrast first if needed

---

## Applying Filters to Selections

Once you have an active selection, **all filters and effects apply ONLY to the selected area**:

### Example Workflow:
1. Load image
2. Enable Selection Mode (Ctrl+S)
3. Choose **Magic Wand**
4. Click on sky (tolerance 40)
5. Apply **Filters → Gaussian Blur** → Only sky blurs!

### Supported Operations:
- **All filters**: Gaussian Blur, Median Blur, Edge Detection, etc.
- **Adjustments**: Brightness, Contrast, Saturation
- **Effects**: Sharpen, Emboss, Sketch
- **Enhancements**: Auto Enhance, Denoise
- **Restoration**: Wiener Filter, Motion Blur Correction

---

## Menu Shortcuts

**Analysis → Selection Tool:**
- **Toggle Selection Mode** (Ctrl+S) - Enable/disable selection
- **Rectangle Selection** - Simple rectangle
- **Polygon Selection** - Multi-point lasso
- **Magic Wand** - Color-based
- **Threshold Selection** - Brightness range
- **Edge-Based Selection** - Intelligent object detection
- **Clear Selection** - Remove current selection

---

## Tips & Best Practices

### Combining Methods:
1. Start with **Magic Wand** to select background
2. Use **Polygon** to manually refine edges
3. Apply **Threshold** to fine-tune brightness areas

### Visual Feedback:
- **Green overlay** = selected pixels
- **No overlay** = nothing selected
- Status bar shows selection method and completion

### Selection Persistence:
- Selection stays active until:
  - You click "Clear Selection"
  - You switch selection methods
  - You disable selection mode

### Common Workflows:

#### Portrait Enhancement:
1. Magic Wand → Select background (tolerance 50)
2. Apply Gaussian Blur → Background softens
3. Clear selection
4. Polygon → Select face
5. Apply Sharpen → Face becomes crisp

#### Sky Replacement:
1. Magic Wand → Click sky (tolerance 40-60)
2. Apply Color Adjustment → Change sky color
3. Apply Brightness → Adjust sky tone

#### Object Isolation:
1. Edge-Based → Click inside object
2. Apply Sharpen → Object stands out
3. Polygon → Select background manually
4. Apply Blur → Background fades

---

## Troubleshooting

### Selection not visible?
- Check green overlay on image
- Status bar shows "Selection completed!"
- Make sure selection mode is enabled

### Magic Wand selects too much?
- Lower tolerance value (try 20-30)
- Click on more specific color sample

### Edge-Based doesn't work?
- Increase image contrast first
- Object may lack clear edges
- Try Polygon selection instead

### Filter doesn't apply?
- Ensure selection is active (green overlay visible)
- Check status bar for "Selection completed"
- Try clearing and recreating selection

---

## Advanced Techniques

### Feathered Edges (Future):
Currently selections have hard edges. For soft transitions:
1. Select area
2. Apply filter
3. Adjust filter strength/radius for smoother blend

### Multiple Selections:
- Clear previous selection first
- Each selection replaces the last
- Use Polygon for complex multi-region selections

### Precision Selection:
- Zoom in (Ctrl+Wheel) for pixel-perfect selections
- Use Polygon with many points for smooth curves
- Combine Edge-Based with manual refinement

---

## Technical Details

### Selection Storage:
- Selections stored as binary masks (0=unselected, 255=selected)
- Mask resolution matches image resolution
- Efficient memory usage with OpenCV cv::Mat

### Algorithm Details:
- **Rectangle**: Simple bounding box
- **Polygon**: cv::fillPoly for arbitrary shapes
- **Magic Wand**: cv::floodFill with tolerance
- **Threshold**: cv::inRange for intensity ranges
- **Edge-Based**: Canny edge detection + flood fill

### Filter Application:
- Original image copied
- Filter applied to full copy
- Result blended using mask: `result = (filtered & mask) | (original & ~mask)`

---

## Keyboard Shortcuts

- **Ctrl+S** - Toggle Selection Mode
- **Ctrl+O** - Open image
- **Ctrl+S** (when selecting) - Save with selection
- **Esc** - Clear current selection

---

## Selection Tool vs Brush Tool

**Previous Brush Tool (deprecated):**
- Manual painting with brush
- Hard to control
- Display issues

**New Selection Tool (current):**
- 5 intelligent methods
- Automatic selection algorithms
- Reliable visualization
- Professional workflow

---

## Future Enhancements (Roadmap)

- [ ] Feathered/soft edge selections
- [ ] Selection save/load
- [ ] Selection arithmetic (add, subtract, intersect)
- [ ] Quick selection mode (drag-to-select)
- [ ] Selection preview before applying
- [ ] Undo/redo for selections
- [ ] Multiple simultaneous selections

---

## Examples Gallery

### Example 1: Selective Blur
**Goal**: Blur background, keep subject sharp
1. Magic Wand → Click background
2. Gaussian Blur (radius 15)
3. Result: Professional depth-of-field effect

### Example 2: Sky Enhancement
**Goal**: Make sky more dramatic
1. Magic Wand → Click sky (tolerance 50)
2. Adjust Brightness → Increase +20
3. Adjust Saturation → Increase +30
4. Result: Vivid, dramatic sky

### Example 3: Object Isolation
**Goal**: Extract car from image
1. Edge-Based → Click on car
2. If imperfect, use Polygon to refine
3. Apply Sharpen → Car stands out
4. Clear selection
5. Magic Wand → Select background
6. Gaussian Blur → Background fades

### Example 4: Shadow/Highlight Fix
**Goal**: Lift shadows, preserve highlights
1. Threshold → Min=0, Max=80 (shadows only)
2. Brightness → Increase +40
3. Clear selection
4. Threshold → Min=200, Max=255 (highlights)
5. Brightness → Decrease -10
6. Result: Balanced exposure

---

## Credits

**Algorithm Implementation:**
- OpenCV 4.3.0 (cv::floodFill, cv::Canny, cv::fillPoly)
- Qt 6.7.3 (UI framework)

**Selection Methods:**
- Rectangle: Classic selection tool
- Polygon: Photoshop-style lasso tool
- Magic Wand: Tolerance-based flood fill (inspired by Adobe)
- Threshold: Intensity range masking
- Edge-Based: Canny edge detection + intelligent fill

---

## Support

For issues or feature requests:
1. Check this guide first
2. Review troubleshooting section
3. Experiment with different methods
4. Combine techniques for complex selections

**Happy Selecting!** 🎨
