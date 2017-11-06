var client = require('cheerio-httpcli');

var url = "http://www.aozora.gr.jp/index_pages/person81.html";
var param = {};

client.fetch(url, param, function(err, $, res) {
        if (err) { conosle.log("Error:", err); return; }
        var body = $.html();
        console.log(body);
    });

