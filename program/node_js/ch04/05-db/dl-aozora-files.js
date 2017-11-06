//
// Download Aozora-Bunko top 30.
//
var URL_RANKING = "http://www.aozora.gr.jp/access_ranking/2014_xhtml.html";
var MAX_RANK    = 30;
var SAVE_DIR    = __dirname + "/aozora";

// Loading Modules
var client = require('cheerio-httpcli');
var fs     = require('fs');
var URL    = require('url');

// for Data Save
var cardlist = [];

// Make Directory for Save novels
if (!fs.existsSync(SAVE_DIR))
    fs.mkdirSync(SAVE_DIR);

// Download Ranking Page
client.fetch(URL_RANKING, function(err, $, res) {
        if (err) { console.log ("DL Error"); return; }
        // Get all line of Ranking Table
        var tr = $("table.list tr");
        if (!tr) {
            console.log("table.list tr");
        }
        // Repeat each table
        for (var i = 0; i < tr.length; i++) {
            // Check necessary nodes
            var cells = tr.eq(i).children();
            var rank  = parseInt(cells.eq(0).text());
            var link  = cells.eq(1);
            var href  = link.children('a').attr('href');
            var name  = link.text().replace(/(^\s+|\s+$)/, "");
            // console.log(rank, name ,href);
            if (isNaN(rank) || rank > MAX_RANK) continue;
            // Convert relatve path to absolute path
            href = URL.resolve(URL_RANKING, href);
            cardlist.push([rank, name, href]);
        }
        downloadNextFile();
    });


// Dowload each novels
function downloadNextFile() {
    if (cardlist.length == 0) {
        console.log("Operation COmplete");
        return;
    }
    // Delay operation
    setTimeout(function() {
            var card = cardlist.shift();
            downloadCard(card);
        }, 1000);
}


// Download Card
function downloadCard(card) {
    var index = card[0], name = card[1], link = card[2];
    console.log ("Book Card" + index + ":" + name);
    client.fetch(link, function(err, $, res) {
            if (err) { console.log("ERROR"); return; }
            // Get All Link
            var xhtml_link = "";
            $("a").each(function(idx) {
                    var text = $(this).text();
                    var href = $(this).attr('href');
                    if (text.indexOf("XHTML版で読む") >= 0) {
                        // Convert Relative path to absolute path
                        href = URL.resolve(link, href);
                        xhtml_link = href;
                        return false;
                    }
                });
            if (xhtml_link == "") {
                console.log("Cant find novel list");
            }
            // Download novels
            var path = SAVE_DIR + "/" + index + ".html";
            console.log("Start Download : " + name);
            client.setBrowser('chrome');
            client.fetch(xhtml_link, function (err, $, res, body) {
                    body = body.replace(/Shift_JIS/ig, "UTF-8");
                    fs.writeFileSync(path, body, "utf-8");
                    console.log("Complete : " + name);
                    downloadNextFile();
                });
        });
}

