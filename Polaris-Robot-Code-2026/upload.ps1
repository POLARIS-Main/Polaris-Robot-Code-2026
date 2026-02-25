# Polaris Robot - Teensy Upload Script

# Change to script directory
Set-Location $PSScriptRoot

# Colors
$RED = "$([char]0x1b)[0;31m"
$GREEN = "$([char]0x1b)[0;32m"
$YELLOW = "$([char]0x1b)[1;33m"
$BLUE = "$([char]0x1b)[0;34m"
$NC = "$([char]0x1b)[0m"

Write-Host "${BLUE}=== Polaris Teensy Upload ===${NC}"
Write-Host ""

# Build first
Write-Host "${YELLOW}Building...${NC}"
python -m platformio run

if ($LASTEXITCODE -ne 0) {
    Write-Host "${RED}Build failed!${NC}"
    exit 1
}

Write-Host ""
Write-Host "${GREEN}Build successful!${NC}"
Write-Host ""
Write-Host "${YELLOW}Uploading to Teensy...${NC}"
Write-Host "${BLUE}(Press the PROGRAM button on Teensy if it doesn't start)${NC}"
Write-Host ""

# Upload
python -m platformio run -t upload

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "${GREEN}$([char]0x2713) Upload complete!${NC}"
    Write-Host ""
    Write-Host "${YELLOW}Run the Driver Station to control the robot:${NC}"
    Write-Host "  .\driver_station\run.ps1"
} else {
    Write-Host ""
    Write-Host "${RED}Upload failed!${NC}"
    Write-Host "${YELLOW}Try: Press and hold PROGRAM button, then run this script again${NC}"
}
