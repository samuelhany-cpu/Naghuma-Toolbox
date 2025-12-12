# Add Release Configuration to Naghuma Toolbox.vcxproj
# Run this script AFTER closing Visual Studio

Write-Host "Adding Release|x64 configuration to Naghuma Toolbox.vcxproj..." -ForegroundColor Cyan

$projectFile = "F:\Naghuma Toolbox\Naghuma Toolbox.vcxproj"

# Check if VS is running
$vsProcess = Get-Process -Name "devenv" -ErrorAction SilentlyContinue
if ($vsProcess) {
    Write-Host "ERROR: Visual Studio is still running!" -ForegroundColor Red
    Write-Host "Please close Visual Studio first, then run this script again." -ForegroundColor Yellow
    exit 1
}

# Read the file
$content = Get-Content $projectFile -Raw

# Check if Release config already exists
if ($content -match 'Release\|x64') {
    Write-Host "Release|x64 configuration already exists!" -ForegroundColor Green
    exit 0
}

# Backup the original
Copy-Item $projectFile "$projectFile.before_release_config"
Write-Host "Backup created: $projectFile.before_release_config" -ForegroundColor Green

# Add Release to ProjectConfigurations
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

# Save the modified content
$content | Set-Content $projectFile -Encoding UTF8

Write-Host "`nRelease|x64 configuration added successfully!" -ForegroundColor Green
Write-Host "`nYou can now:" -ForegroundColor Cyan
Write-Host "1. Open Visual Studio" -ForegroundColor White
Write-Host "2. Build in Release mode" -ForegroundColor White
Write-Host "`nOr build from command line:" -ForegroundColor Cyan
Write-Host 'msbuild "Naghuma Toolbox.sln" /p:Configuration=Release /p:Platform=x64 /t:Rebuild' -ForegroundColor Yellow
