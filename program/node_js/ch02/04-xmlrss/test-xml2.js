// Loading module
var parseString = require('xml2js').parseString;

// Test XML Data
var xml = "<fruits shop='AAA'>" +
    "<item price='140'>Banana</item>" +
    "<item price='200'>Apple</item>" +
    "</fruits>";

// Parse XML
parseString(xml, function (err, result) {
        var shop = result.fruits.$.shop;
        console.log("shop=" + shop);
        // Show Fruits Name and Price
        var items = result.fruits.item;
        for (var i in items) {
            var item = items[i];
            console.log("-- name=" + item._);
            console.log("   price=" + item.$.price);
        }
    });


