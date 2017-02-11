/**
 * Bluetoooth - manages Robot bluetooth connection using bluetooth-serial-port
 */


/**
 * Constructor. Configures the bluetooth connection
 * @param address local robot bluetooth device address
 * @param channel local bluetooth serial device channel
 * @param lineCallback callback function that will be called after receiving a line of data from robot through bluetooth
*/ 
function Bluetooth(address, channel, callback) {
	this.address = address;
	this.channel = channel;

	var BTSP = require('bluetooth-serial-port');
	this.serial = new BTSP.BluetoothSerialPort();
	this.setup(callback);
}


/**
 * Returns a bluetooth-serial-port instance
 */
Bluetooth.prototype.serial = function() {
	return this.serial;	
};


/**
 * Setups bluetooth connection
 */
Bluetooth.prototype.setup = function(callback) {
	var serial = this.serial;
	var that = this;
	
	// configure bluetooth connection
	serial.on('found', function(address, name) {
		serial.findSerialPortChannel(address, function(channel) {
			// connection
			serial.connect(that.address, that.channel, function() {
				that.log('connected!');
				
				// data reception
				var tmp = '';
				serial.on('data', function(buffer) {
					var chunk = buffer.toString('ascii');
				
					// split received data in lines (separated by newline char)
					var i=0, line='', offset=0;
					tmp += chunk;
					while ( (i=tmp.indexOf('\n', offset)) !== -1) {
						line = tmp.substr(offset, i-offset);
						offset = i + 1;
						that.processLine(line, callback);
					}
					tmp = tmp.substr(offset);
				});
			}, function() {
				that.log('findSerialPortChannel error!');
			});
		});
	});

	serial.on('close', function() {
		that.log('closed!');
	});

	serial.on('failure', function(err) {
		that.log('failure!');
		that.log(err);
		//that.log('trying to re-connect');
		//that.serial.inquire();
	});
};


/**
 * Process a line of bluetooth received data
 */
Bluetooth.prototype.processLine = function(line, callback) {
	this.log(line);
	callback(line);
}


/**
 * Logs a blueetooth message to console
 */
Bluetooth.prototype.log = function(msg) {
	console.log('[bluetooth] ' + msg);
}


module.exports = Bluetooth;
