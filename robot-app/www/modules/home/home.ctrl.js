var module = angular.module('robotWeb.home');

module.controller('homeCtrl', ['$scope', '$log', '$http', '$interval', function ($scope, $log, $http, $interval) {
	$scope.series = { heading: [], obstacle: [] };

	$scope.refresh = function() {
		$http.get('http://192.168.1.112:8081/api/').then(
			function(response) {
				angular.forEach(response.data, function(obj, key) {
					$scope.series.heading.push([parseFloat(obj.time/1000), parseFloat(obj.heading)]);
					$scope.series.obstacle.push([parseFloat(obj.time/1000), parseFloat(obj.obstacle)]);
				});
			
   				$('#demo').highcharts({
            		chart:{
                		type:'scatter',
						zoomType: 'x'
            		},
					/*
           		 	plotOptions:{
                		scatter:{
                    		lineWidth:2
                		}
            		},
					*/
            		series: [{
                		name: 'Heading',
                		data: $scope.series.heading
            		},
					{
						name: 'Obstacle',
						data: $scope.series.obstacle
					}],
				
					/*	
					responsive: {
  						rules: [{
    						condition: {
      							key: 'this.chartWidth',
      							operator: '<',
      							value: 500,
      							// callback: function () { return someThing(); }
    						},
    						chartOptions: {
      							chart: {
        							className: 'small-chart'
      							},
      							legend: {
        							align: 'center',
        							verticalAlign: 'bottom',
        							layout: 'horizontal'
      							}
    						}
  						}]
					}
					*/					
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
