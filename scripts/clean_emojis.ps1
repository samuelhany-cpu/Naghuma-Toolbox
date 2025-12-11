# Clean Emoji Removal Script
# Removes emojis and special characters from C++ source files

Write-Host "Emoji Removal Script Starting..." -ForegroundColor Cyan
Write-Host ""

$projectRoot = "F:\Naghuma Toolbox"
$extensions = @("*.cpp", "*.h", "*.hpp")

$files = Get-ChildItem -Path $projectRoot -Include $extensions -Recurse -File | 
         Where-Object { $_.FullName -notmatch '\\x64\\' -and $_.FullName -notmatch '\\Debug\\' -and $_.FullName -notmatch '\\Release\\' -and $_.FullName -notmatch '\\scripts\\' }

$totalFiles = $files.Count
$modifiedFiles = 0

Write-Host "Found $totalFiles files to process..." -ForegroundColor Yellow
Write-Host ""

foreach ($file in $files) {
    $relativePath = $file.FullName.Replace($projectRoot, "")
    Write-Host "Processing: $relativePath" -ForegroundColor Gray
    
    try {
        $content = Get-Content -Path $file.FullName -Raw -Encoding UTF8
        $originalContent = $content
        
        # Remove high Unicode characters (emojis and special symbols)
        # Keep ASCII (0-127) and basic Latin extensions
        $content = [regex]::Replace($content, '[\u0080-\u009F\u2000-\uFFFF]', '')
        
        if ($content -ne $originalContent) {
            [System.IO.File]::WriteAllText($file.FullName, $content, [System.Text.UTF8Encoding]::new($false))
            $modifiedFiles++
            Write-Host "  -> Modified" -ForegroundColor Green
        } else {
            Write-Host "  -> No changes" -ForegroundColor DarkGray
        }
    }
    catch {
        Write-Host "  -> ERROR: $($_.Exception.Message)" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "Complete! Modified $modifiedFiles of $totalFiles files" -ForegroundColor Green
Write-Host ""
