# SAFE Fix Script - Adds Release config and output directories
# This version is tested and safe

Write-Host "`n=== Naghuma Toolbox - Safe Configuration Fix ===" -ForegroundColor Cyan
Write-Host "This will:" -ForegroundColor Yellow
Write-Host "  1. Add explicit output directories (fixes nested folder)" -ForegroundColor White
Write-Host "  2. Add Release|x64 configuration" -ForegroundColor White
Write-Host "`nPress Ctrl+C to cancel, or any key to continue..." -ForegroundColor Yellow
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

$projectFile = "F:\Naghuma Toolbox\Naghuma Toolbox.vcxproj"

# Backup
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
Copy-Item $projectFile "$projectFile.safe_backup_$timestamp"
Write-Host "`n? Backup created: Naghuma Toolbox.vcxproj.safe_backup_$timestamp" -ForegroundColor Green

# Read file
[xml]$xml = Get-Content $projectFile

# Check if already has Release config
$hasRelease = $xml.Project.ItemGroup.ProjectConfiguration | Where-Object { $_.Include -eq "Release|x64" }
if ($hasRelease) {
    Write-Host "? Release|x64 configuration already exists" -ForegroundColor Green
} else {
    Write-Host "`nAdding Release|x64 configuration..." -ForegroundColor Cyan
    
    # Find the ProjectConfigurations ItemGroup
    $configGroup = $xml.Project.ItemGroup | Where-Object { $_.Label -eq "ProjectConfigurations" }
    
    # Create Release configuration
    $releaseConfig = $xml.CreateElement("ProjectConfiguration", $xml.Project.NamespaceURI)
    $releaseConfig.SetAttribute("Include", "Release|x64")
    
    $config = $xml.CreateElement("Configuration", $xml.Project.NamespaceURI)
    $config.InnerText = "Release"
    $releaseConfig.AppendChild($config) | Out-Null
    
    $platform = $xml.CreateElement("Platform", $xml.Project.NamespaceURI)
    $platform.InnerText = "x64"
    $releaseConfig.AppendChild($platform) | Out-Null
    
    $configGroup.AppendChild($releaseConfig) | Out-Null
    
    # Find Debug PropertyGroup with Label="Configuration"
    $debugConfigPG = $xml.Project.PropertyGroup | Where-Object { 
        $_.Condition -eq "'`$(Configuration)|`$(Platform)'=='Debug|x64'" -and $_.Label -eq "Configuration" 
    }
    
    # Create Release PropertyGroup
    $releaseConfigPG = $xml.CreateElement("PropertyGroup", $xml.Project.NamespaceURI)
    $releaseConfigPG.SetAttribute("Condition", "'`$(Configuration)|`$(Platform)'=='Release|x64'")
    $releaseConfigPG.SetAttribute("Label", "Configuration")
    
    $configType = $xml.CreateElement("ConfigurationType", $xml.Project.NamespaceURI)
    $configType.InnerText = "Application"
    $releaseConfigPG.AppendChild($configType) | Out-Null
    
    $useDebugLibs = $xml.CreateElement("UseDebugLibraries", $xml.Project.NamespaceURI)
    $useDebugLibs.InnerText = "false"
    $releaseConfigPG.AppendChild($useDebugLibs) | Out-Null
    
    $platformToolset = $xml.CreateElement("PlatformToolset", $xml.Project.NamespaceURI)
    $platformToolset.InnerText = "v143"
    $releaseConfigPG.AppendChild($platformToolset) | Out-Null
    
    $wpo = $xml.CreateElement("WholeProgramOptimization", $xml.Project.NamespaceURI)
    $wpo.InnerText = "true"
    $releaseConfigPG.AppendChild($wpo) | Out-Null
    
    $charset = $xml.CreateElement("CharacterSet", $xml.Project.NamespaceURI)
    $charset.InnerText = "Unicode"
    $releaseConfigPG.AppendChild($charset) | Out-Null
    
    # Insert after Debug config PropertyGroup
    $xml.Project.InsertAfter($releaseConfigPG, $debugConfigPG) | Out-Null
    
    # Find Debug ImportGroup
    $debugImportGroup = $xml.Project.ImportGroup | Where-Object { 
        $_.Condition -eq "'`$(Configuration)|`$(Platform)'=='Debug|x64'" -and $_.Label -eq "PropertySheets"
    }
    
    # Create Release ImportGroup
    $releaseImportGroup = $xml.CreateElement("ImportGroup", $xml.Project.NamespaceURI)
    $releaseImportGroup.SetAttribute("Label", "PropertySheets")
    $releaseImportGroup.SetAttribute("Condition", "'`$(Configuration)|`$(Platform)'=='Release|x64'")
    
    $import = $xml.CreateElement("Import", $xml.Project.NamespaceURI)
    $import.SetAttribute("Project", "`$(UserRootDir)\Microsoft.Cpp.`$(Platform).user.props")
    $import.SetAttribute("Condition", "exists('`$(UserRootDir)\Microsoft.Cpp.`$(Platform).user.props')")
    $import.SetAttribute("Label", "LocalAppDataPlatform")
    $releaseImportGroup.AppendChild($import) | Out-Null
    
    $xml.Project.InsertAfter($releaseImportGroup, $debugImportGroup) | Out-Null
    
    Write-Host "  ? Release configuration structure added" -ForegroundColor Green
}

