var address = '98:D3:33:80:82:00';
var channel = 1;

var BTSP = require('bluetooth-serial-port');
var serial = new BTSP.BluetoothSerialPort();

serial.on('found', function(address, name) {

	serial.findSerialPortChannel(address, function(channel) {
		serial.connect(address, channel, function() {
			console.log('connected');
			
			/*			
			serial.on('data', function(buffer) {
				var chunk = buffer.toString('ascii');
				console.log(chunk);
			});
			*/
			
						
			var tmp = '';
			serial.on('data', function(buffer) {
				var chunk = buffer.toString('ascii');
				
				var i=0, piece='', offset=0;
				tmp += chunk;
				while ( (i=tmp.indexOf('\n', offset)) !== -1) {
					piece = tmp.substr(offset, i-offset);
					offset = i+1;
					newPiece(piece);
				}
				tmp = tmp.substr(offset);
			});
			
		});
	});
});

serial.inquire();


function newPiece(msg) {
	console.log(msg);
}

