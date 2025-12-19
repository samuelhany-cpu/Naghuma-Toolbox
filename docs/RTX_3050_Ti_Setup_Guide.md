# RTX 3050 Ti GPU Setup Guide

## ? Your GPU is Perfect for Acceleration!

**NVIDIA GeForce RTX 3050 Ti Laptop**
- Compute Capability: **8.6** (Excellent!)
- VRAM: **4GB** (Good for 2K-4K images)
- CUDA Version: **13.1** ?
- Driver: **581.57** ?

---

## ?? Current Status

**What's Working:**
- ? CUDA Toolkit 13.1 installed at: `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1`
- ? GPU driver latest version
- ? `nvcc` compiler working
- ? GPU detected by `nvidia-smi`

**What's Missing:**
- ?? OpenCV with CUDA support
- Your current OpenCV: `C:\opencv\build\x64\vc16\bin\opencv_world4120.dll` (CPU only)

---

## ?? Quick Fix: Get OpenCV with CUDA

### Option 1: Use Pre-built Binaries (Easiest - 10 min)

1. **Download OpenCV with CUDA**
   ```
   Visit: https://github.com/opencv/opencv/releases
   Look for: opencv-4.10.0-windows.exe or newer
   OR search for: "opencv cuda prebuilt windows"
   ```

2. **Install to New Location**
   ```
   Extract to: C:\opencv-cuda
   (Keep your current C:\opencv as backup)
   ```

3. **Verify CUDA DLLs**
   ```powershell
   # Check if these files exist:
   C:\opencv-cuda\build\x64\vc16\bin\opencv_world4xx.dll
   C:\opencv-cuda\build\x64\vc16\bin\opencv_cuda4xx.dll  # This is key!
   C:\opencv-cuda\build\x64\vc16\bin\cudart64_*.dll
   ```

4. **Update Naghuma Toolbox Project**
   - Open Visual Studio
   - Right-click project ? Properties
   - **C/C++ ? General ? Additional Include Directories**
     ```
     C:\opencv-cuda\build\include
     ```
   - **Linker ? General ? Additional Library Directories**
     ```
     C:\opencv-cuda\build\x64\vc16\lib
     ```
   - **Linker ? Input ? Additional Dependencies**
     ```
     opencv_world4100.lib
     opencv_cuda4100.lib
     ```
   - **C/C++ ? Preprocessor ? Preprocessor Definitions**
     - Add: `HAVE_OPENCV_CUDA`

5. **Update System PATH**
   ```powershell
   # Run as Administrator in PowerShell:
   [Environment]::SetEnvironmentVariable(
     "Path",
     $env:Path + ";C:\opencv-cuda\build\x64\vc16\bin",
     "Machine"
   )
   ```

6. **Rebuild and Test**
   ```
   Visual Studio ? Build ? Rebuild Solution
   Run application
   Check console for: "GPU Acceleration initialized successfully"
   ```

---

### Option 2: Build OpenCV with CUDA (Advanced - 2 hours)

**Only if pre-built doesn't work. Follow main guide in `GPU_Acceleration_Guide.md`**

---

## ?? Test GPU Acceleration

After setup, run this test:

1. **Load Naghuma Toolbox**
2. **Load a test image**
3. **Check console output:**
   ```
   Expected:
   ? GPU Acceleration initialized successfully
   ? CUDA Device: NVIDIA GeForce RTX 3050 Ti Laptop
   ? Compute Capability: 8.6
   ? Total Memory: 4096 MB
   ```

4. **Apply Gaussian Blur to 2K image:**
   ```
   Without GPU: ~180ms
   With GPU:    ~15-20ms  (10x faster!)
   ```

---

## ?? Expected Performance (RTX 3050 Ti)

| Operation | CPU (i7) | GPU (3050Ti) | Speedup |
|-----------|----------|--------------|---------|
| Gaussian Blur 2K | 180ms | 15ms | **12x** ? |
| Median Filter 2K | 450ms | 30ms | **15x** ? |
| Sobel Edge 2K | 95ms | 10ms | **9.5x** ? |
| K-Means 2K | 3200ms | 220ms | **14x** ? |
| Morphology 2K | 120ms | 18ms | **6.7x** ? |

