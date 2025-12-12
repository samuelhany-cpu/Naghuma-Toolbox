# How to Run Naghuma Toolbox

## Quick Start (Easiest Method)

**Just double-click:** `Launch Naghuma Toolbox.bat`

This will automatically:
- Check for required DLLs
- Copy them if needed
- Set up the environment
- Launch the application

## Alternative Methods

### Method 1: PowerShell Script
```powershell
.\scripts\run_app.ps1
```

### Method 2: Build and Run
```powershell
.\scripts\final_build.ps1
```
This will build the project and launch it with all required DLLs.

### Method 3: Manual Launch
1. First time only - copy DLLs:
   ```powershell
   .\scripts\setup_dlls.ps1
   ```

2. Run the application:
   ```
   x64\Debug\Naghuma Toolbox.exe
   ```

## Troubleshooting

### "Qt6Cored.dll was not found"
**Solution:** Run the DLL setup script:
```powershell
.\scripts\setup_dlls.ps1
```

### "opencv_world430d.dll was not found"
**Solution:** Run the DLL setup script (same as above).

### "Application failed to start - platform plugin"
**Solution:** The `platforms` folder with `qwindowsd.dll` is missing.
Run:
```powershell
.\scripts\setup_dlls.ps1
```

### Application won't start
1. Check that the build was successful
2. Verify DLLs are present in `x64\Debug\`
3. Try running from PowerShell to see error messages:
   ```powershell
   & ".\x64\Debug\Naghuma Toolbox.exe"
   ```

## What Gets Copied

The setup script copies these DLLs to `x64\Debug\`:

### Qt DLLs
- `Qt6Cored.dll` - Qt Core (Debug)
- `Qt6Guid.dll` - Qt GUI (Debug)
- `Qt6Widgetsd.dll` - Qt Widgets (Debug)

### Qt Platforms Plugin
- `platforms\qwindowsd.dll` - Windows platform integration

### OpenCV DLLs
- `opencv_world430d.dll` - OpenCV world module (Debug)

## Release Build

For release builds, you need to:

1. Build in Release mode:
   ```powershell
   msbuild "Naghuma Toolbox.sln" /p:Configuration=Release /p:Platform=x64
   ```

2. Copy release DLLs (without 'd' suffix):
   - Qt6Core.dll
   - Qt6Gui.dll
   - Qt6Widgets.dll
   - platforms\qwindows.dll
   - opencv_world430.dll

## Project Structure

```
Naghuma Toolbox/
??? Launch Naghuma Toolbox.bat  ? Double-click to run
??? scripts/
?   ??? run_app.ps1             ? PowerShell launcher
?   ??? setup_dlls.ps1          ? DLL setup script
?   ??? final_build.ps1         ? Build and run
?   ??? ...
??? x64/Debug/
    ??? Naghuma Toolbox.exe     ? The application
    ??? Qt6Cored.dll            ? Qt DLLs (copied)
    ??? opencv_world430d.dll    ? OpenCV DLL (copied)
    ??? platforms/
        ??? qwindowsd.dll       ? Qt platform plugin (copied)
```

## Features Available

Once running, you can:

? **Load Images** - PNG, JPG, BMP, TIFF formats
? **Apply Filters** - Laplacian, Sobel, Traditional, Pyramidal, Circular, Cone
? **Transform Images** - Translate, Rotate, Skew, Zoom, Flip
? **Adjust Images** - Grayscale, Threshold, Blur, Edge Detection, Invert
? **View Histogram** - Real-time histogram in right sidebar
? **Manage Layers** - See processing history, remove layers
? **Save Results** - Save processed images

## Need Help?

- Check the main README.md for full documentation
- See docs/MODULAR_ARCHITECTURE.md for code structure
- See docs/INTEGRATION_GUIDE.md for development info
