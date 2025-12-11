[xml]$xml = Get-Content "Naghuma Toolbox.vcxproj"

# Find the ItemGroup with ClCompile
$compileGroup = $xml.Project.ItemGroup | Where-Object { $_.ClCompile -ne $null } | Select-Object -First 1

# Add MOC files
@("moc_MainWindow.cpp", "moc_ImageCanvas.cpp", "moc_HistogramWidget.cpp") | ForEach-Object {
    $elem = $xml.CreateElement("ClCompile", $xml.Project.NamespaceURI)
    $elem.SetAttribute("Include", $_)
    $compileGroup.AppendChild($elem) | Out-Null
}

$xml.Save((Resolve-Path "Naghuma Toolbox.vcxproj").Path)
Write-Host "MOC files added to project!"
