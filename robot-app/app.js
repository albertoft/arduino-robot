/*
 * Database
 */
var db = {};
db.filename = "/tmp/robot-stateChange.data";

// move old database file (if any)
var fs = require('fs');
try {
	fs.accessSync(db.filename, fs.W_OK);
	fs.renameSync(db.filename, db.filename + ".old");
	console.log("renamed previous " + db.filename + " to " + db.filename + ".old");
} catch (ex) {
	;
}

// initialize new database
var Datastore = require('nedb');
db.stateChange = new Datastore({ filename: db.filename, autoload: true});


/*
 * Bluetoooth
 */
var address = '98:D3:33:80:82:00';
var channel = 1;
var BTSP = require('bluetooth-serial-port');
var serial = new BTSP.BluetoothSerialPort();

// configure bluetooth connection
serial.on('found', function(address, name) {
	serial.findSerialPortChannel(address, function(channel) {
		
		// connection
		serial.connect(address, channel, function() {
			console.log('connected');

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
					processBluetoothLine(line);
				}
				tmp = tmp.substr(offset);
			});
		});
	});
});

// Process a line of bluetooth received data
function processBluetoothLine(line) {
	//console.log(line);
	
	// robot will send JSON strings, parse to object.
	var object = JSON.parse(line);
	
	// insert state change object in database
	db.stateChange.insert(object, function (err, newDoc) {
		;
	});
}

// launch bluetooth connection
serial.inquire();


/*
 * REST APi
 */
var express = require('express');
var app = express();

// static content
app.use(express.static('www'));

// GET /api: returns latest robot state change events
app.get('/api', function (req, res) {
	
	// events ordered by time 
	db.stateChange.find({}).sort({ time: 1 }).skip(1).limit(800).exec(function (err, docs) {
		res.status(200).send(docs)
	});
})

// launch server
var server = app.listen(8081, function () {
   var host = server.address().address
   var port = server.address().port
   console.log("Example app listening at http://%s:%s", host, port)
})
