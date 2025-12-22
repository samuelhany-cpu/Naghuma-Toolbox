# ============================================================================
# DEEP DIVE XML FIX - Check for Hidden Characters
# ============================================================================

Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "  ANALYZING XML ERROR IN DETAIL     " -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

$projectFile = "Naghuma Toolbox.vcxproj"

# Read as bytes to see hidden characters
Write-Host "[1/3] Reading file as bytes to find hidden characters..." -ForegroundColor Yellow
$bytes = [System.IO.File]::ReadAllBytes($projectFile)
$lines = Get-Content $projectFile

Write-Host "  File size: $($bytes.Length) bytes" -ForegroundColor Cyan
Write-Host "  Line count: $($lines.Length)" -ForegroundColor Cyan
Write-Host ""

# Check line 106 in detail
Write-Host "[2/3] Examining line 106 byte-by-byte..." -ForegroundColor Yellow
$line106 = $lines[105]
Write-Host "  Line 106 text: [$line106]" -ForegroundColor White
Write-Host "  Line 106 length: $($line106.Length) characters" -ForegroundColor Cyan

# Show each character with its byte value
Write-Host "  Character breakdown:" -ForegroundColor Yellow
for ($i = 0; $i -lt $line106.Length; $i++) {
    $char = $line106[$i]
    $code = [int][char]$char
    
    if ($char -eq ' ') {
        Write-Host "    [$i] = SPACE (byte $code)" -ForegroundColor Gray
    }
    elseif ($char -eq "`t") {
        Write-Host "    [$i] = TAB (byte $code)" -ForegroundColor Yellow
    }
    elseif ($code -lt 32 -or $code -gt 126) {
        Write-Host "    [$i] = SPECIAL (byte $code)" -ForegroundColor Red
    }
    else {
        Write-Host "    [$i] = '$char' (byte $code)" -ForegroundColor White
    }
}
Write-Host ""

# Try parsing XML and capture exact error
Write-Host "[3/3] Attempting XML parse to see exact error..." -ForegroundColor Yellow
try {
    [xml]$xml = Get-Content $projectFile -Raw
    Write-Host "  ERROR: XML parsed successfully! But Visual Studio still fails?" -ForegroundColor Red
    Write-Host "  This suggests Visual Studio is using a different parser." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "  Trying alternate fix..." -ForegroundColor Cyan
} catch {
    Write-Host "  Parse Error: $($_.Exception.Message)" -ForegroundColor Red
    
    if ($_.Exception.Message -match 'line (\d+)') {
        $errorLine = [int]$matches[1]
        Write-Host "  Error occurs at line: $errorLine" -ForegroundColor Yellow
        
        Write-Host ""
        Write-Host "  Content around error line:" -ForegroundColor Cyan
        for ($i = ($errorLine - 3); $i -lt ($errorLine + 3); $i++) {
            if ($i -ge 0 -and $i -lt $lines.Length) {
                $marker = if ($i -eq ($errorLine - 1)) { " >>> " } else { "     " }
                Write-Host "$marker Line $($i+1): $($lines[$i])" -ForegroundColor White
            }
        }
    }
}

Write-Host ""
Write-Host "Creating sanitized version..." -ForegroundColor Yellow
