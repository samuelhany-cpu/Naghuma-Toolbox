# Script to remove all emoji characters from source files

$files = @(
    "MainWindow.cpp",
    "MainWindow.h",
    "ImageCanvas.cpp",
    "ImageCanvas.h",
    "HistogramWidget.cpp",
    "HistogramWidget.h",
    "main.cpp"
)

# Emoji to text replacements
$replacements = @{
    "??" = ""
    "??" = ""
    "???" = ""
    "??" = ""
    "??" = ""
    "?" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "?" = ""
    "?" = ""
    "???" = ""
    "??" = ""
    "?" = ""
    "?" = ""
    "??" = ""
    "?" = ""
    "×" = "x"
    "?" = "x"
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
    "??" = ""
}

foreach ($file in $files) {
    if (Test-Path $file) {
        Write-Host "Processing $file..."
        $content = Get-Content $file -Raw -Encoding UTF8
        
        # Replace each emoji
        foreach ($emoji in $replacements.Keys) {
            $content = $content.Replace($emoji, $replacements[$emoji])
        }
        
        # Save with UTF-8 BOM to preserve special characters
        [System.IO.File]::WriteAllText((Resolve-Path $file).Path, $content, [System.Text.UTF8Encoding]::new($false))
        Write-Host "  - Cleaned $file"
    }
}

Write-Host "`nAll emojis removed! Regenerating MOC files..."
& ".\generate_moc.ps1"

Write-Host "`nDone! Ready to rebuild."
