# Script to remove emojis and special Unicode characters from all C++ source files
# Usage: Run this script from the project root directory

Write-Host "========================================" -ForegroundColor Magenta
Write-Host "   Emoji & Special Character Remover   " -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""

# Define the project root directory
$projectRoot = "F:\Naghuma Toolbox"

# File extensions to process
$extensions = @("*.cpp", "*.h", "*.hpp", "*.cc", "*.cxx")

# Get all C++ source and header files
$files = Get-ChildItem -Path $projectRoot -Include $extensions -Recurse -File | 
         Where-Object { $_.FullName -notmatch '\\x64\\' -and $_.FullName -notmatch '\\Debug\\' -and $_.FullName -notmatch '\\Release\\' -and $_.FullName -notmatch '\\scripts\\' }

$totalFiles = $files.Count
$processedFiles = 0
$modifiedFiles = 0

Write-Host "Found $totalFiles files to process..." -ForegroundColor Yellow
Write-Host ""

foreach ($file in $files) {
    $processedFiles++
    $relativePath = $file.FullName.Replace($projectRoot, "").TrimStart("\")
    
    Write-Host "[$processedFiles/$totalFiles] Processing: $relativePath" -ForegroundColor Gray
    
    try {
        # Read the file content
        $content = Get-Content -Path $file.FullName -Raw -Encoding UTF8
        $originalContent = $content
        
        # Remove emojis and special Unicode characters
        # Remove high Unicode characters (keep basic ASCII and Latin)
        # This removes emojis and most special symbols
        
        # Simple replacements for common special characters
        $replacements = @{
            # Box drawing characters
            '│' = '|'
            '─' = '-'
            '┌' = '+'
            '┐' = '+'
            '└' = '+'
            '┘' = '+'
            '├' = '|'
            '┤' = '|'
            '┬' = '-'
            '┴' = '-'
            '┼' = '+'
            '═' = '='
            '║' = '|'
            '╔' = '+'
            '╗' = '+'
            '╚' = '+'
            '╝' = '+'
            '╠' = '|'
            '╣' = '|'
            '╦' = '='
            '╩' = '='
            '╬' = '+'
            '•' = '*'
            '●' = '*'
            '◆' = '*'
            '■' = '#'
            '□' = '#'
            '▪' = '*'
            '▫' = '*'
            '×' = 'x'
            '÷' = '/'
            '±' = '+/-'
            '≈' = '~='
            '≠' = '!='
            '≤' = '<='
            '≥' = '>='
            '™' = '(TM)'
            '©' = '(C)'
            '®' = '(R)'
            '°' = ' degrees'
            '→' = '->'
            '←' = '<-'
            '↑' = '^'
            '↓' = 'v'
            '↔' = '<->'
            '↕' = '^v'
            '…' = '...'
            '—' = '--'
            '–' = '-'
            ''' = "'"
            ''' = "'"
            '"' = '"'
            '"' = '"'
            '„' = '"'
            '‹' = '<'
            '›' = '>'
            '«' = '<<'
            '»' = '>>'
        }
        
        foreach ($key in $replacements.Keys) {
            $content = $content -replace [regex]::Escape($key), $replacements[$key]
        }
        
        # Remove any remaining high Unicode characters (keep basic ASCII and extended Latin)
        # This removes emojis and most special symbols while keeping normal text
        $content = [regex]::Replace($content, '[\u0080-\u009F\u2000-\uFFFF]', '')
        
        # Check if content was modified
        if ($content -ne $originalContent) {
            # Write the cleaned content back to the file
            [System.IO.File]::WriteAllText($file.FullName, $content, [System.Text.UTF8Encoding]::new($false))
            $modifiedFiles++
            Write-Host "  -> Modified and cleaned" -ForegroundColor Green
        } else {
            Write-Host "  -> No changes needed" -ForegroundColor DarkGray
        }
    }
    catch {
        Write-Host "  -> ERROR: $($_.Exception.Message)" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Magenta
Write-Host "             COMPLETE!                  " -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Magenta
Write-Host ""
Write-Host "Summary:" -ForegroundColor Cyan
Write-Host "  Total files processed: $processedFiles" -ForegroundColor White
Write-Host "  Files modified: $modifiedFiles" -ForegroundColor Yellow
Write-Host "  Files unchanged: $($processedFiles - $modifiedFiles)" -ForegroundColor DarkGray
Write-Host ""
Write-Host "All emojis and special characters have been removed!" -ForegroundColor Green
Write-Host ""

# Pause to see results
Read-Host "Press Enter to exit"
