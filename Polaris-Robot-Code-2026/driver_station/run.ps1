# Launch Polaris Driver Station

# Change to script directory
Set-Location $PSScriptRoot

# Check for pyserial
python -c "import serial" 2>$null

if ($LASTEXITCODE -ne 0) {
    Write-Host "Installing pyserial..." -ForegroundColor Yellow
    pip install pyserial
}

python driver_station.py
