# Polaris Robot Code - Quick Push Script

Write-Host "=== Polaris Robot Push Script ===" -ForegroundColor Blue

# Check if we're in a git repository
try {
    git rev-parse --is-inside-work-tree 2>&1 | Out-Null
    if ($LASTEXITCODE -ne 0) {
        throw "Not in a git repository"
    }
} catch {
    Write-Host "Error: Not in a git repository" -ForegroundColor Red
    exit 1
}

# Show current branch
$branch = git branch --show-current
Write-Host "Current branch: " -ForegroundColor Yellow -NoNewline
Write-Host $branch

# Show status
Write-Host "`nChanged files:" -ForegroundColor Yellow
git status --short

# Check if there are any changes
$status = git status --porcelain
if ([string]::IsNullOrEmpty($status)) {
    Write-Host "No changes to commit!" -ForegroundColor Green
    exit 0
}

# Get commit message
Write-Host "`nEnter commit message (or press Enter for default):" -ForegroundColor Yellow
$message = Read-Host

if ([string]::IsNullOrEmpty($message)) {
    $message = "Update $(Get-Date -Format 'yyyy-MM-dd HH:mm')"
}

# Stage all changes
Write-Host "`nStaging changes..." -ForegroundColor Blue
git add -A

# Commit
Write-Host "Committing..." -ForegroundColor Blue
git commit -m $message

# Push
Write-Host "Pushing to origin/$branch..." -ForegroundColor Blue
git push origin $branch

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n✓ Successfully pushed to origin/$branch" -ForegroundColor Green
} else {
    Write-Host "`nPush failed!" -ForegroundColor Red
    exit 1
}
