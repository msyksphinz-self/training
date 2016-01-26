// Analyze Link and Download all
// -- Load modules --
var client = require('cheerio-httpcli');
var request = require('request');
var URL = require('url');
var fs = require('fs');
var path = require('path');

// -- common settings --
// set level
var LINK_LEVEL = 3;
// base URL
var TARGET_URL = "http://nodejs.jp/nodejs.org_ja/docs/v0.10/api/";
var list = {};

// main function
downloadRec(TARGET_URL, 0);

// download specified URL with maximum level LEVEL
function downloadRec (url, level) {
    // check maximum level
    if (level >= LINK_LEVEL) return;
    // ignore already found site
    if (list[url]) return;
    list[url] = true;
    // ignore outside of base URL
    var us = TARGET_URL.split("/");
    us.pop();
    var base = us.join("/");
    if (url.indexOf(base) < 0) return;
    // Get HTML
    client.fetch(url, {}, function(err, $, res) {
            // Get Linked page
            $("a").each(function(idx) {
                    // Get <a> link direction
                    var href = $(this).attr('href');
                    if (!href) return;
                    // change relative path into absolute path
                    href = URL.resolve(url, href);
                    // ignore following #
                    href = href.replace(/\#.+$/, "");
                    downloadRec(href, level + 1);
                });
            // Save page
            if (url.substr(url.length-1, 1) == '/') {
                url+= "index.html";
            }
            var savepath = url.split("/").slice(2).join("/");
            checkSaveDir(savepath);
            console.log(savepath);
            fs.writeFileSync(savepath, $.html());
        });
}

function checkSaveDir (fname) {
    // extract directory name
    var dir = path.dirname(fname);
    // make directory recursively
    var dirlist = dir.split("/");
    var p = "";
    for (var i in dirlist) {
        p += dirlist[i] + "/";
        if (!fs.existsSync(p)) {
            fs.mkdirSync(p);
        }
    }
}

