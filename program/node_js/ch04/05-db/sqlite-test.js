// Loading Module

var sqlite3 = require('sqlite3').verbose();
// Open Local DB
var db = new sqlite3.Database('test.sqlite');

db.serialize(function() {
        // Create Table with SQL
        db.run('CREATE TABLE IF NOT EXISTS items(name, value)');

        // Insert Data using Prepared Statement
        var stmt = db.prepare('INSERT INTO items VALUES(?,?)');
        stmt.run(["Banana", 300]);
        stmt.run(["Apple", 150]);
        stmt.run(["Mango", 250]);
        stmt.finalize();

        // Extract Data
        db.each("SELECT * FROM items", function (eer, row) {
                console.log(row.name + ":" + row.value);
            });
    });

db.close();

