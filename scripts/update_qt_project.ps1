$xml = [xml](Get-Content "Naghuma Toolbox.vcxproj")
$ns = New-Object System.Xml.XmlNamespaceManager($xml.NameTable)
$ns.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003")

# Qt paths
$qtPath = "C:\Qt\6.7.3\msvc2019_64"
$qtInclude = "$qtPath\include"
$qtLib = "$qtPath\lib"

# Find Debug|x64 ItemDefinitionGroup
$debugx64 = $xml.SelectSingleNode("//ns:ItemDefinitionGroup[@Condition=`"'`$(Configuration)|`$(Platform)'=='Debug|x64'`"]", $ns)
$debugCompile = $debugx64.SelectSingleNode("ns:ClCompile", $ns)
$debugLink = $debugx64.SelectSingleNode("ns:Link", $ns)

# Update include directories for Debug
$debugInclude = $debugCompile.SelectSingleNode("ns:AdditionalIncludeDirectories", $ns)
if ($debugInclude -eq $null) {
    $debugInclude = $xml.CreateElement("AdditionalIncludeDirectories", "http://schemas.microsoft.com/developer/msbuild/2003")
    $debugCompile.AppendChild($debugInclude) | Out-Null
}
$debugInclude.InnerText = "F:\OpenCV\opencv\build\include;$qtInclude;$qtInclude\QtCore;$qtInclude\QtGui;$qtInclude\QtWidgets;%(AdditionalIncludeDirectories)"

# Add preprocessor definitions for Qt
$debugPreproc = $debugCompile.SelectSingleNode("ns:PreprocessorDefinitions", $ns)
if ($debugPreproc -eq $null) {
    $debugPreproc = $xml.CreateElement("PreprocessorDefinitions", "http://schemas.microsoft.com/developer/msbuild/2003")
    $debugCompile.AppendChild($debugPreproc) | Out-Null
}
$debugPreproc.InnerText = "_DEBUG;_CONSOLE;QT_WIDGETS_LIB;QT_GUI_LIB;QT_CORE_LIB;%(PreprocessorDefinitions)"

# Update library directories for Debug
$debugLibDir = $debugLink.SelectSingleNode("ns:AdditionalLibraryDirectories", $ns)
if ($debugLibDir -ne $null) {
    $debugLibDir.InnerText = "F:\OpenCV\opencv\build\x64\vc15\lib;$qtLib;%(AdditionalLibraryDirectories)"
}

# Update dependencies for Debug
$debugDeps = $debugLink.SelectSingleNode("ns:AdditionalDependencies", $ns)
if ($debugDeps -ne $null) {
    $debugDeps.InnerText = "opencv_world430d.lib;Qt6Cored.lib;Qt6Guid.lib;Qt6Widgetsd.lib;%(AdditionalDependencies)"
}

# Find Release|x64 ItemDefinitionGroup
$releasex64 = $xml.SelectSingleNode("//ns:ItemDefinitionGroup[@Condition=`"'`$(Configuration)|`$(Platform)'=='Release|x64'`"]", $ns)
$releaseCompile = $releasex64.SelectSingleNode("ns:ClCompile", $ns)
$releaseLink = $releasex64.SelectSingleNode("ns:Link", $ns)

# Update include directories for Release
$releaseInclude = $releaseCompile.SelectSingleNode("ns:AdditionalIncludeDirectories", $ns)
if ($releaseInclude -eq $null) {
    $releaseInclude = $xml.CreateElement("AdditionalIncludeDirectories", "http://schemas.microsoft.com/developer/msbuild/2003")
    $releaseCompile.AppendChild($releaseInclude) | Out-Null
}
$releaseInclude.InnerText = "F:\OpenCV\opencv\build\include;$qtInclude;$qtInclude\QtCore;$qtInclude\QtGui;$qtInclude\QtWidgets;%(AdditionalIncludeDirectories)"

# Add preprocessor definitions for Qt (Release)
$releasePreproc = $releaseCompile.SelectSingleNode("ns:PreprocessorDefinitions", $ns)
if ($releasePreproc -eq $null) {
    $releasePreproc = $xml.CreateElement("PreprocessorDefinitions", "http://schemas.microsoft.com/developer/msbuild/2003")
    $releaseCompile.AppendChild($releasePreproc) | Out-Null
}
$releasePreproc.InnerText = "NDEBUG;_CONSOLE;QT_WIDGETS_LIB;QT_GUI_LIB;QT_CORE_LIB;QT_NO_DEBUG;%(PreprocessorDefinitions)"

# Update library directories for Release
$releaseLibDir = $releaseLink.SelectSingleNode("ns:AdditionalLibraryDirectories", $ns)
if ($releaseLibDir -ne $null) {
    $releaseLibDir.InnerText = "F:\OpenCV\opencv\build\x64\vc15\lib;$qtLib;%(AdditionalLibraryDirectories)"
}

# Update dependencies for Release
$releaseDeps = $releaseLink.SelectSingleNode("ns:AdditionalDependencies", $ns)
if ($releaseDeps -ne $null) {
    $releaseDeps.InnerText = "opencv_world430.lib;Qt6Core.lib;Qt6Gui.lib;Qt6Widgets.lib;%(AdditionalDependencies)"
}

# Update ItemGroup with new files
$itemGroup = $xml.SelectSingleNode("//ns:ItemGroup[ns:ClCompile]", $ns)

# Clear existing files
$itemGroup.RemoveAll()

# Add all cpp files
$files = @("main.cpp", "MainWindow.cpp", "ImageCanvas.cpp", "HistogramWidget.cpp")
foreach ($file in $files) {
    $compile = $xml.CreateElement("ClCompile", "http://schemas.microsoft.com/developer/msbuild/2003")
    $compile.SetAttribute("Include", $file)
    $itemGroup.AppendChild($compile) | Out-Null
}

# Add header files ItemGroup
$existingHeaderGroup = $xml.SelectSingleNodes("//ns:ItemGroup[ns:ClInclude]", $ns)
foreach ($group in $existingHeaderGroup) {
    $group.ParentNode.RemoveChild($group) | Out-Null
}

$headerGroup = $xml.CreateElement("ItemGroup", "http://schemas.microsoft.com/developer/msbuild/2003")
$headers = @("MainWindow.h", "ImageCanvas.h", "HistogramWidget.h")
foreach ($header in $headers) {
    $headerNode = $xml.CreateElement("ClInclude", "http://schemas.microsoft.com/developer/msbuild/2003")
    $headerNode.SetAttribute("Include", $header)
    $headerGroup.AppendChild($headerNode) | Out-Null
}

# Insert header group before the Import element
$importNode = $xml.SelectSingleNode("//ns:Import[@Project=`"`$(VCTargetsPath)\Microsoft.Cpp.targets`"]", $ns)
$xml.Project.InsertBefore($headerGroup, $importNode) | Out-Null

$xml.Save("$PWD\Naghuma Toolbox.vcxproj")
Write-Host "Project file updated with Qt configuration successfully!"
Write-Host "Qt Include: $qtInclude"
Write-Host "Qt Lib: $qtLib"
