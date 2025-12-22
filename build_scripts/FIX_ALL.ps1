# Master Fix Script - Fixes all project configuration issues
# Run this AFTER closing Visual Studio

Write-Host "`n========================================" -ForegroundColor Magenta
Write-Host "  Naghuma Toolbox - Master Fix Script" -ForegroundColor Magenta
Write-Host "========================================`n" -ForegroundColor Magenta

# Check if VS is running
$vsProcess = Get-Process -Name "devenv" -ErrorAction SilentlyContinue
if ($vsProcess) {
    Write-Host "? ERROR: Visual Studio is still running!" -ForegroundColor Red
    Write-Host "`nPlease:" -ForegroundColor Yellow
    Write-Host "  1. Close Visual Studio" -ForegroundColor White
    Write-Host "  2. Run this script again" -ForegroundColor White
    exit 1
}

$projectFile = "F:\Naghuma Toolbox\Naghuma Toolbox.vcxproj"

# Create backup
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$backupFile = "$projectFile.backup_$timestamp"
Copy-Item $projectFile $backupFile
Write-Host "? Backup created: Naghuma Toolbox.vcxproj.backup_$timestamp`n" -ForegroundColor Green

# Read file
$content = Get-Content $projectFile -Raw

# ============================================
# FIX 1: Remove any nested output directories
# ============================================
Write-Host "?? Fix 1: Cleaning up nested directories..." -ForegroundColor Cyan
if (Test-Path "F:\Naghuma Toolbox\Naghuma Toolbox\") {
    Remove-Item "F:\Naghuma Toolbox\Naghuma Toolbox\" -Recurse -Force -ErrorAction SilentlyContinue
    Write-Host "   Removed: F:\Naghuma Toolbox\Naghuma Toolbox\" -ForegroundColor Yellow
}
Write-Host "   ? Directory structure clean`n" -ForegroundColor Green

# ============================================
# FIX 2: Add explicit output directories
# ============================================
Write-Host "?? Fix 2: Setting explicit output directories..." -ForegroundColor Cyan
if ($content -notmatch '<OutDir>') {
    $outputDirs = @'
  <PropertyGroup>
    <OutDir>$(SolutionDir)x64\$(Configuration)\</OutDir>
    <IntDir>$(SolutionDir)x64\$(Configuration)\Intermediate\</IntDir>
  </PropertyGroup>
'@
    $content = $content -replace '(<PropertyGroup Label="UserMacros" />)', "`$1`n$outputDirs"
    Write-Host "   ? Output directories configured" -ForegroundColor Green
    Write-Host "     Debug   ? x64\Debug\" -ForegroundColor White
    Write-Host "     Release ? x64\Release\`n" -ForegroundColor White
} else {
    Write-Host "   ? Output directories already configured`n" -ForegroundColor Green
}

# ============================================
# FIX 3: Add Release|x64 configuration
# ============================================
Write-Host "?? Fix 3: Adding Release|x64 configuration..." -ForegroundColor Cyan
if ($content -notmatch 'Release\|x64') {
    # Add to ProjectConfigurations
    $content = $content -replace `
        '(<ProjectConfiguration Include="Debug\|x64">[\s\S]*?</ProjectConfiguration>)', `
        '$1
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>'
    
    # Add Release PropertyGroup
    $releasePropertyGroup = @'
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
'@
    $content = $content -replace `
        '(</PropertyGroup>\s*<Import Project="\$\(VCTargetsPath\)\\Microsoft\.Cpp\.props" />)', `
        "$releasePropertyGroup`n  `$1"
    
    # Add Release ImportGroup
    $releaseImportGroup = @'
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
'@
    $content = $content -replace `
        '(<ImportGroup Label="PropertySheets" Condition.*?Debug.*?</ImportGroup>)', `
        "`$1`n$releaseImportGroup"
    
    # Add Release ItemDefinitionGroup
    $releaseItemDef = @'
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>NDEBUG;_CONSOLE;QT_WIDGETS_LIB;QT_GUI_LIB;QT_CORE_LIB;QT_NO_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <AdditionalIncludeDirectories>$(ProjectDir)include;$(ProjectDir)lib;F:\OpenCV\opencv\build\include;C:\Qt\6.7.3\msvc2019_64\include;C:\Qt\6.7.3\msvc2019_64\include\QtCore;C:\Qt\6.7.3\msvc2019_64\include\QtGui;C:\Qt\6.7.3\msvc2019_64\include\QtWidgets;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <LanguageStandard>stdcpp17</LanguageStandard>
      <AdditionalOptions>/Zc:__cplusplus %(AdditionalOptions)</AdditionalOptions>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <AdditionalLibraryDirectories>F:\OpenCV\opencv\build\x64\vc15\lib;C:\Qt\6.7.3\msvc2019_64\lib;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
      <AdditionalDependencies>opencv_world430.lib;Qt6Core.lib;Qt6Gui.lib;Qt6Widgets.lib;%(AdditionalDependencies)</AdditionalDependencies>
    </Link>
  </ItemDefinitionGroup>
'@
    $content = $content -replace `
        '(<ItemDefinitionGroup Condition.*?Debug.*?</ItemDefinitionGroup>)', `
        "`$1`n$releaseItemDef"
    
    Write-Host "   ? Release|x64 configuration added`n" -ForegroundColor Green
} else {
    Write-Host "   ? Release|x64 configuration already exists`n" -ForegroundColor Green
}

# Save
$content | Set-Content $projectFile -Encoding UTF8

# ============================================
# Summary
# ============================================
Write-Host "`n========================================" -ForegroundColor Green
Write-Host "  ? ALL FIXES APPLIED SUCCESSFULLY!" -ForegroundColor Green
Write-Host "========================================`n" -ForegroundColor Green

Write-Host "What was fixed:" -ForegroundColor Cyan
Write-Host "  ? Nested directory removed" -ForegroundColor White
Write-Host "  ? Output directories configured" -ForegroundColor White
Write-Host "  ? Release|x64 configuration added`n" -ForegroundColor White

Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "  1. Open Visual Studio" -ForegroundColor White
Write-Host "  2. Build in Debug mode:" -ForegroundColor White
Write-Host "     msbuild `"Naghuma Toolbox.sln`" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild" -ForegroundColor Yellow
Write-Host "  3. Run the application:" -ForegroundColor White
Write-Host "     .\x64\Debug\Naghuma` Toolbox.exe`n" -ForegroundColor Yellow

Write-Host "Your refactored code is ready to test! ??" -ForegroundColor Green