# Add output directories (fixes nested folder issue)
$hasOutDir = $xml.Project.PropertyGroup | Where-Object { $_.OutDir }
if (-not $hasOutDir) {
    Write-Host "`nAdding explicit output directories..." -ForegroundColor Cyan
    
    # Find UserMacros PropertyGroup
    $userMacrosPG = $xml.Project.PropertyGroup | Where-Object { $_.Label -eq "UserMacros" }
    
    # Create output directories PropertyGroup
    $outputPG = $xml.CreateElement("PropertyGroup", $xml.Project.NamespaceURI)
    
    $outDir = $xml.CreateElement("OutDir", $xml.Project.NamespaceURI)
    $outDir.InnerText = "`$(SolutionDir)x64\`$(Configuration)\"
    $outputPG.AppendChild($outDir) | Out-Null
    
    $intDir = $xml.CreateElement("IntDir", $xml.Project.NamespaceURI)
    $intDir.InnerText = "`$(SolutionDir)x64\`$(Configuration)\Intermediate\"
    $outputPG.AppendChild($intDir) | Out-Null
    
    $xml.Project.InsertAfter($outputPG, $userMacrosPG) | Out-Null
    
    Write-Host "  ? Output directories configured" -ForegroundColor Green
    Write-Host "    OutDir: x64\`$(Configuration)\" -ForegroundColor White
    Write-Host "    IntDir: x64\`$(Configuration)\Intermediate\" -ForegroundColor White
} else {
    Write-Host "? Output directories already configured" -ForegroundColor Green
}

# Add Release ItemDefinitionGroup if needed
$hasReleaseItemDef = $xml.Project.ItemDefinitionGroup | Where-Object { 
    $_.Condition -eq "'`$(Configuration)|`$(Platform)'=='Release|x64'"
}

