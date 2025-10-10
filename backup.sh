#!/bin/bash

# Backup script for web application
# Creates timestamped backup of src/, data/, config/, and logs/ directories

# Configuration
PROJECT_DIR="my-webapp"
BACKUP_DIR="$PROJECT_DIR/backups"

# TODO: Generate timestamp using date command
echo "Generating filename for the backup..."
timestamp="$(date +"%Y-%m-%d_%H_%M_%S")"

# TODO: Create backup filename
filename_backup="Backup_$timestamp.zip"
echo "Filename has been created." 
echo "Searching for the backup dictionary..."

# TODO: Check if project directory exists
if [ ! -d $BACKUP_DIR ]; then
    # TODO: Create backups directory if it doesn't exist
    echo "$BACKUP_DIR doesn't exist. Creating one..."
    mkdir -p $BACKUP_DIR
fi

echo "$BACKUP_DIR has been found." 
echo "Listing all files..."
# TODO: Create the backup
to_be_saved=()
for file in $PROJECT_DIR/*; do
    if [ ! "$file" = "$BACKUP_DIR" ]; then
        to_be_saved+=($file)
    fi
done

for element in "${to_be_saved[@]}"; do
    if [ -d $element ]; then
        for file in $element/*; do
            to_be_saved+=($file)
        done
    fi
done

for file in "${to_be_saved[@]}"; do
    echo "  - $file"
done

echo "All files have been listed."
echo "Creating the backup..."
zip "$BACKUP_DIR/$filename_backup" ${to_be_saved[@]}
echo "Successfully created a backup at $BACKUP_DIR/$filename_backup." 
echo "Exiting..."
exit 0