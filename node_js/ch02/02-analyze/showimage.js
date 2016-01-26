// Loading modules
var client = require('cheerio-httpcli');
var URL = require('url');

// Download
var url = "http://en.wikipedia.org/wiki/Dog";
var param = {};
client.fetch(url, param, function(err, $, res) {
        if (err) { console.log("error : ", err); return; }
        // Extract Link and show
        $("img").each(function(idx) {
                var src = $(this).attr('src');
                src = URL.resolve(url, src);
                console.log(src);
            });
    });

