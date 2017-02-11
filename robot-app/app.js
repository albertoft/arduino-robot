/**
 * Robot Server App
 *  - Connects to robot using Bluetooth
 *  - Receives data from robot and stores it in a database
 *  - Send live data from robot to webclients using websockets
 */
var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);
app.use(express.static('www'));

var bodyParser = require('body-parser');
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies


/**
 * Database
 */
var Database = require('./database.js');
var filename = (process.argv.length > 2) ? process.argv[2]:'';
var load = (process.argv.length > 3);
var db = new Database(filename, load);


/**
 * Websockets
 */
io.on('connection', function(socket) {  
    console.log('[websocket] new connection');
});


/**
 * Bluetooth
 */
var Bluetooth = require('./bluetooth.js');
var bluetooth = new Bluetooth('98:D3:33:80:82:00', 1, function(line) {
	// robot will send JSON strings, parse into an object
	var data = JSON.parse(line);
	
	// format received data 
	var stateName = {'F':'Forward', 'X':'Steer', 'W':'FindWayOut', 'S':'Stop', 'E':'Error'}
	data.state = stateName[data.state];

	// insert dato into database
	db.store.insert(data, function (err, newDoc) {
		;
	});

	// send data to websocket clients 
	io.emit('messages', data);
});
bluetooth.log("trying to connect...");
bluetooth.serial.inquire();


/**
 * REST
 */
app.get('/api', function (req, res) {
	// return latest 800 documents in database ordered by time
	db.store.count({}, function (err, count) {
		var nlimit = 800;
		var nskip = ((count-nlimit) > 0) ? (count-nlimit):0;
		db.store.find({}).sort({time: 1}).skip(nskip).limit(nlimit).exec(function(err, docs) {
			res.status(200).send(docs);
		});
	});
});

app.post('/api', function (req, res) {
	console.log('[api] command=' + req.body.cmd)
	
	// send command to robot
	bluetooth.serial.write(new Buffer(req.body.cmd, 'utf-8'), function(err, bytesWritten) {
		if (err) console.log(err);
	});

	res.status(200).send();
});


/**
 * Server
 */
server.listen(8081, function () {
   console.log("[server] ready at http://raspberrypi.local:8081");
});
