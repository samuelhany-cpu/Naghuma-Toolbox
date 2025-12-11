[xml]$xml = Get-Content "Naghuma Toolbox.vcxproj"

# Update C++ standard for all configurations
$xml.Project.ItemDefinitionGroup | ForEach-Object {
    if ($_.ClCompile) {
        # Remove old LanguageStandard if exists
        if ($_.ClCompile.LanguageStandard) {
            $_.ClCompile.RemoveChild($_.ClCompile.LanguageStandard) | Out-Null
        }
        
        # Add C++17 standard
        $langStd = $xml.CreateElement("LanguageStandard", $xml.Project.NamespaceURI)
        $langStd.InnerText = "stdcpp17"
        $_.ClCompile.AppendChild($langStd) | Out-Null
        
        # Add /Zc:__cplusplus flag
        if ($_.ClCompile.AdditionalOptions) {
            $_.ClCompile.AdditionalOptions += " /Zc:__cplusplus"
        } else {
            $addOpts = $xml.CreateElement("AdditionalOptions", $xml.Project.NamespaceURI)
            $addOpts.InnerText = "/Zc:__cplusplus %(AdditionalOptions)"
            $_.ClCompile.AppendChild($addOpts) | Out-Null
        }
    }
}

$xml.Save((Resolve-Path "Naghuma Toolbox.vcxproj").Path)
Write-Host "C++17 standard configured successfully!"
