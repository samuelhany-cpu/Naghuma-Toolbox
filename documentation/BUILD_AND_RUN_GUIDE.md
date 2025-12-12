# TROUBLESHOOTING & BUILD GUIDE

## Issue Identified and Fixed!

### Problem:
The application wasn't running because **required DLLs were missing** from the Debug folder.

### Solution Applied:
✅ Created `scripts/copy_dlls.ps1` script
✅ Copied all required Qt6 Debug DLLs:
   - Qt6Cored.dll
   - Qt6Guid.dll
   - Qt6Widgetsd.dll
✅ Copied OpenCV Debug DLL:
   - opencv_world430d.dll  
✅ Copied Qt platform plugin:
   - platforms/qwindowsd.dll

---

## How to Build and Run

### Method 1: Quick Run (Recommended if already built)
```batch
"F:\Naghuma Toolbox\RUN_APP.bat"
```
This will:
- Check for DLLs and copy if missing
- Launch the application

### Method 2: Full Rebuild (If you made code changes)

**Option A: Using Visual Studio (Easiest)**
1. Open `Naghuma Toolbox.sln` in Visual Studio
2. Select **Debug** and **x64** configuration
3. Press `Ctrl+Shift+B` to build
4. Press `F5` to run

**Option B: Using Command Line**
1. Open "Developer Command Prompt for VS"
2. Navigate to project folder:
   ```
   cd "F:\Naghuma Toolbox"
   ```
3. Build:
   ```
   msbuild "Naghuma Toolbox.sln" /t:Rebuild /p:Configuration=Debug /p:Platform=x64
   ```
4. Copy DLLs:
   ```
   powershell -ExecutionPolicy Bypass -File "scripts\copy_dlls.ps1"
   ```
5. Run:
   ```
   "x64\Debug\Naghuma Toolbox.exe"
   ```

---

## Verification Steps

### 1. Check if DLLs are present:
```powershell
dir "F:\Naghuma Toolbox\x64\Debug\*.dll"
```

Should show:
- Qt6Cored.dll
- Qt6Guid.dll
- Qt6Widgetsd.dll
- opencv_world430d.dll

### 2. Check if executable exists:
```powershell
Test-Path "F:\Naghuma Toolbox\x64\Debug\Naghuma Toolbox.exe"
```

Should return: `True`

### 3. Check if platforms folder exists:
```powershell
Test-Path "F:\Naghuma Toolbox\x64\Debug\platforms\qwindowsd.dll"
```

Should return: `True`

---

## Common Issues and Solutions

### Issue 1: "Application failed to start"
**Solution:** Run `scripts\copy_dlls.ps1` to copy required DLLs

### Issue 2: "Qt platform plugin not found"
**Solution:** Ensure `platforms/qwindowsd.dll` exists in Debug folder

### Issue 3: "Cannot find opencv_world430d.dll"
**Solution:** 
- Verify OpenCV is installed at `F:\OpenCV\opencv`
- Run DLL copy script

### Issue 4: Build errors about missing ImageFilters.h
**Solution:** 
- Verify `.vcxproj` includes library files
- Run `scripts\update_vcxproj_with_libs.ps1` if needed

---

## Project Status

✅ **Libraries Created:**
- lib/filters/ImageFilters.h & .cpp
- lib/transforms/ImageTransforms.h & .cpp
- lib/histogram/HistogramOperations.h & .cpp

✅ **Project File Updated:**
- All 6 library files added to `.vcxproj`

✅ **DLLs Copied:**
- All Qt6 Debug DLLs: ✅
- OpenCV Debug DLL: ✅
- Qt platform plugin: ✅

✅ **MainWindow Refactored:**
- Library includes added
- Filters menu created
- 9 new filter functions implemented

---

## Quick Commands Reference

### Copy DLLs:
```powershell
powershell -ExecutionPolicy Bypass -File "F:\Naghuma Toolbox\scripts\copy_dlls.ps1"
```

### Run Application:
```batch
"F:\Naghuma Toolbox\x64\Debug\Naghuma Toolbox.exe"
```

### Or use launcher:
```batch
"F:\Naghuma Toolbox\RUN_APP.bat"
```

---

## Dependency Paths

### Qt 6.7.3:
```
C:\Qt\6.7.3\msvc2019_64\
```

### OpenCV 4.3.0:
```
F:\OpenCV\opencv\build\
```

---

## Next Steps

1. **Run the application:**
   ```batch
   "F:\Naghuma Toolbox\RUN_APP.bat"
   ```

2. **Test your filters:**
   - Load an image
   - Go to Filters menu
   - Try Sobel Combined (H+V+D)
   - Try Laplacian Filter

3. **If you make code changes:**
   - Open in Visual Studio
   - Build (Ctrl+Shift+B)
   - Run (F5)

---

## Files Created for Easy Build/Run:

1. **RUN_APP.bat** - Quick launcher
2. **REBUILD_AND_RUN.bat** - Full rebuild and run
3. **scripts/copy_dlls.ps1** - Copy required DLLs
4. **scripts/clean_emojis.ps1** - Remove emojis from code
5. **scripts/update_vcxproj_with_libs.ps1** - Update project file

---

**Status: READY TO RUN!**

All dependencies are in place. The application is ready to launch.
