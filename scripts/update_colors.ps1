# Script to update color scheme in MainWindow.cpp

$file = "MainWindow.cpp"
Write-Host "Updating color scheme in $file..."

$content = Get-Content $file -Raw -Encoding UTF8

# Define color replacements
# Old colors -> New colors
$colorMap = @{
    # Backgrounds - Add more pink/navy tones
    '#0f0a14' = '#1a0a1f'  # Deeper navy-purple background
    '#1a1625' = '#1f1535'  # Navy-purple secondary
    '#2d2838' = '#2d2547'  # Muted navy
    '#4a3f5c' = '#5b4b73'  # Lighter purple-navy
    
    # Accent colors - More pink, baby blue, navy
    '#ff6b9d' = '#ff69b4'  # Hot pink (main pink)
    '#ff85ad' = '#ff8dc7'  # Lighter pink hover
    '#a29bfe' = '#87ceeb'  # Baby blue (from lavender)
    '#ffeaa7' = '#ffb6d9'  # Light pink (from peach)
    '#55efc4' = '#98d8e8'  # Soft baby blue (from mint)
    '#fd79a8' = '#ff91c7'  # Pink
    
    # Additional accent
    '#6c5b7b' = '#7b68ee'  # Medium slate blue
    
    # Keep some colors but adjust
    '#fef5f8' = '#f0f4ff'  # Very light blue-white
    '#dfe6e9' = '#e6f2ff'  # Light blue-white
    '#b2bec3' = '#c8d8eb'  # Light blue-gray
}

# Apply replacements
foreach ($oldColor in $colorMap.Keys) {
    $newColor = $colorMap[$oldColor]
    $content = $content.Replace($oldColor, $newColor)
}

# Save the updated file
[System.IO.File]::WriteAllText("$PWD\$file", $content, [System.Text.Encoding]::UTF8)
Write-Host "Color scheme updated successfully!"
Write-Host ""
Write-Host "New Color Palette:"
Write-Host "  Primary Pink: #ff69b4"
Write-Host "  Baby Blue: #87ceeb"
Write-Host "  Navy Purple: #1a0a1f"
Write-Host "  Light Pink: #ffb6d9"
Write-Host "  Soft Blue: #98d8e8"
Write-Host "  Medium Slate Blue: #7b68ee"
