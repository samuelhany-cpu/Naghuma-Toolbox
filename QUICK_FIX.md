# ?? Drawing Mode Crash - QUICK FIX GUIDE

## ?? The Problem
Clicking "Drawing Mode: OFF" ? Program crashes instantly!

## ? The Solution
**Change 3 lines** in `src/MainWindow.cpp` around **line 1665-1690**

---

## ?? Step-by-Step Fix

### 1. Open `src/MainWindow.cpp`

### 2. Find the `toggleDrawingMode()` function (around line 1650)

### 3. Look for this section (around line 1665):
```cpp
// Enable/disable mouse events on processed canvas
processedCanvas->setMouseEventsEnabled(drawingMode);

if (drawingMode) {
    // Start with current image as drawing canvas
    try {
        drawingCanvas = currentImage.clone();
        
        if (drawingCanvas.empty()) {
            // ... error handling ...
        }
        
        processedImage = drawingCanvas.clone();
        recentlyProcessed = true;
        updateDisplay();
        updateStatus("Drawing mode ENABLED...", "success");
```

### 4. **DELETE** this line:
```cpp
processedCanvas->setMouseEventsEnabled(drawingMode);  // ? DELETE THIS!
```

### 5. **REPLACE** these lines:
```cpp
processedImage = drawingCanvas.clone();
recentlyProcessed = true;
updateDisplay();
updateStatus("Drawing mode ENABLED...", "success");
```

**With these lines:**
```cpp
processedImage = drawingCanvas.clone();
recentlyProcessed = true;
processedCanvas->setImage(processedImage);       // ? ADD THIS!
processedCanvas->setMouseEventsEnabled(true);    // ? ADD THIS!
updateStatus("Drawing mode ENABLED. Click and drag on the processed image to draw!", "success");
```

### 6. Find the `else` block (around line 1700):
```cpp
} else {
    // Finalize drawing
    if (!drawingCanvas.empty()) {
```

### 7. **ADD** this line at the start of the `else` block:
```cpp
} else {
    // Disable mouse events first
    processedCanvas->setMouseEventsEnabled(false);  // ? ADD THIS LINE!
    
    // Finalize drawing
    if (!drawingCanvas.empty()) {
```

### 8. Save the file

### 9. Rebuild:
```powershell
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build
```

---

## ?? What You Changed

### Before (Crashed):
```cpp
processedCanvas->setMouseEventsEnabled(drawingMode);  // ? Mouse enabled on empty canvas!

if (drawingMode) {
    drawingCanvas = currentImage.clone();
    processedImage = drawingCanvas.clone();
    recentlyProcessed = true;
    updateDisplay();  // ? Image shown AFTER mouse enabled = CRASH!
}
```

### After (Works!):
```cpp
if (drawingMode) {
    drawingCanvas = currentImage.clone();
    processedImage = drawingCanvas.clone();
    recentlyProcessed = true;
    processedCanvas->setImage(processedImage);       // ? Image shown FIRST
    processedCanvas->setMouseEventsEnabled(true);    // ? Mouse enabled AFTER = SAFE!
    updateStatus(...);
} else {
    processedCanvas->setMouseEventsEnabled(false);   // ? Disable when off
}
```

---

## ?? Test It!

1. Run the app
2. Load an image
3. Click "Drawing Mode: OFF" button
4. It should turn to "Drawing Mode: ON" **without crashing**
5. Draw on the processed canvas (right side)
6. Click button again to save drawing

---

## ?? Why It Crashed

### The Bug:
1. Mouse events were enabled **before** showing the image
2. You clicked ? mouse handler ran
3. Handler tried to access `scaledPixmap.width()`
4. But `scaledPixmap` was **NULL** ? **ACCESS VIOLATION** ? **CRASH!**

### The Fix:
1. Show the image **first** (creates valid pixmaps)
2. **Then** enable mouse events (safe to map coordinates)
3. Order matters!

---

## ?? Complete Fixed Function

See `FIX_DRAWING_CRASH.txt` for the complete fixed function you can copy-paste!

---

## ? After the Fix

You'll be able to:
- ? Enable drawing mode without crashing
- ? Draw with mouse in real-time
- ? Change brush size, color, opacity
- ? Save drawings as layers
- ? Use all 3 brush types (Circle, Square, Soft Edge)

---

**Need the complete function?** Open `FIX_DRAWING_CRASH.txt`

**Still having issues?** Make sure you:
1. Deleted the old `setMouseEventsEnabled(drawingMode)` line
2. Added both new lines in the correct order
3. Added the disable line in the `else` block
4. Saved and rebuilt the project
