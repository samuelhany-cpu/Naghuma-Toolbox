# Fix the missing processedInfoLabel initialization in MainWindow.cpp

$file = "src\MainWindow.cpp"
$content = Get-Content $file -Raw

# Find the line where we need to add the missing code
$searchPattern = "processedLayout->addWidget\(processedContainer, 1\);\s+// Metrics label"

# The code to insert
$insertCode = @"
processedLayout->addWidget(processedContainer, 1);
    
    // Initialize processedInfoLabel (was missing - caused null pointer crash)
    processedInfoLabel = new QLabel("No processing yet");
    processedInfoLabel->setStyleSheet("color: #c4b5fd; padding: 8px; font-size: 9pt; font-weight: 500;");
    processedInfoLabel->setAlignment(Qt::AlignCenter);
    processedLayout->addWidget(processedInfoLabel);
    
    // Metrics label
"@

# Replace the pattern
$newContent = $content -replace $searchPattern, $insertCode

# Write the fixed content back
Set-Content -Path $file -Value $newContent -NoNewline

Write-Host "Fixed processedInfoLabel initialization in MainWindow.cpp" -ForegroundColor Green
Write-Host "The null pointer crash should now be resolved." -ForegroundColor Green