**Your 4GB VRAM can handle:**
- Images up to 4096x4096 comfortably
- Multiple operations in sequence
- Real-time preview for most filters

---

## ?? Troubleshooting

### Issue: "opencv_cuda4xx.dll not found"

**Solution:**
```powershell
# Copy CUDA runtime DLLs to your project:
cd "F:\Naghuma Toolbox\x64\Debug"

# Copy from OpenCV CUDA build:
copy "C:\opencv-cuda\build\x64\vc16\bin\opencv_cuda*.dll" .
copy "C:\opencv-cuda\build\x64\vc16\bin\opencv_world*.dll" .

# Copy CUDA runtime (if needed):
copy "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1\bin\cudart64_*.dll" .
copy "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1\bin\cublas64_*.dll" .
copy "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1\bin\cufft64_*.dll" .
```

### Issue: "GPU not detected" but CUDA works

**Solution:**
```cpp
// Check preprocessor definition
Project Properties ? C/C++ ? Preprocessor
Should contain: HAVE_OPENCV_CUDA

// Rebuild completely
Build ? Clean Solution
Build ? Rebuild Solution
```

### Issue: Application crashes

**Solution:**
```
1. Check CUDA compute capability match
   Your GPU: 8.6
   OpenCV built for: Should include 8.0 or 8.6

2. Use debug mode to see exact error
   Build ? Configuration: Debug
   Run and check console

3. Fallback to CPU temporarily
   Remove HAVE_OPENCV_CUDA
   Rebuild
```

---

## ?? Quick Reference

**CUDA Location:**
```
C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v13.1
```

**Current OpenCV (CPU-only):**
```
C:\opencv\build\x64\vc16\bin\opencv_world4120.dll
```

**Needed OpenCV (with CUDA):**
```
C:\opencv-cuda\build\x64\vc16\bin\opencv_world4xx.dll
C:\opencv-cuda\build\x64\vc16\bin\opencv_cuda4xx.dll  ? Must exist!
```

**Project Files:**
```
F:\Naghuma Toolbox\Naghuma Toolbox.vcxproj
F:\Naghuma Toolbox\include\GPUAccelerator.h
F:\Naghuma Toolbox\src\GPUAccelerator.cpp
```

---

## ?? Success Checklist

- [ ] Downloaded OpenCV with CUDA
- [ ] Verified opencv_cuda*.dll exists
- [ ] Updated project include paths
- [ ] Updated project library paths
- [ ] Added HAVE_OPENCV_CUDA preprocessor
- [ ] Updated system PATH
- [ ] Rebuilt project successfully
- [ ] Console shows "GPU Acceleration initialized"
- [ ] Tested Gaussian Blur (should be ~15ms for 2K image)
- [ ] GPU memory usage visible in nvidia-smi

---

## ?? Pro Tips for RTX 3050 Ti

1. **Optimal Image Sizes:**
   - Up to 2048x2048: Excellent performance
   - 2048-4096: Very good performance  
   - Above 4096: May need tiling (4GB VRAM limit)

2. **Batch Operations:**
   ```cpp
   auto& gpu = GPUAccelerator::instance();
   cv::Mat step1 = gpu.gaussianBlur(input, 5, 1.5);
   cv::Mat step2 = gpu.threshold(step1, 127, 255, cv::THRESH_BINARY);
   cv::Mat step3 = gpu.erode(step2, kernel, 1);
   // All 3 operations in < 50ms!
   ```

3. **Monitor GPU Usage:**
   ```powershell
   # In separate terminal:
   nvidia-smi -l 1
   # Shows real-time GPU usage and memory
   ```

---

## ?? Need Help?

**If stuck, check:**
1. Console output in Naghuma Toolbox (press F12 or check debug window)
2. Build output in Visual Studio
3. nvidia-smi for GPU status
4. Windows Event Viewer for crashes

**Common Issues Repository:**
https://github.com/samuelhany-cpu/Naghuma-Toolbox/issues

---

**Your RTX 3050 Ti is perfect for GPU acceleration! ??**

Follow Option 1 above and you'll have 10-15x faster processing in 10 minutes!
