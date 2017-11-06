// ダウンロードしたファイルをSQLiteに流し込む for Node.js

// パスの指定など
var FILES_DIR = __dirname + "/aozora";
var DB_PATH = __dirname + "/aozora.sqlite";

// モジュールの取り込み
var sqlite3 = require('sqlite3').verbose();
var cheerio = require('cheerio');
var fs = require('fs');

// Get FileList for storing DB
var files = fs.readdirSync(FILES_DIR);
files = files.filter(function(s) {
        return s.match(/\.html$/);
    });

// Open DB
var db = new sqlite3.Database(DB_PATH);

// Register Data
db.serialize(function() {
        // Make Table
        db.run("CREATE TABLE IF NOT EXISTS items(" +
               "item_id INTEGER PRIMARY KEY, " +
               "author TEXT, title TEXT, body TEXT)");
        var ins_stmt = db.prepare('INSERT INTO items (author, title, body)' +
                                  'VALUES(?, ?, ?)');
        files.forEach(function(file, i, ar) {
                var html = fs.readFileSync(FILES_DIR + "/" + file);
                var $ = cheerio.load(html);
                var title = $(".title").text();
                var author = $(".author").text();
                var body = $('body').text();
                ins_stmt.run(author, title, body);
                console.log("+ " + title + "Registered");
            });
        ins_stmt.finalize();
    });

console.log ("Summary : ");
db.each ("SELECT author, COUNT(author) as cnt "
         + "FROM items GROUP BY author "
         + "ORDER BY cnt DESC",
         function(err, row) {
             console.log(row.cnt + "Times : " + row.author);
         });


