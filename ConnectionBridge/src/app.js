const app = require('express')();
const net = require('net');
const http = require('http').Server(app);
const io = require('socket.io')(http);

const DATA_SERVER_URL = "localhost";
const DATA_SERVER_PORT = 54000;

io.on("connection", socket => {
    var client = new net.Socket();

    client.connect(DATA_SERVER_PORT, DATA_SERVER_URL, function() {
        console.log('Connection opened');
        socket.emit('connectClient', `Connected to ${DATA_SERVER_URL} on port ${DATA_SERVER_PORT}`);
    });

    client.on('data', function(data) {
        console.log('Passing data: ' + data);
        socket.emit('data', data.toString());
    });
    
    client.on('close', function() {
        console.log('Connection closed');
        socket.emit('disconnectClient', `Disconnected from ${DATA_SERVER_URL}`);
        socket.leaveAll();
    });
    
    client.on('error', function(error) {
        console.log('Error: ' + error);
        socket.emit('receivederror', error);
    })
});

http.listen(4444, () => {
    console.log('Listening on port 4444');
});