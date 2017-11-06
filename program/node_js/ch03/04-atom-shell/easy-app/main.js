var TARGET_URL = "https://ja.wikipedia.org";

var app = require('app');
var BrowserWindow = require('browser-window');

var mainWindow = null;
app.on('ready', function() {
	mainWindow = new BrowserWindow({width:800, height:600});
	mainWindow.loadUrl(TARGET_URL);
    });

