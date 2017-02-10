var module = angular.module('robotWeb.cmd');

module.controller('cmdCtrl', ['$scope', '$log', '$http', '$interval', function ($scope, $log, $http, $interval) {

	$scope.sendingPromise = "";

	/**
     * Keep sending a command to robot
	 */
	$scope.sendInterval = function(cmd) {
		$interval.cancel($scope.sendingPromise);
		
		$scope.sendingPromise = $interval(function() {
			$scope.sendCommand(cmd);
		}, 1000);
	};
	
	/*
	 * Send a command only once to robot api
     */
	$scope.sendOnce = function(cmd) {
		$interval.cancel($scope.sendingPromise);
		$scope.sendCommand(cmd);
	};

	/**
	 * Send command to robot api
	 */
	$scope.sendCommand = function(cmd) {
		$http.post('http://raspberrypi.local:8081/api/', {"cmd": cmd});
	};

}]);
