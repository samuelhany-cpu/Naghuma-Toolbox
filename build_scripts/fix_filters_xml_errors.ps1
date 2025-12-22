# Fix vcxproj.filters XML Errors
# This script fixes malformed XML in the Naghuma Toolbox.vcxproj.filters file

Write-Host "========================================" -ForegroundColor Cyan
Write-Host " Fixing vcxproj.filters XML Errors" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$filtersPath = "Naghuma Toolbox.vcxproj.filters"

# Check if file exists
if (-not (Test-Path $filtersPath)) {
    Write-Host "ERROR: File not found: $filtersPath" -ForegroundColor Red
    exit 1
}

Write-Host "`n[1/3] Creating backup..." -ForegroundColor Yellow
$backupPath = "$filtersPath.backup"
Copy-Item $filtersPath $backupPath -Force
Write-Host "  - Backup created: $backupPath" -ForegroundColor Green

Write-Host "`n[2/3] Loading and parsing XML file..." -ForegroundColor Yellow

try {
    # Load as XML
    [xml]$xml = Get-Content $filtersPath -Raw
    
    Write-Host "  - XML loaded successfully" -ForegroundColor Green
    
    # Find and fix any malformed ClInclude elements around line 210
    $includeGroup = $xml.Project.ItemGroup | Where-Object { $_.ClInclude -ne $null } | Select-Object -First 1
    
    if ($includeGroup) {
        Write-Host "  - Found ClInclude group" -ForegroundColor Green
        
        # Check for ColorProcessor.h
        $colorProcessorHeader = $includeGroup.ClInclude | Where-Object { $_.Include -eq "lib\color\ColorProcessor.h" }
        
        if ($null -eq $colorProcessorHeader) {
            Write-Host "  - Adding ColorProcessor.h to headers" -ForegroundColor Yellow
            
            $newInclude = $xml.CreateElement("ClInclude", $xml.DocumentElement.NamespaceURI)
            $newInclude.SetAttribute("Include", "lib\color\ColorProcessor.h")
            
            $filter = $xml.CreateElement("Filter", $xml.DocumentElement.NamespaceURI)
            $filter.InnerText = "Header Files"
            $newInclude.AppendChild($filter) | Out-Null
            
            $includeGroup.AppendChild($newInclude) | Out-Null
            Write-Host "  - Added ColorProcessor.h" -ForegroundColor Green
        } else {
            Write-Host "  - ColorProcessor.h already exists" -ForegroundColor Green
        }
    }
    
    Write-Host "`n[3/3] Saving fixed XML file..." -ForegroundColor Yellow
    
    # Save with proper formatting
    $settings = New-Object System.Xml.XmlWriterSettings
    $settings.Indent = $true
    $settings.IndentChars = "  "
    $settings.NewLineChars = "`r`n"
    $settings.Encoding = [System.Text.UTF8Encoding]::new($false) # UTF-8 without BOM
    
    $writer = [System.Xml.XmlWriter]::Create($filtersPath, $settings)
    $xml.Save($writer)
    $writer.Close()
    
    Write-Host "  - File saved successfully" -ForegroundColor Green
    
    Write-Host "`n========================================" -ForegroundColor Green
    Write-Host " Fix Complete!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "`nThe vcxproj.filters file has been fixed." -ForegroundColor White
    Write-Host "Backup saved as: $backupPath" -ForegroundColor Gray
    
} catch {
    Write-Host "`nERROR: Failed to parse/fix XML file" -ForegroundColor Red
    Write-Host "Error details: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "`nAttempting alternative fix method..." -ForegroundColor Yellow
    
    # Alternative: Text-based fix
    Write-Host "`nUsing text-based repair..." -ForegroundColor Yellow
    
    $content = Get-Content $filtersPath -Raw
    
    # Remove any malformed entries (lines with issues)
    # Fix common XML issues
    $content = $content -replace '<ClInclude\s+Include="[^"]*"\s*/>\s*(?=<ClInclude)', ''
    
    # Ensure ColorProcessor.h is in the file
    if ($content -notmatch 'ColorProcessor\.h') {
        # Find the ClInclude ItemGroup and add ColorProcessor.h
        $insertPoint = $content.IndexOf('</ItemGroup>', $content.IndexOf('<ClInclude'))
        if ($insertPoint -gt 0) {
            $insertion = @"
    <ClInclude Include="lib\color\ColorProcessor.h">
      <Filter>Header Files</Filter>
    </ClInclude>
"@
            $content = $content.Insert($insertPoint, $insertion + "`r`n    ")
        }
    }
    
    # Save the fixed content
    $content | Set-Content $filtersPath -NoNewline -Encoding UTF8
    
    Write-Host "  - Text-based repair completed" -ForegroundColor Green
    Write-Host "`n========================================" -ForegroundColor Green
    Write-Host " Alternative Fix Applied!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
}

Write-Host "`nNext steps:" -ForegroundColor Cyan
Write-Host "1. Close Visual Studio completely" -ForegroundColor White
Write-Host "2. Reopen Visual Studio" -ForegroundColor White
Write-Host "3. Load the project" -ForegroundColor White
Write-Host "4. Build the solution (F7)" -ForegroundColor White

Write-Host "`nIf errors persist, restore from backup:" -ForegroundColor Yellow
Write-Host "  Copy-Item '$backupPath' '$filtersPath' -Force" -ForegroundColor Gray
