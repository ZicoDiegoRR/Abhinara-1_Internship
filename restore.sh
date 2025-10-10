#!/bin/bash

# Restore script for web application
# Restores from the most recent backup file

# Configuration
PROJECT_DIR="my-webapp"
BACKUP_DIR="$PROJECT_DIR/backups"

# TODO: Check if backup directory exists and check if any backup files exist
if [[ ! -d $BACKUP_DIR || -z $(ls -A $BACKUP_DIR) ]]; then
    echo "Couldn't find $BACKUP_DIR or any backup file. Make sure to backup the project first."
    echo "Aborting..."
    exit 1
fi

# TODO: Find the most recent backup file
echo "Getting the last backup at $BACKUP_DIR..."
for backup in $(ls $BACKUP_DIR | sort -r); do
    last_backup=$backup
    break
done

# TODO: Display which backup will be restored
echo "Found the last backup at $last_backup"

# TODO: Create safety backup of current state
echo "Creating another backup for safety..."
./backup.sh

# TODO: Restore from the latest backup
echo "Backup has been created."
echo "Restoring..."
unzip "$BACKUP_DIR/$last_backup" -d .
echo "Successfully restored the backup at $last_backup."
echo "Finishing..."
exit 0
