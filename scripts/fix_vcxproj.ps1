# Properly fix duplicate entries in vcxproj

$vcxproj = "Naghuma Toolbox.vcxproj"
$backup = "Naghuma Toolbox.vcxproj.bak"

# Create backup
Copy-Item $vcxproj $backup -Force
Write-Host "Created backup: $backup"

# Read XML
[xml]$xml = Get-Content $vcxproj

# Find all ItemGroup elements
$itemGroups = $xml.Project.ItemGroup

foreach ($itemGroup in $itemGroups) {
    # Process ClCompile items
    if ($itemGroup.ClCompile) {
        $compileItems = @($itemGroup.ClCompile)
        $uniqueIncludes = @{}
        $toRemove = @()
        
        foreach ($item in $compileItems) {
            $include = $item.Include
            if ($uniqueIncludes.ContainsKey($include)) {
                Write-Host "Found duplicate ClCompile: $include"
                $toRemove += $item
            } else {
                $uniqueIncludes[$include] = $true
            }
        }
        
        foreach ($item in $toRemove) {
            $itemGroup.RemoveChild($item) | Out-Null
        }
    }
    
    # Process ClInclude items
    if ($itemGroup.ClInclude) {
        $includeItems = @($itemGroup.ClInclude)
        $uniqueIncludes = @{}
        $toRemove = @()
        
        foreach ($item in $includeItems) {
            $include = $item.Include
            if ($uniqueIncludes.ContainsKey($include)) {
                Write-Host "Found duplicate ClInclude: $include"
                $toRemove += $item
            } else {
                $uniqueIncludes[$include] = $true
            }
        }
        
        foreach ($item in $toRemove) {
            $itemGroup.RemoveChild($item) | Out-Null
        }
    }
}

# Save XML
$xml.Save((Resolve-Path $vcxproj).Path)

Write-Host "`nProject file fixed! Backup saved as $backup"
Write-Host "Please reload the project in Visual Studio"
