// application modules
angular.module('robotWeb.home', []);
angular.module('robotWeb.cmd', []);

// application
var app = angular.module('robotWeb', ['robotWeb.home', 'robotWeb.cmd', 'ngRoute']);

app.config(['$routeProvider',
    function ($routeProvider) {
        $routeProvider.
        when('/', {
                templateUrl: 'modules/home/homeView.html', 
				controller: 'homeCtrl'
            })
			.when("/command", {
				templateUrl : 'modules/cmd/cmdView.html',
				controller: 'cmdCtrl'
			})
            .otherwise({
                redirectTo: '/'
            });
    }]);
