const {SerialPort} = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline')
const port = new SerialPort({path: '/dev/ttyACM0', baudRate: 9600 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));
// Read the port data
port.on("open", () => {
  console.log('serial port open');
});
parser.on('data', data =>{
  console.log('got word from arduino:', data);
});

port.write('hello from node\n', (err) => {
    if (err) {
      return console.log('Error on write: ', err.message);
    }
    console.log('message written');
});

function receiveData() {
    endMarker = '\n'; // message separator
    receivedChar;     // read char from serial port
    ndx = 0;          // current index of data buffer
    // clean data buffer
    memset(data, 32, sizeof(data));
    // read while we have data available and we are
    // still receiving the same message.
    while(Serial.available() > 0) {
      receivedChar = Serial.read();
      if (receivedChar == endMarker) {
        data[ndx] = '\0'; // end current message
        return;
      }
      // looks like a valid message char, so append it and
      // increment our index
      data[ndx] = receivedChar;
      ndx++;
      // if the message is larger than our max size then
      // stop receiving and clear the data buffer. this will
      // most likely cause the next part of the message
      // to be truncated as well, but hopefully when you
      // parse the message, you'll be able to tell that it's
      // not a valid message.
      if (ndx >= DATA_MAX_SIZE) {
        break;
      }
    }
    // no more available bytes to read from serial and we
    // did not receive the separato. it's an incomplete message!
    Serial.println("error: incomplete message");
    Serial.println(data);
    memset(data, 32, sizeof(data));
};