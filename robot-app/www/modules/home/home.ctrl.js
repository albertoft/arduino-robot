var module = angular.module('robotWeb.home');

module.controller('homeCtrl', ['$scope', '$log', '$http', '$interval', function ($scope, $log, $http, $interval) {
    // charts configuration and highcharts series object reference  
	$scope.chart = { 
		data: { config: null, series: null },
		position: { config: null, series: null }
	};


	/**
     * Chart configuration. Robot-data
     */  
	$scope.chart.data.config = {
		title: { text: 'robot data' },
		chart:{
			type:'line',
			zoomType: 'xy',
			events: {
				load: function(event) {
					$scope.chart.data.series = this.series;
				}
			}
		},
		plotOptions:{ line: { lineWidth:2 } },
		tooltip: {
			shared: true,
			crosshairs: true,
			formatter: function () {
				var points = this.points;
				var pointsLength = points.length;
				var html = "";
					
				if (pointsLength) {
					html += this.points[0].point.stateName + '<br/>';
					html += this.points[0].point.totalDistance + '<br/>';
				}

				for (var index = 0; index < pointsLength; index ++) {
					var yValue = points[index].y;
					html += '<span style="color:' + points[index].series.color + '">\u25CF</span> ' + points[index].series.name + ': <b>' + yValue  + '</b><br/>';	
				}
				return html;
			}
		},
		xAxis: [ { title: { text: 'time (s)' } } ],
		yAxis: [ 
			{ min: 0, max: 360, tickInterval: 90, title: { text: 'degrees' } },
			{ min: 0, max: 50,  tickInterval: 10, title: { text: 'obstacle (cm)' }, opposite: true }
		],
		series: [ 
			{ name: 'Heading', data: [], yAxis: 0}, 
			{ name: 'Course', data: [], yAxis: 0 },
			{ name: 'Obstacle', data: [], yAxis: 1 }
		]
	};


	/**
     * Chart configuration. Robot-position
     */  
	$scope.chart.position.config = {
		title: { text: 'robot position' },
		chart:{
        	type:'scatter',
			zoomType: 'xy',
			events: {
				load: function(event) {
					$scope.chart.position.series = this.series;
				}
			}
        },
        plotOptions: { line: { lineWidth:2 } } ,
		xAxis: [{ min: -600, max: 600, title: {text: 'cm'} }],
		yAxis: [{ min: -600, max: 600, title: {text: 'cm'}, reversed: true }],
        series: [ 
        	{ name: 'position', data: [] },
        	{ name: 'obstacle', data: [] }
		]
	};


	/**
	 * Websocket. Process live robot data sent from server using websockets
     * Updates charts using received live data.
	 */
	$scope.connect = function() {
		var socket = io.connect('http://raspberrypi.local:8081', { 'forceNew': true } );

		socket.on('messages', function(obj) {  
			var data = $scope.processData(obj);
			
			$scope.chart.data.series[0].addPoint(data.heading, true, true);
			$scope.chart.data.series[1].addPoint(data.course, true, true);
			$scope.chart.data.series[2].addPoint(data.obstacle, true, true);

			$scope.chart.position.series[0].addPoint(data.position, true, true);
			$scope.chart.position.series[1].addPoint(data.obspos, true, true);
		});
	};


	/**
	 * Process an single data object received from server.
	 * @obj robot data object received from server  
     * @return object containg heading, course, obstacle and position points generated from data object received
     */
	$scope.processData = function(obj) {
		var result = { heading: null, course: null, obstacle: null, position: null, obspos: null };
		
		// use time in seconds in x-axis
		var time = obj.time/1000;

		result.heading = {stateName: obj.state, deviation: obj.deviation, x: time, y: obj.heading, totalDistance: obj.totalDistance};
		result.course = {x: time, y: obj.course};
		result.obstacle = {x: time, y: obj.obstacle };
		result.position = {x:obj.xpos, y: obj.ypos};
		result.obspos = {x:obj.obsxpos, y:obj.obsypos};

		return result;
	}; 


	/**
	 * Init. Retrieves stored robot data from server and creates charts. 
	 */
	$scope.init = function() {
		$http.get('http://raspberrypi.local:8081/api/').then( function(response) {
			// build series data
			angular.forEach(response.data, function(obj, key) {
				var data = $scope.processData(obj);

				$scope.chart.data.config.series[0].data.push(data.heading);
				$scope.chart.data.config.series[1].data.push(data.course);
				$scope.chart.data.config.series[2].data.push(data.obstacle);

				$scope.chart.position.config.series[0].data.push(data.position);
				$scope.chart.position.config.series[1].data.push(data.obspos);
			});
			
   			$('#robot-data').highcharts($scope.chart.data.config);
   			$('#robot-position').highcharts($scope.chart.position.config);
		}); // http.get
	};
	
	$scope.init();
	$scope.connect();

}]);
