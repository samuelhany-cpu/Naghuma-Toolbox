# Quick Start: Phase 13 Edge Detectors ??

## What You Got

6 new edge detection algorithms in **Filters ? Edge Detectors** menu:

| Icon | Name | What It Does | Best For |
|------|------|--------------|----------|
| ?? | Prewitt Edge | All edges | General use |
| ? | Prewitt X | Vertical edges | Columns, vertical lines |
| ? | Prewitt Y | Horizontal edges | Rows, horizontal lines |
| ? | Roberts Cross | Diagonal edges | Fast detection, thin edges |
| ?? | LoG | Smooth edges | Noisy images, blobs |
| ?? | DoG | Feature edges | SIFT, multi-scale |

## How to Use

### Method 1: Menu
1. Load image (`File ? Load Image`)
2. Go to `Filters ? Edge Detectors`
3. Click any algorithm
4. View result on right side
5. Save if you like it (`File ? Save Image`)

### Method 2: Keyboard
1. Load image
2. Press `Alt+F` (opens Filters menu)
3. Press `E` (Edge Detectors submenu)
4. Press 1-6 to select algorithm

### Method 3: Layer System
1. Apply multiple edge detectors
2. Compare in Layers panel (right sidebar)
3. Toggle visibility to compare
4. Remove unwanted ones

## Quick Comparison

**Test Image**: Load any photo

### Try This Sequence:
1. **Prewitt Edge** - See all edges equally
2. **Prewitt X** - See only vertical elements
3. **Prewitt Y** - See only horizontal elements
4. **Roberts Cross** - See thin, sharp edges (noisier)
5. **LoG** - See smooth, continuous edges
6. **DoG** - See prominent features only

### What Each Shows

```
Original Image:
???????????????
?  ??????    ?
?             ?
?  ???????   ?
???????????????

Prewitt Edge:        All outlines
Prewitt X:           | | | (verticals)
Prewitt Y:           — — — (horizontals)
Roberts Cross:       Sharp, thin outlines
LoG:                 Smooth, blob-like
DoG:                 Key features only
```

## Tips & Tricks

### ?? Best Practices

**For Noisy Images**:
1. First: `Process ? Noise Removal ? Gaussian`
2. Then: `Filters ? Edge Detectors ? LoG`
3. Result: Clean edges without noise

**For Feature Detection**:
1. Use: `Filters ? Edge Detectors ? DoG`
2. Then: Threshold if needed
3. Result: Key feature points

**For Direction Analysis**:
1. Run Prewitt X (vertical)
2. Run Prewitt Y (horizontal)
3. Compare layers
4. Result: See which direction dominates

### ? Speed Comparison

From fastest to slowest:
1. **Roberts Cross** ??? (fastest)
2. **Prewitt X/Y** ???
3. **Prewitt Edge** ??
4. **DoG** ?
5. **LoG** ? (slowest, best quality)

### ?? Quality Comparison

From best to good:
1. **LoG** ????? (best, noise-resistant)
2. **DoG** ????
3. **Prewitt Edge** ????
4. **Prewitt X/Y** ???
5. **Roberts Cross** ??? (fast but noisy)

## Undo & Layers

### Undo
- **Ctrl+Z**: Undo last edge detector
- **Undo Button**: Same as Ctrl+Z
- **Layer Remove**: Right-click layer ? Remove

### Compare Results
1. Apply edge detector 1
2. Apply edge detector 2
3. Go to Layers panel
4. Click to toggle visibility
5. See differences instantly!

### Save Best Result
1. Apply multiple detectors
2. Find best one in layers
3. Click that layer
4. `File ? Save Image`

## Common Use Cases

### ?? Document Scanning
**Best**: Prewitt Edge or LoG  
**Why**: Clear text/line boundaries  
**Workflow**: Load scan ? LoG ? Save  

### ?? Architectural Photos
**Best**: Prewitt X + Prewitt Y  
**Why**: Emphasize horizontal/vertical lines  
**Workflow**: Load photo ? Try both ? Compare  

### ?? Medical Images
**Best**: LoG  
**Why**: Noise-resistant, smooth edges  
**Workflow**: Load X-ray ? LoG ? Adjust threshold  

### ?? Feature Detection
**Best**: DoG  
**Why**: Multi-scale, SIFT-compatible  
**Workflow**: Load image ? DoG ? Threshold ? Save  

### ?? Photo Editing
**Best**: Prewitt Edge  
**Why**: Balanced, general-purpose  
**Workflow**: Load photo ? Prewitt ? Adjust  

## Keyboard Shortcuts Summary

| Action | Shortcut |
|--------|----------|
| Load Image | None (use menu) |
| Undo | Ctrl+Z |
| Save | None (use menu) |
| Filters Menu | Alt+F |
| Edge Detectors | Alt+F, E |

## Troubleshooting

### "No processed image to save"
**Solution**: Click "Apply" or wait for processing to finish

### Edges look noisy
**Solution**: 
1. First denoise: `Process ? Noise Removal`
2. Then detect edges: `Filters ? Edge Detectors ? LoG`

### Want only vertical/horizontal
**Solution**: Use Prewitt X or Prewitt Y specifically

### Edges too thick
**Solution**: Try Roberts Cross for thinner edges

### Not enough detail
**Solution**: Try LoG with default parameters

## Next Steps

After Phase 13, try combining edge detectors with:

1. **Morphology** (`Morphology` menu)
   - Dilate edges to make thicker
   - Erode edges to make thinner

2. **Threshold** (`Process ? Binary Threshold`)
   - Convert edges to pure black/white

3. **Invert** (`Process ? Invert Colors`)
   - Reverse edge colors

4. **Blend** (via layers)
   - Combine multiple edge detectors
   - Create custom edge maps

## Summary

? **6 edge detectors** ready to use  
? **Menu integrated** in Filters  
? **Layer support** for comparison  
? **Undo support** with Ctrl+Z  
? **Professional results**  

**Start experimenting!** ??

---

**Quick Test**:
1. Load any image
2. `Filters ? Edge Detectors ? Prewitt Edge`
3. See result instantly!

That's it! You're ready to use Phase 13 edge detectors! ??
