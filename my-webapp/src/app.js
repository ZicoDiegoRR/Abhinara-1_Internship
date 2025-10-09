const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const port = 3000;

app.use(express.json());
app.use(express.static('public'));

app.get('/', (req, res) => {
    res.json({ message: 'Welcome to My Web App!', timestamp: new Date().toISOString() });
});

app.get('/users', (req, res) => {
    try {
        const usersData = fs.readFileSync(path.join(__dirname, '../data/users.json'), 'utf8');
        res.json(JSON.parse(usersData));
    } catch (error) {
        res.status(500).json({ error: 'Could not read users data' });
    }
});

app.post('/users', (req, res) => {
    const { name, email } = req.body;

    if (!name || !email) {
        return res.status(400).json({ error: 'Name and email are required' });
    }

    try {
        const usersData = JSON.parse(fs.readFileSync(path.join(__dirname, '../data/users.json'), 'utf8'));
        const newUser = {
            id: Date.now(),
            name,
            email,
            created: new Date().toISOString()
        };

        usersData.users.push(newUser);
        fs.writeFileSync(path.join(__dirname, '../data/users.json'), JSON.stringify(usersData, null, 2));

        res.status(201).json(newUser);
    } catch (error) {
        res.status(500).json({ error: 'Could not save user data' });
    }
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});