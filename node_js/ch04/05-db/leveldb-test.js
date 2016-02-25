// Example of LevelDB for Node.js

// Loading Module
var levelup = require('level');
var db = levelup('./testdb');

// Set values
db.put('Apple', 'red', function(err) {
        if (err) { console.log('Error', err); return; }
        testGet();
    });

function testGet () {
    db.get ('Apple', function (err, value) {
            if (err) { console.log('Error', err); return; }
            console.log('Apple =' + value);
            testBatch();
        });
}

function testBatch() {
    db.batch()
        .put('Mango', 'yellow')
        .put('Banana', 'yellow')
        .put('Kiwi', 'green')
        .write(function() { testGet2(); });
}


function testGet2 () {
    db.get('Banana', function(err, value) {
            console.log('Banana='+value);
            testKeys();
        });
}




