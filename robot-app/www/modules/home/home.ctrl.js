var module = angular.module('robotWeb.home');

module.controller('homeCtrl', ['$scope', '$log', '$http', '$interval', function ($scope, $log, $http, $interval) {
	$scope.series = { heading: [], course: [], obstacle: [] };

	$scope.refresh = function() {
		$http.get('http://raspberrypi.local:8081/api/').then(
			function(response) {

				var stateName = {'F': 'Forward', 'X': 'Steer', 'S': 'Stop', 'W': 'FindWayOut', 'I': 'Init'};
				// build series data
				angular.forEach(response.data, function(obj, key) {
					// use time in seconds in x-axis
					var time = parseFloat(parseInt(obj.time)/1000);

					$scope.series.heading.push({name: stateName[obj.state], deviation: obj.deviation, x: time, y: parseFloat(obj.heading)});
					$scope.series.course.push({name: obj.state, deviation: obj.deviation, x: time, y: parseFloat(obj.course)});
					$scope.series.obstacle.push({name: obj.state, x:time, y: parseFloat(obj.obstacle)});
				});
			
				// heading vs course graph
				// TODO: chart title
				// TODO: proper shared tooltip
   				$('#course').highcharts({
            		chart:{
                		type:'line',
						zoomType: 'x'
            		},
           		 	plotOptions:{
                		line: { lineWidth:2 }
            		},
					tooltip: {
						shared: true,
						crosshairs: true,
						formatter: function () {
                			var s = 'Heading=' + this.points[0].y + ' º'; 
                			s += '<br/>Course=' + this.points[1].y + ' º'; 
                    		s += '<br/>State = ' + this.points[0].point.name;
                    		s += '<br/>deviation = ' + this.points[0].point.deviation;
							return s;
                		}
    				},
					yAxis: [
						{ 
							title: {
                				text: 'degrees',
            				}
        				}
					],
            		series: [ 
						{ name: 'Heading', data: $scope.series.heading, yAxis: 0}, 
						{ name: 'Course', data: $scope.series.course, yAxis: 0 },
					]
        		});
				
				// obstacle detected graph
				// TODO: chart title
   				$('#obstacle').highcharts({
            		chart:{
                		type:'scatter',
						zoomType: 'x'
            		},
           		 	plotOptions:{
                		scatter: {lineWidth:2 }
            		},
					tooltip: {
        				pointFormat: "State:{point.name}"
    				},
            		series: [ {name: 'Obstacle', data: $scope.series.obstacle}]
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
