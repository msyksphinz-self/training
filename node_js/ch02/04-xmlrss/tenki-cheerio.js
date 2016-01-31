// Yahoo!Japan Tenki-Yohou (cheerio version) for Node.js

var RSS="http://rss.weather.yahoo.co.jp/rss/days/4410.xml";

var client = require('cheerio-httpcli');

// Download RSS
client.fetch (RSS, {}, function(err, $, res) {
        if (err) { console.log("error"); return; }
        $("item > title").each(function(idx) {
                var title = $(this).text();
                console.log(title);
            });
    });

