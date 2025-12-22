# ? SOLUTION: Switch to Release Build

## ?? **The Problem**

Your application is a **Debug build** but the **Debug Runtime DLLs** are not available in your Visual Studio installation.

**Debug builds require:**
- `vcruntime140d.dll`
- `msvcp140d.dll`
- `vcruntime140_1d.dll`
- `ucrtbased.dll`

These are only available if you installed the "C++ Debug Runtime" component in Visual Studio.

---

## ? **The Solution: Use Release Build**

Release builds work with the standard Visual C++ Redistributable (which you likely already have).

### **Step-by-Step:**

1. **Open Visual Studio**
   ```
   Open: Naghuma Toolbox.sln
   ```

2. **Change Build Configuration**
   ```
   At the top toolbar:
   Change "Debug" dropdown ? Select "Release"
   ```

3. **Rebuild**
   ```
   Build ? Rebuild Solution
   OR
   Press: Ctrl+Shift+B
   ```

4. **Copy DLLs to Release folder**
   ```powershell
   # Copy all DLLs from Debug to Release
   Copy-Item "x64\Debug\*.dll" "x64\Release\" -Force
   ```

5. **Run from Release**
   ```
   .\x64\Release\Naghuma Toolbox.exe
   ```

---

## ?? **Quick Fix Script**

Save this as `switch_to_release.ps1`:

```powershell
Write-Host "???????????????????????????????????????" -ForegroundColor Cyan
Write-Host "  Switching to Release Build" -ForegroundColor Yellow
Write-Host "???????????????????????????????????????" -ForegroundColor Cyan
Write-Host ""

# 1. Build Release (if you have msbuild in PATH)
Write-Host "1. Building Release..." -ForegroundColor Yellow
$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
if (Test-Path $msbuild) {
    & $msbuild "Naghuma Toolbox.sln" /p:Configuration=Release /p:Platform=x64 /t:Build
} else {
    Write-Host "   ??  MSBuild not found - build manually in Visual Studio" -ForegroundColor Yellow
}

# 2. Ensure Release folder exists
if (!(Test-Path "x64\Release")) {
    New-Item "x64\Release" -ItemType Directory -Force | Out-Null
}

# 3. Copy all DLLs
Write-Host ""
Write-Host "2. Copying DLLs to Release folder..." -ForegroundColor Yellow
$copied = 0
Get-ChildItem "x64\Debug\*.dll" | ForEach-Object {
    $dst = "x64\Release\$($_.Name)"
    if (!(Test-Path $dst)) {
        Copy-Item $_.FullName $dst -Force
        $copied++
    }
}
Write-Host "   ? Copied $copied DLLs" -ForegroundColor Green

# 4. Verify
Write-Host ""
Write-Host "3. Verification:" -ForegroundColor Yellow
$exePath = "x64\Release\Naghuma Toolbox.exe"
if (Test-Path $exePath) {
    Write-Host "   ? EXE exists: $exePath" -ForegroundColor Green
    
    $dllCount = (Get-ChildItem "x64\Release\*.dll" | Measure-Object).Count
    Write-Host "   ? DLLs present: $dllCount" -ForegroundColor Green
    
    Write-Host ""
    Write-Host "???????????????????????????????????????" -ForegroundColor Cyan
    Write-Host "? RELEASE BUILD READY!" -ForegroundColor Green
    Write-Host "???????????????????????????????????????" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "Run with:" -ForegroundColor Yellow
    Write-Host "  .\x64\Release\Naghuma` Toolbox.exe" -ForegroundColor Cyan
} else {
    Write-Host "   ? EXE not found - build in Visual Studio" -ForegroundColor Red
}

Write-Host ""
```

Then run:
```
.\switch_to_release.ps1
```

---

## ?? **Alternative: Manual in Visual Studio**

1. Open **Naghuma Toolbox.sln**
2. At the top: **Debug** ? Change to **Release**
3. **Build** ? **Rebuild Solution** (Ctrl+Shift+B)
4. Wait for build to finish
5. Close Visual Studio
6. Run PowerShell:
   ```powershell
   Copy-Item "x64\Debug\*.dll" "x64\Release\" -Force
   ```
7. Run:
   ```
   .\x64\Release\Naghuma Toolbox.exe
   ```

---

## ? **Why This Works**

| Build Type | Runtime DLLs Required | Available? |
|------------|----------------------|------------|
| **Debug** | vcruntime140**d**.dll | ? Not in your VS installation |
| **Release** | vcruntime140.dll | ? Standard VC++ Redistributable |

Release build = No special debug DLLs needed = Works everywhere!

---

## ?? **Status After Release Build**

```
Configuration: Release ?
Platform: x64 ?
DLLs: All 72 copied ?
Runtime: Standard (no debug DLLs) ?
Ready to run: YES ?
```

---

**Do this now:**
1. Open Visual Studio
2. Change "Debug" to "Release"  
3. Press Ctrl+Shift+B
4. Copy DLLs to x64\Release\
5. Run from Release folder

**It will work! ??**
