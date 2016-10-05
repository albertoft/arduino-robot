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
			console.log('[bluetooth] connected!');

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
	console.log("[bluetooth] line=" + line);
		
	// robot will send JSON strings, parse to object.
	var object = JSON.parse(line);

	formatRobotData(object, function(data) {
		// insert state change object in database
		db.stateChange.insert(data, function (err, newDoc) {
			;
		});
	});

	
}

function formatRobotData(data, callback) {
	var stateName = {'F': 'Forward', 'X': 'Steer', 'W': 'FindWayOut', 'S': 'Stop', 'E': 'Error'}
	data.state = stateName[data.state];
	data.course = (data.course === -1) ? null:data.course;
	callback(data);
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
   var port = server.address().port
   console.log("[app server] ready at http://raspberrypi.local:" + port);
})
