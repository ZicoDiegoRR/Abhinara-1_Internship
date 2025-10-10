[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/jehcvBj5)
# Bash Homework: Backup & Restore System

## Profile

NRP: ...

Name: ...

## Objective
Create two bash scripts that implement a simple backup and restore system for a web application.

## Project Structure
```
my-webapp/
├── src/           # Source code files
├── data/          # Application data
├── config/        # Configuration files
├── logs/          # Log files
└── backups/       # Backup storage directory
```

## Tasks

### Task 1: Create `backup.sh`
Write a script that creates backups of the web application:
- Create a timestamped backup file named: `backup_YYYY-MM-DD_HH-MM-SS.zip`
- Include all directories: `src/`, `data/`, `config/`, `logs/`
- Save the backup to the `backups/` directory
- Display a success message showing the backup filename
- Handle errors gracefully and show error messages if backup fails

### Task 2: Create `restore.sh`
Write a script that restores from the most recent backup:
- Automatically find and select the newest backup file
- Create a safety backup of the current state before restoring
- Extract and restore files from the latest backup
- Show progress messages during the restore process
- Display confirmation when restore is complete
- Handle cases where no backup files exist

## Requirements
- Scripts should provide clear feedback to the user
- Display informative messages about what the script is doing
- Handle errors and show helpful error messages
- Exit with appropriate exit codes (0 for success, 1 for failure)
- Make scripts user-friendly with clear output

## Helpful Commands
- `date +"%Y-%m-%d_%H-%M-%S"` - Generate timestamp
- `zip -r archive.zip folders/` - Create archive
- `ls -t *.zip | head -1` - Get newest file
- `unzip -o archive.zip` - Extract overwriting files
- `[ -f filename ]` - Check if file exists
- `echo "message"` - Display messages to user
