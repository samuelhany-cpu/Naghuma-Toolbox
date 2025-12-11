# Fix Duplicate Entries in Project File

$vcxproj = "Naghuma Toolbox.vcxproj"

# Read the file content
$content = Get-Content $vcxproj -Raw

# Count occurrences
$adjustmentCppCount = ([regex]::Matches($content, 'src\\AdjustmentDialog\.cpp')).Count
$adjustmentHCount = ([regex]::Matches($content, 'include\\AdjustmentDialog\.h')).Count
$brushToolCppCount = ([regex]::Matches($content, 'src\\BrushTool\.cpp')).Count
$brushDialogCppCount = ([regex]::Matches($content, 'src\\BrushDialog\.cpp')).Count

Write-Host "Found duplicates:"
Write-Host "  AdjustmentDialog.cpp: $adjustmentCppCount"
Write-Host "  AdjustmentDialog.h: $adjustmentHCount"
Write-Host "  BrushTool.cpp: $brushToolCppCount"
Write-Host "  BrushDialog.cpp: $brushDialogCppCount"

# Remove duplicate lines by splitting, filtering unique, and rejoining
$lines = $content -split "`r`n"
$uniqueLines = @()
$seenLines = @{}

foreach ($line in $lines) {
    $trimmedLine = $line.Trim()
    
    # Skip if this exact line was already added (except for empty lines and XML tags)
    if ($trimmedLine -ne "" -and -not $trimmedLine.StartsWith("<?") -and -not $trimmedLine.StartsWith("<Project")) {
        if ($seenLines.ContainsKey($trimmedLine)) {
            Write-Host "Removing duplicate: $trimmedLine"
            continue
        }
        $seenLines[$trimmedLine] = $true
    }
    
    $uniqueLines += $line
}

# Join back together
$newContent = $uniqueLines -join "`r`n"

# Write back
Set-Content $vcxproj -Value $newContent -NoNewline

Write-Host "`nProject file cleaned!"
