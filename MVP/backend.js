var http = require("http");
var fs = require('fs');

var index = fs.readFileSync('index.html');


var SerialPort = require('serialport');

const parsers = SerialPort.parsers;
const parser = new parsers.ReadLine({
  delimeter: '\r\n'
})

const port = new SerialPort('COM3', {
  baudRate: 9600,
  dataBite: 8,
  parity: 'none',
  stopBits: 1,
  flowControl: false
});

port.pipe(parser);

var app = http.createServer(function(req, res) {
  res.writeHead(200, {'Content-Type':'text/html'});
  res.end(index);
});

var io = require('socket.io').listen(app);

io.on('connection', function(data) {
  console.log('Node.js is listening');
})

parser.on('data', function(data) {
  console.log(data);
  io.emit('data', data);
});

app.listen(3000);