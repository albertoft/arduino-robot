// application modules
angular.module('robotWeb.home', []);

// application
var app = angular.module('robotWeb', ['robotWeb.home', 'ngRoute']);

app.config(['$routeProvider'
    , function ($routeProvider) {
        $routeProvider.
        when('/', {
                templateUrl: 'modules/home/homeView.html'
                , controller: 'homeCtrl'
            })
            .otherwise({
                redirectTo: '/'
            });
    }]);
