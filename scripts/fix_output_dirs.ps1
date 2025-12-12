# Fix Output Directories in Naghuma Toolbox.vcxproj
# This fixes the nested directory issue by setting explicit output paths

Write-Host "Fixing output directories in Naghuma Toolbox.vcxproj..." -ForegroundColor Cyan

$projectFile = "F:\Naghuma Toolbox\Naghuma Toolbox.vcxproj"

# Check if VS is running
$vsProcess = Get-Process -Name "devenv" -ErrorAction SilentlyContinue
if ($vsProcess) {
    Write-Host "ERROR: Visual Studio is still running!" -ForegroundColor Red
    Write-Host "Please close Visual Studio first, then run this script again." -ForegroundColor Yellow
    exit 1
}

# Backup
Copy-Item $projectFile "$projectFile.before_output_fix"
Write-Host "Backup created: $projectFile.before_output_fix" -ForegroundColor Green

# Read file
$content = Get-Content $projectFile -Raw

# Add PropertyGroup for output directories after UserMacros
$outputDirs = @'
  <PropertyGroup>
    <OutDir>$(SolutionDir)x64\$(Configuration)\</OutDir>
    <IntDir>$(SolutionDir)x64\$(Configuration)\Intermediate\</IntDir>
  </PropertyGroup>
'@

# Insert after <PropertyGroup Label="UserMacros" />
$content = $content -replace '(<PropertyGroup Label="UserMacros" />)', "`$1`n$outputDirs"

# Save
$content | Set-Content $projectFile -Encoding UTF8

Write-Host "`nOutput directories fixed!" -ForegroundColor Green
Write-Host "Debug output will go to: x64\Debug\" -ForegroundColor White
Write-Host "Release output will go to: x64\Release\" -ForegroundColor White
Write-Host "`nNow run:" -ForegroundColor Cyan
Write-Host '  .\scripts\add_release_config.ps1' -ForegroundColor Yellow
Write-Host "Then rebuild your project." -ForegroundColor Cyan
