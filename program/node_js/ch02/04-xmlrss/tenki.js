// Yahoo!Japan Tennki-Yohou RSS
var RSS="http://rss.weather.yahoo.co.jp/rss/days/4410.xml";

// Loading Module
var parseString = require('xml2js').parseString;
var request = require('request');

// Download RSS
request(RSS, function (err, responce, body) {
        if (!err && responce.statusCode == 200) {
            analyzeRSS(body);
        }
    });


// Analyze RSS
function analyzeRSS(xml) {
    // Convert XML to object
    parseString(xml, function(err, obj) {
            if (err) { console.log(err); return; }
            // Display Tenki
            // console.log(JSON.stringify(obj));
            var items = obj.rss.channel[0].item;
            for (var i in items) {
                var item = items[i];
                console.log(item.title[0]);
            }
        });
}

