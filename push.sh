#!/bin/zsh
# Polaris Robot Code - Quick Push Script

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "${BLUE}=== Polaris Robot Push Script ===${NC}"

# Check if we're in a git repository
if ! git rev-parse --is-inside-work-tree > /dev/null 2>&1; then
    echo "${RED}Error: Not in a git repository${NC}"
    exit 1
fi

# Show current branch
BRANCH=$(git branch --show-current)
echo "${YELLOW}Current branch:${NC} $BRANCH"

# Show status
echo "\n${YELLOW}Changed files:${NC}"
git status --short

# Check if there are any changes
if [ -z "$(git status --porcelain)" ]; then
    echo "${GREEN}No changes to commit!${NC}"
    exit 0
fi

# Get commit message
echo "\n${YELLOW}Enter commit message (or press Enter for default):${NC}"
read -r MESSAGE

if [ -z "$MESSAGE" ]; then
    MESSAGE="Update $(date '+%Y-%m-%d %H:%M')"
fi

# Stage all changes
echo "\n${BLUE}Staging changes...${NC}"
git add -A

# Commit
echo "${BLUE}Committing...${NC}"
git commit -m "$MESSAGE"

# Push
echo "${BLUE}Pushing to origin/$BRANCH...${NC}"
if git push origin "$BRANCH"; then
    echo "\n${GREEN}✓ Successfully pushed to $BRANCH!${NC}"
else
    echo "\n${RED}Push failed. You may need to pull first or set upstream:${NC}"
    echo "  git push --set-upstream origin $BRANCH"
fi
