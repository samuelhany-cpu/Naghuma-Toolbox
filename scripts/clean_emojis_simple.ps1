$files = @("MainWindow.cpp")

foreach ($file in $files) {
    Write-Host "Cleaning $file..."
    $content = Get-Content $file -Raw -Encoding UTF8
    
    # Remove common emoji patterns that show as ??
    $content = $content -replace '[^\x00-\x7F]+', ''
    
    # Fix known issues
    $content = $content -replace '\?\?+', ''
    $content = $content -replace '===+', '==============================================='
    
    # Save
    Set-Content -Path $file -Value $content -Encoding UTF8 -NoNewline
    Write-Host "Cleaned $file"
}
