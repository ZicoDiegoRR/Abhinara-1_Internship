# My Web App

A simple Express.js web application for demonstrating backup and maintenance operations.

## Features

- User management API
- JSON data storage
- Configuration management
- Logging system

## Getting Started

1. Install dependencies:
   ```bash
   npm install
   ```

2. Start the application:
   ```bash
   npm start
   ```

3. The app will be available at http://localhost:3000

## API Endpoints

- `GET /` - Welcome message
- `GET /users` - Get all users
- `POST /users` - Create a new user

## Project Structure

```
my-webapp/
├── src/           # Application source code
├── data/          # JSON data files
├── config/        # Configuration files
├── logs/          # Application logs
├── backups/       # Backup storage directory
└── package.json   # Project dependencies
```