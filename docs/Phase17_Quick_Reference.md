# Phase 17: Advanced Segmentation - Quick Reference

## ?? 5 Segmentation Methods

| Method | Purpose | Key Parameter | Speed | Best For |
|--------|---------|---------------|-------|----------|
| **Watershed** | Topology-based separation | Threshold (1-100%) | Medium | Touching objects, cell counting |
| **K-Means** | Color clustering | K clusters (2-20) | Fast | Color quantization, object detection prep |
| **Mean Shift** | Mode-finding | Spatial/Color radius | Slow | Natural scenes, texture segmentation |
| **GrabCut** | Foreground extraction | Iterations (1-10) | Slow | Background removal, object cutouts |
| **SLIC** | Superpixel regions | Num regions (10-1000) | Medium | CV preprocessing, hierarchical segmentation |

---

## ?? Default Parameters (Good Starting Point)

```
Watershed:
  Distance Threshold: 20%

K-Means:
  Clusters (K): 3
  Iterations: 100

Mean Shift:
  Spatial Radius: 20
  Color Radius: 40

GrabCut:
  Iterations: 5

SLIC:
  Superpixels: 200
  Compactness: 10
```

---

## ?? When to Use Which Method?

### **Watershed** ? Use when:
- Separating overlapping objects
- Counting objects (coins, cells, particles)
- Clear intensity gradients exist

### **K-Means** ? Use when:
- Need specific number of colors/regions
- Color-based segmentation sufficient
- Speed is important

### **Mean Shift** ? Use when:
- Natural images with gradual transitions
- Want to preserve edges
- Don't know number of segments beforehand

### **GrabCut** ? Use when:
- Need to extract main subject
- Remove background
- Create image cutouts/masks

### **SLIC** ? Use when:
- Need regular, compact regions
- Preprocessing for object recognition
- Want to reduce image complexity

---

## ?? Quick Workflow

```
1. Segmentation > Region-Based Segmentation...
2. Choose method from dropdown
3. Adjust parameters
4. Watch preview update
5. Click "Apply" when satisfied
6. Layer added to sidebar
7. Ctrl+Z to undo if needed
```

---

## ?? Pro Tips

**Watershed:**
- Pre-blur noisy images (Process > Gaussian Blur)
- Lower threshold = more segments
- Works best on binary or grayscale

**K-Means:**
- Try K=3 first, increase if needed
- Odd numbers (3,5,7) often work better
- Max iterations: 100 usually sufficient

**Mean Shift:**
- Spatial 20, Color 40 = good default
- Increase both for smoother, fewer regions
- Decrease both for finer segmentation

**GrabCut:**
- Works best with clear subject
- Increase iterations if edges not clean
- Outputs black background (can invert if needed)

**SLIC:**
- 200 superpixels = good balance
- Compactness 10 = natural boundaries
- Compactness 50+ = square-like regions

---

## ?? Common Issues

| Problem | Solution |
|---------|----------|
| Too many small regions | Increase threshold/radius or decrease K |
| Regions merged incorrectly | Decrease threshold/radius or increase K |
| Slow preview | Reduce image size or parameter values |
| Poor quality | Try different method or adjust parameters |
| SLIC unavailable | Install OpenCV with contrib modules |

---

## ?? Example Parameter Sets

### Natural Photographs
```
K-Means: K=5
Mean Shift: Spatial=20, Color=40
SLIC: 300 regions, compactness=10
```

### Medical Images
```
Watershed: Threshold=30%
K-Means: K=3
Mean Shift: Spatial=15, Color=30
```

### Object Detection
```
SLIC: 200 regions, compactness=15
K-Means: K=4
```

### Artistic Effects
```
K-Means: K=8-15 (posterization)
Mean Shift: Spatial=30, Color=60 (cartoon effect)
```

---

## ?? Performance Guide

**Small Images (< 500x500):**
- All methods: < 1 second
- Use any parameters

**Medium Images (500-1500):**
- Most methods: 1-3 seconds
- Recommended for testing

**Large Images (> 1500):**
- K-Means: Still fast (1-2 sec)
- Mean Shift/GrabCut: May be slow (5-10 sec)
- Consider downscaling first

---

## ?? Integration with Other Features

**Before Segmentation:**
- Denoise (Process > Noise Removal)
- Enhance (Process > Auto Enhance)
- Blur (Process > Gaussian Blur) for Watershed

**After Segmentation:**
- Morphology operations (clean boundaries)
- Thresholding (extract specific regions)
- Save as layer for comparison

**Workflow Example:**
```
1. Load image
2. Process > Gaussian Blur (if noisy)
3. Segmentation > Region-Based...
4. Apply K-Means (K=5)
5. Morphology > Closing (smooth boundaries)
6. Save result
```

---

## ?? Goals by Application

### Photo Editing
- Use: K-Means (color adjustment), GrabCut (background)
- Goal: Clean subject extraction

### Scientific Analysis
- Use: Watershed (counting), SLIC (preprocessing)
- Goal: Accurate object detection

### Artistic
- Use: K-Means (posterization), Mean Shift (cartoon)
- Goal: Creative effects

### Computer Vision
- Use: SLIC (superpixels), Mean Shift (preprocessing)
- Goal: Reduce complexity

---

**Remember:** All parameters have real-time preview - experiment freely! ??
