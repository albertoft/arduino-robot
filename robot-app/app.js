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


/**
 * Database
 */
var Database = require('./database.js');
var filename = (process.argv.length > 2) ? process.argv[2]:"/tmp/robot.db";
var load = (process.argv.length > 3);
var db = new Database(filename, load);


/**
 * REST
 */
app.get('/api', function (req, res) {
	// return latest 800 documents in database ordered by time
	db.store.count({}, function (err, count) {
		var nlimit = 800;
		var nskip = ((count-nlimit) > 0) ? (count-nlimit):0;
		db.store.find({}).sort({time: 1}).skip(nskip).limit(nlimit).exec(function(err, docs) {
			res.status(200).send(docs)
		});
	});
});


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
bluetooth.serial.inquire();


/**
 * Server
 */
server.listen(8081, function () {
   console.log("[server] ready at http://raspberrypi.local:8081");
});
