// Get Kawase for Node.js

// Kawase API URL
var API = "http://api.aoikujira.com/kawase/get.php?code=USD&format=json";

// Loading Module
var request = require('request');
var fs = require('fs');

// Access Web API
request(API, function(err, responce, body) {
        // Check HTTP Error
        if (err || responce.statusCode != 200) {
            console.log("ERROR", err); return;
        }
        // Convert JSON into JS
        var r = JSON.parse(body);
        var jpy = r["JPY"];
        // Save Kawase rate into file
        var t = new Date();
        var fname = "USD_JPY_" +
            t.getFullYear() + "-" + (t.getMonth()+1) +
            "-" + t.getDay() + ".txt";
        var text = "1usd=" + jpy + "jpy";
        console.log(text);
        fs.writeFile(fname, text);
    });

