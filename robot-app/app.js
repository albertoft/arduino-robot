// Database
var Datastore = require('nedb'), db = new Datastore({ filename: '/tmp/robot.data' });
db.loadDatabase(function (err) {
  ;
});


// Bluetooth
var address = '98:D3:33:80:82:00';
var channel = 1;

var BTSP = require('bluetooth-serial-port');
var serial = new BTSP.BluetoothSerialPort();

serial.on('found', function(address, name) {
	serial.findSerialPortChannel(address, function(channel) {
		serial.connect(address, channel, function() {
			console.log('connected');
			var tmp = '';
			serial.on('data', function(buffer) {
				var chunk = buffer.toString('ascii');
				
				var i=0, piece='', offset=0;
				tmp += chunk;
				while ( (i=tmp.indexOf('\n', offset)) !== -1) {
					piece = tmp.substr(offset, i-offset);
					offset = i + 1;
					newPiece(piece);
				}
				tmp = tmp.substr(offset);
			});
		});
	});
});

function newPiece(msg) {
	var object = JSON.parse(msg);
	
	db.insert(object, function (err, newDoc) {
		;
	});
}

serial.inquire();


// REST api
var express = require('express');
var app = express();
app.use(express.static('www'));

app.get('/api', function (req, res) {
	db.find({}).sort({ time: 1 }).skip(1).limit(200).exec(function (err, docs) {
  		res.send(200, docs);
	});
})

var server = app.listen(8081, function () {
   var host = server.address().address
   var port = server.address().port
   console.log("Example app listening at http://%s:%s", host, port)
})
