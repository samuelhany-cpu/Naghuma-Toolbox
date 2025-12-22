# Fix MSB8013 Error - Add Release|x64 Configuration

## Problem
Your project only has **Debug|x64** configuration, but you're trying to build in **Release|x64** mode.

## Solution

### Option 1: Use Visual Studio GUI (Recommended)

1. **Close Visual Studio**
2. Open Visual Studio again
3. In **Solution Explorer**, right-click on the solution
4. Select **Configuration Manager**
5. In the **Active solution configuration** dropdown, click **<New...>**
6. Name: `Release`
7. Copy settings from: `Debug`
8. Click **OK**
9. The Release|x64 configuration will be created automatically

### Option 2: Switch to Debug Mode

If you just want to test your refactored code:

```powershell
# Build in Debug mode instead
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build
```

### Option 3: Manual Edit (If Option 1 doesn't work)

1. **Close Visual Studio completely**
2. Open `Naghuma Toolbox.vcxproj` in Notepad
3. Find this section (around line 4):
```xml
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
```

4. **Replace with**:
```xml
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|x64">
      <Configuration>Debug</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|x64">
      <Configuration>Release</Configuration>
      <Platform>x64</Platform>
    </ProjectConfiguration>
  </ItemGroup>
```

5. Find this section (around line 17):
```xml
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
```

6. **Add after it**:
```xml
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v143</PlatformToolset>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
```

7. Find this section (around line 25):
```xml
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
```

8. **Add after it**:
```xml
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
```

9. Find the Debug ItemDefinitionGroup section (around line 30) and add this **after it** (before `</Project>`):
```xml
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
```

10. **Save and close** the file
11. Reopen Visual Studio

## Key Differences Between Debug and Release

### Debug Configuration
- Uses debug libraries: `opencv_world430d.lib`, `Qt6Cored.lib`, etc. (note the 'd' suffix)
- Preprocessor: `_DEBUG`
- Optimizations: OFF
- Debug info: Full

### Release Configuration
- Uses release libraries: `opencv_world430.lib`, `Qt6Core.lib` (no 'd' suffix)
- Preprocessor: `NDEBUG`, `QT_NO_DEBUG`
- Optimizations: ON (`WholeProgramOptimization`, `IntrinsicFunctions`)
- Code optimization: `EnableCOMDATFolding`, `OptimizeReferences`

## Verify It Works

After adding the configuration, build in Release mode:

```powershell
msbuild "Naghuma Toolbox.sln" /p:Configuration=Release /p:Platform=x64 /t:Rebuild
```

## About Qt6Guid.dll Error

The second error you showed indicates that `Qt6Guid.dll` is missing. This happens because:

1. **Release builds don't use debug DLLs**
2. You need to ensure Qt release DLLs are in your PATH or copy them to the output directory

### Fix Qt6Guid.dll Missing Error

After building in Release mode, you'll need the release Qt DLLs:
- `Qt6Core.dll` (not Qt6Cored.dll)
- `Qt6Gui.dll` (not Qt6Guid.dll)
- `Qt6Widgets.dll` (not Qt6Widgetsd.dll)

**Copy from**: `C:\Qt\6.7.3\msvc2019_64\bin\`
**To**: `F:\Naghuma Toolbox\x64\Release\`

Or add to your PATH:
```powershell
$env:PATH += ";C:\Qt\6.7.3\msvc2019_64\bin"
```

## Recommendation

For development and testing your refactored code, I recommend:
- **Use Debug mode** (easier to debug, DLLs already set up)
- Only use Release mode when you need to create a production build

```powershell
# For testing refactoring
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build
.\x64\Debug\Naghuma` Toolbox.exe
```
