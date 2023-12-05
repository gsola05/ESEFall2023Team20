const SerialPort = require('serialport');

const parsers = SerialPort.parsers;
const parser = new parsers.ReadLine({
  delimiter: '\r\n'
})

const port = new SerialPort('COM3', {
  baudRate: 9600,
  dataBite: 8,
  parity: 'none',
  stopBits: 1,
  flowControl: false
});

port.pipe(parser);

parser.on('data', function(data) {
  console.log(data);
});