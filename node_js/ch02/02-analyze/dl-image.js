// Loading Modules
var client = require('cheerio-httpcli');
var request = require('request');
var fs = require('fs');
var URL = require('url');

// Make Download directory
var savedir = __dirname + "/img";
if (!fs.existsSync(savedir)) {
    fs.mkdirSync(savedir);
}

// Specify HTML file
var url = "http://en.wikipedia.org/wiki/Dog";
var param = {};
// Get HTML file
client.fetch(url, param, function(err, $, res) {
        if (err) { console.log ("error: ", err); return; }
        // Extract Link and show
        $("img").each(function(idx) {
                var src = $(this).attr('src');
                // change relative path to absolute path
                src = URL.resolve(url, src);
                // Make save filename
                var fname = URL.parse(src).pathname;
                console.log(fname);
                fname = savedir + "/" + fname.replace(/[^a-zA-Z0-9\.]+/g, '_');
                console.log(fname);
                request(src).pipe(fs.createWriteStream(fname));
            });
    });
