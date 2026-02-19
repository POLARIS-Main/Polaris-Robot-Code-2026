#!/bin/zsh
# Launch Polaris Driver Station

cd "$(dirname "$0")"

# Check for pyserial
python3 -c "import serial" 2>/dev/null || {
    echo "Installing pyserial..."
    pip3 install pyserial
}

python3 driver_station.py
