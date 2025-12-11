[xml]$xml = Get-Content "Naghuma Toolbox.vcxproj"

$qtPath = "C:\Qt\6.7.3\msvc2019_64"
$qtIncludePath = "$qtPath\include;$qtPath\include\QtCore;$qtPath\include\QtGui;$qtPath\include\QtWidgets"
$qtLibPath = "$qtPath\lib"

# Find and update Debug|x64 configuration
$debugNode = $xml.Project.ItemDefinitionGroup | Where-Object { $_.Condition -eq "'`$(Configuration)|`$(Platform)'=='Debug|x64'" }
if ($debugNode) {
    $debugNode.ClCompile.AdditionalIncludeDirectories = "F:\OpenCV\opencv\build\include;$qtIncludePath;%(AdditionalIncludeDirectories)"
    $debugNode.ClCompile.PreprocessorDefinitions = "_DEBUG;_CONSOLE;QT_WIDGETS_LIB;QT_GUI_LIB;QT_CORE_LIB;%(PreprocessorDefinitions)"
    $debugNode.Link.AdditionalLibraryDirectories = "F:\OpenCV\opencv\build\x64\vc15\lib;$qtLibPath;%(AdditionalLibraryDirectories)"
    $debugNode.Link.AdditionalDependencies = "opencv_world430d.lib;Qt6Cored.lib;Qt6Guid.lib;Qt6Widgetsd.lib;%(AdditionalDependencies)"
}

# Find and update Release|x64 configuration
$releaseNode = $xml.Project.ItemDefinitionGroup | Where-Object { $_.Condition -eq "'`$(Configuration)|`$(Platform)'=='Release|x64'" }
if ($releaseNode) {
    $releaseNode.ClCompile.AdditionalIncludeDirectories = "F:\OpenCV\opencv\build\include;$qtIncludePath;%(AdditionalIncludeDirectories)"
    $releaseNode.ClCompile.PreprocessorDefinitions = "NDEBUG;_CONSOLE;QT_WIDGETS_LIB;QT_GUI_LIB;QT_CORE_LIB;QT_NO_DEBUG;%(PreprocessorDefinitions)"
    $releaseNode.Link.AdditionalLibraryDirectories = "F:\OpenCV\opencv\build\x64\vc15\lib;$qtLibPath;%(AdditionalLibraryDirectories)"
    $releaseNode.Link.AdditionalDependencies = "opencv_world430.lib;Qt6Core.lib;Qt6Gui.lib;Qt6Widgets.lib;%(AdditionalDependencies)"
}

# Update source files
$compileGroup = $xml.Project.ItemGroup | Where-Object { $_.ClCompile -ne $null } | Select-Object -First 1
if ($compileGroup) {
    $compileGroup.RemoveAll()
    @("main.cpp", "MainWindow.cpp", "ImageCanvas.cpp", "HistogramWidget.cpp") | ForEach-Object {
        $elem = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
        $elem.SetAttribute("Include", $_)
        $compileGroup.AppendChild($elem) | Out-Null
    }
}

# Update header files
$headerGroup = $xml.Project.ItemGroup | Where-Object { $_.ClInclude -ne $null } | Select-Object -First 1
if ($headerGroup) {
    $headerGroup.ParentNode.RemoveChild($headerGroup) | Out-Null
}

$newHeaderGroup = $xml.CreateElement("ItemGroup", $xml.Project.NamespaceURI)
@("MainWindow.h", "ImageCanvas.h", "HistogramWidget.h") | ForEach-Object {
    $elem = $xml.CreateElement("ClInclude", $xml.Project.NamespaceURI)
    $elem.SetAttribute("Include", $_)
    $newHeaderGroup.AppendChild($elem) | Out-Null
}

$importNode = $xml.Project.Import | Where-Object { $_.Project -eq '$(VCTargetsPath)\Microsoft.Cpp.targets' }
$xml.Project.InsertBefore($newHeaderGroup, $importNode) | Out-Null

$xml.Save((Resolve-Path "Naghuma Toolbox.vcxproj").Path)
Write-Host "Qt configuration applied successfully!"