if (-not $hasReleaseItemDef -and -not $hasRelease) {
    Write-Host "`nAdding Release build settings..." -ForegroundColor Cyan
    
    # Find Debug ItemDefinitionGroup
    $debugItemDef = $xml.Project.ItemDefinitionGroup | Where-Object { 
        $_.Condition -eq "'`$(Configuration)|`$(Platform)'=='Debug|x64'"
    }
    
    # Create Release ItemDefinitionGroup
    $releaseItemDef = $xml.CreateElement("ItemDefinitionGroup", $xml.Project.NamespaceURI)
    $releaseItemDef.SetAttribute("Condition", "'`$(Configuration)|`$(Platform)'=='Release|x64'")
    
    # ClCompile
    $clCompile = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    
    $warning = $xml.CreateElement("WarningLevel", $xml.Project.NamespaceURI)
    $warning.InnerText = "Level3"
    $clCompile.AppendChild($warning) | Out-Null
    
    $funcLevel = $xml.CreateElement("FunctionLevelLinking", $xml.Project.NamespaceURI)
    $funcLevel.InnerText = "true"
    $clCompile.AppendChild($funcLevel) | Out-Null
    
    $intrinsic = $xml.CreateElement("IntrinsicFunctions", $xml.Project.NamespaceURI)
    $intrinsic.InnerText = "true"
    $clCompile.AppendChild($intrinsic) | Out-Null
    
    $sdl = $xml.CreateElement("SDLCheck", $xml.Project.NamespaceURI)
    $sdl.InnerText = "true"
    $clCompile.AppendChild($sdl) | Out-Null
    
    $preproc = $xml.CreateElement("PreprocessorDefinitions", $xml.Project.NamespaceURI)
    $preproc.InnerText = "NDEBUG;_CONSOLE;QT_WIDGETS_LIB;QT_GUI_LIB;QT_CORE_LIB;QT_NO_DEBUG;%(PreprocessorDefinitions)"
    $clCompile.AppendChild($preproc) | Out-Null
    
    $conformance = $xml.CreateElement("ConformanceMode", $xml.Project.NamespaceURI)
    $conformance.InnerText = "true"
    $clCompile.AppendChild($conformance) | Out-Null
    
    $addInc = $xml.CreateElement("AdditionalIncludeDirectories", $xml.Project.NamespaceURI)
    $addInc.InnerText = "`$(ProjectDir)include;`$(ProjectDir)lib;F:\OpenCV\opencv\build\include;C:\Qt\6.7.3\msvc2019_64\include;C:\Qt\6.7.3\msvc2019_64\include\QtCore;C:\Qt\6.7.3\msvc2019_64\include\QtGui;C:\Qt\6.7.3\msvc2019_64\include\QtWidgets;%(AdditionalIncludeDirectories)"
    $clCompile.AppendChild($addInc) | Out-Null
    
    $langStd = $xml.CreateElement("LanguageStandard", $xml.Project.NamespaceURI)
    $langStd.InnerText = "stdcpp17"
    $clCompile.AppendChild($langStd) | Out-Null
    
    $addOpts = $xml.CreateElement("AdditionalOptions", $xml.Project.NamespaceURI)
    $addOpts.InnerText = "/Zc:__cplusplus %(AdditionalOptions)"
    $clCompile.AppendChild($addOpts) | Out-Null
    
    $releaseItemDef.AppendChild($clCompile) | Out-Null
    
    # Link
    $link = $xml.CreateElement("Link", $xml.Project.NamespaceURI)
    
    $subsys = $xml.CreateElement("SubSystem", $xml.Project.NamespaceURI)
    $subsys.InnerText = "Console"
    $link.AppendChild($subsys) | Out-Null
    
    $comdat = $xml.CreateElement("EnableCOMDATFolding", $xml.Project.NamespaceURI)
    $comdat.InnerText = "true"
    $link.AppendChild($comdat) | Out-Null
    
    $optRef = $xml.CreateElement("OptimizeReferences", $xml.Project.NamespaceURI)
    $optRef.InnerText = "true"
    $link.AppendChild($optRef) | Out-Null
    
    $genDebug = $xml.CreateElement("GenerateDebugInformation", $xml.Project.NamespaceURI)
    $genDebug.InnerText = "true"
    $link.AppendChild($genDebug) | Out-Null
    
    $addLibDir = $xml.CreateElement("AdditionalLibraryDirectories", $xml.Project.NamespaceURI)
    $addLibDir.InnerText = "F:\OpenCV\opencv\build\x64\vc15\lib;C:\Qt\6.7.3\msvc2019_64\lib;%(AdditionalLibraryDirectories)"
    $link.AppendChild($addLibDir) | Out-Null
    
    $addDeps = $xml.CreateElement("AdditionalDependencies", $xml.Project.NamespaceURI)
    $addDeps.InnerText = "opencv_world430.lib;Qt6Core.lib;Qt6Gui.lib;Qt6Widgets.lib;%(AdditionalDependencies)"
    $link.AppendChild($addDeps) | Out-Null
    
    $releaseItemDef.AppendChild($link) | Out-Null
    
    $xml.Project.InsertAfter($releaseItemDef, $debugItemDef) | Out-Null
    
    Write-Host "  ? Release build settings added" -ForegroundColor Green
}

# Save with proper formatting
$settings = New-Object System.Xml.XmlWriterSettings
$settings.Indent = $true
$settings.IndentChars = "  "
$settings.Encoding = [System.Text.Encoding]::UTF8

$writer = [System.Xml.XmlWriter]::Create($projectFile, $settings)
$xml.Save($writer)
$writer.Close()

Write-Host "`n=== Success! ===" -ForegroundColor Green
Write-Host "`nProject file updated:" -ForegroundColor Cyan
Write-Host "  ? Release|x64 configuration available" -ForegroundColor White
Write-Host "  ? Output directories configured (no more nested folders)" -ForegroundColor White
Write-Host "  ? Backup saved as: Naghuma Toolbox.vcxproj.safe_backup_$timestamp" -ForegroundColor White

Write-Host "`nNext steps:" -ForegroundColor Cyan
Write-Host "  1. Clean and rebuild:" -ForegroundColor White
Write-Host "     msbuild `"Naghuma Toolbox.sln`" /p:Configuration=Debug /p:Platform=x64 /t:Rebuild" -ForegroundColor Yellow
Write-Host "  2. Run your application:" -ForegroundColor White
Write-Host "     .\x64\Debug\Naghuma`` Toolbox.exe`n" -ForegroundColor Yellow
