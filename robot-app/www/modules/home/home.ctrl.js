var module = angular.module('robotWeb.home');

module.controller('homeCtrl', ['$scope', '$log', '$http', '$interval', function ($scope, $log, $http, $interval) {
	$scope.series = { heading: [], course: [], obstacle: [] };

	$scope.refresh = function() {
		$http.get('http://raspberrypi.local:8081/api/').then(
			function(response) {
				// build series data
				angular.forEach(response.data, function(obj, key) {
					// use time in seconds in x-axis
					var time = obj.time/1000;

					$scope.series.heading.push({stateName: obj.state, deviation: obj.deviation, x: time, y: obj.heading});
					$scope.series.course.push({x: time, y: obj.course});
					$scope.series.obstacle.push({x: time, y: obj.obstacle});
				});
			
   				$('#robot-data').highcharts({
					title: { text: 'robot data' },
            		chart:{
                		type:'line',
						zoomType: 'xy'
            		},
           		 	plotOptions:{
                		line: { lineWidth:2 }
            		},
					tooltip: {
						shared: true,
						crosshairs: true,
						formatter: function () {
							var points = this.points;
            				var pointsLength = points.length;
							var html = "";
							
							if (pointsLength) {
                    			html += this.points[0].point.stateName + '<br/>';
							}
		
            				for (var index = 0; index < pointsLength; index ++) {
								var yValue = points[index].y;
								html += '<span style="color:' + points[index].series.color + '">\u25CF</span> ' + points[index].series.name + ': <b>' + yValue  + '</b><br/>';	
							}
							return html;
                		}
    				},
					xAxis: [
						{ title: { text: 'time (s)' }
						}
					],
					yAxis: [
						{	min: 0,
							max: 360, 
							tickInterval: 90,
							title: { text: 'degrees' }
        				},
						{   min: 0,
							max: 50,
							tickInterval: 10,
							title: {
								text: 'obstacle (cm)',
							},
							opposite: true
						}
					],
            		series: [ 
						{ name: 'Heading', data: $scope.series.heading, yAxis: 0}, 
						{ name: 'Course', data: $scope.series.course, yAxis: 0 },
            		 	{ name: 'Obstacle', data: $scope.series.obstacle, yAxis: 1 }
					]
        		});
	    	
			}
		);
	};
	
	$scope.refresh();
	
	/*
	$scope.Timer = $interval(function() {
		$scope.refresh();
	}, 3000);
	*/
}]);
