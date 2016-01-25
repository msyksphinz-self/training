download("http://www.aozora.gr.jp/index_pages/person81.html",
         "miyazawakenji.html",
         function() { console.log("ok, kenji."); });
download("http://www.aozora.gr.jp/index_pages/person148.html",
         "natumesoseki.html",
         function() { console.log("ok, soseki."); });

function download(url, savepath, callback) {
    var http = require('http');
    var fs = require('fs');
    var outfile = fs.createWriteStream(savepath);
    var req = http.get(url, function(res) {
            res.pipe(outfile);
            res.on('end', function() {
                    outfile.close();
                    callback();
                });
        });
}


