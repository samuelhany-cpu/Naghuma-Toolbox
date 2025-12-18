# Comprehensive Fix and Build Script for Naghuma Toolbox
Write-Host "`n=== Naghuma Toolbox - Build Fix & Compile ===" -ForegroundColor Cyan

# Step 1: Generate MOC file for ColorProcessingDialog
Write-Host "`n[1/4] Generating MOC file for ColorProcessingDialog..." -ForegroundColor Yellow
$qtMoc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"

if (-not (Test-Path $qtMoc)) {
    Write-Host "X Qt MOC not found at $qtMoc" -ForegroundColor Red
    Write-Host "Please install Qt 6.7.3 or update the path" -ForegroundColor Yellow
    exit 1
}

$mocArgs = @(
    "-IC:\Qt\6.7.3\msvc2019_64\include",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtCore",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtGui",
    "-IC:\Qt\6.7.3\msvc2019_64\include\QtWidgets",
    "-Iinclude",
    "-DQT_WIDGETS_LIB",
    "-DQT_GUI_LIB",
    "-DQT_CORE_LIB",
    "include\ColorProcessingDialog.h",
    "-o",
    "src\moc_ColorProcessingDialog.cpp"
)

& $qtMoc $mocArgs

if (Test-Path "src\moc_ColorProcessingDialog.cpp") {
    Write-Host "  + MOC file generated successfully" -ForegroundColor Green
} else {
    Write-Host "  X MOC generation failed" -ForegroundColor Red
    exit 1
}

# Step 2: Add missing files to vcxproj
Write-Host "`n[2/4] Updating Visual Studio project file..." -ForegroundColor Yellow
$vcxprojPath = "Naghuma Toolbox.vcxproj"

if (-not (Test-Path $vcxprojPath)) {
    Write-Host "  X Project file not found: $vcxprojPath" -ForegroundColor Red
    exit 1
}

# Backup
$backupPath = "${vcxprojPath}.backup_$(Get-Date -Format 'yyyyMMdd_HHmmss')"
Copy-Item $vcxprojPath $backupPath
Write-Host "  + Backup created: $backupPath" -ForegroundColor Green

# Load XML
[xml]$xml = Get-Content $vcxprojPath

# Find ItemGroups
$compileGroup = $xml.Project.ItemGroup | Where-Object { $_.ClCompile } | Select-Object -First 1
$includeGroup = $xml.Project.ItemGroup | Where-Object { $_.ClInclude } | Select-Object -First 1

# Add ColorProcessingDialog.cpp if missing
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\ColorProcessingDialog.cpp" }
if (-not $exists) {
    $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\ColorProcessingDialog.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  + Added src\ColorProcessingDialog.cpp" -ForegroundColor Green
}

# Add moc_ColorProcessingDialog.cpp if missing
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\moc_ColorProcessingDialog.cpp" }
if (-not $exists) {
    $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\moc_ColorProcessingDialog.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  + Added src\moc_ColorProcessingDialog.cpp" -ForegroundColor Green
}

# Add ColorProcessor.cpp if missing
$exists = $compileGroup.ClCompile | Where-Object { $_.Include -eq "lib\color\ColorProcessor.cpp" }
if (-not $exists) {
    $newCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $newCompile.SetAttribute("Include", "lib\color\ColorProcessor.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  + Added lib\color\ColorProcessor.cpp" -ForegroundColor Green
}

# Add ColorProcessingDialog.h if missing
$exists = $includeGroup.ClInclude | Where-Object { $_.Include -eq "include\ColorProcessingDialog.h" }
if (-not $exists) {
    $newInclude = $xml.CreateElement("ClInclude", $xml.Project.NamespaceURI)
    $newInclude.SetAttribute("Include", "include\ColorProcessingDialog.h")
    $includeGroup.AppendChild($newInclude) | Out-Null
    Write-Host "  + Added include\ColorProcessingDialog.h" -ForegroundColor Green
}

# Add ColorProcessor.h if missing
$exists = $includeGroup.ClInclude | Where-Object { $_.Include -eq "lib\color\ColorProcessor.h" }
if (-not $exists) {
    $newInclude = $xml.CreateElement("ClInclude", $xml.Project.NamespaceURI)
    $newInclude.SetAttribute("Include", "lib\color\ColorProcessor.h")
    $includeGroup.AppendChild($newInclude) | Out-Null
    Write-Host "  + Added lib\color\ColorProcessor.h" -ForegroundColor Green
}

# Save
$xml.Save((Resolve-Path $vcxprojPath).Path)
Write-Host "  + Project file updated" -ForegroundColor Green

# Step 3: Clean previous build
Write-Host "`n[3/4] Cleaning previous build..." -ForegroundColor Yellow
msbuild "Naghuma Toolbox.sln" /t:Clean /p:Configuration=Debug /p:Platform=x64 /nologo /v:minimal

# Step 4: Build project
Write-Host "`n[4/4] Building project..." -ForegroundColor Yellow
msbuild "Naghuma Toolbox.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /nologo /v:minimal

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n=== + BUILD SUCCESSFUL ===" -ForegroundColor Green
    Write-Host "`nExecutable: x64\Debug\Naghuma Toolbox.exe`n" -ForegroundColor Cyan
    
    # Run the application
    Write-Host "Launch application? (Y/N): " -ForegroundColor Yellow -NoNewline
    $response = Read-Host
    if ($response -eq 'Y' -or $response -eq 'y') {
        Start-Process "x64\Debug\Naghuma Toolbox.exe"
    }
} else {
    Write-Host "`n=== X BUILD FAILED ===" -ForegroundColor Red
    Write-Host "`nPlease check the errors above.`n" -ForegroundColor Yellow
    exit 1
}
