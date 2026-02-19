#!/bin/zsh
# Polaris Robot - Teensy Upload Script

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo "${BLUE}=== Polaris Teensy Upload ===${NC}"
echo ""

# Build first
echo "${YELLOW}Building...${NC}"
~/.platformio/penv/bin/pio run

if [ $? -ne 0 ]; then
    echo "${RED}Build failed!${NC}"
    exit 1
fi

echo ""
echo "${GREEN}Build successful!${NC}"
echo ""
echo "${YELLOW}Uploading to Teensy...${NC}"
echo "${BLUE}(Press the PROGRAM button on Teensy if it doesn't start)${NC}"
echo ""

# Upload
~/.platformio/penv/bin/pio run -t upload

if [ $? -eq 0 ]; then
    echo ""
    echo "${GREEN}✓ Upload complete!${NC}"
    echo ""
    echo "${YELLOW}Run the Driver Station to control the robot:${NC}"
    echo "  ./driver_station/run.sh"
else
    echo ""
    echo "${RED}Upload failed!${NC}"
    echo "${YELLOW}Try: Press and hold PROGRAM button, then run this script again${NC}"
fi
