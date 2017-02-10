/**
 * Database - manages Robot database using nedb (file)
 */


/** 
 * constructor
 * @param filename path to the file that will be used to store robot database, if no filename specified an in-memory version is used
 * @param load if true, filename will be loaded if existing (opens a previously created database file)
 */
function Database(filename, load) {
	this.filename = filename;
	var that = this;
	var Datastore = require('nedb');
	
	// no filename specified, use in-memory database
	if ((!filename) || (filename==='')) {
 		this.store = new Datastore();
		this.log("using in-memory db");
		return;
	}

	// rename database file if it already exists 
	if (!load) {
		var fs = require('fs');
		try {
			fs.accessSync(this.filename, fs.W_OK);
			fs.renameSync(this.filename, this.filename + ".old");
			this.log(this.filename + " to " + this.filename + ".old");
		} catch (ex) {
			;
		}
	}

	// initialize database
	this.store = new Datastore({ filename: this.filename });
	this.store.loadDatabase(function (err) { 
		if (err) {
			that.log(err);
		}
		that.log(that.filename + ' loaded');
	});
}

/**
 * returns a nedb instance
 */
Database.prototype.store = function() {
	return this.store;
};


/**
 * logs a database message to console
 */
Database.prototype.log = function(msg) {
	console.log('[database] ' + msg);
};

module.exports = Database;
