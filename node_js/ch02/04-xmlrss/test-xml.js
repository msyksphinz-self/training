// Loading module
var parseString = require('xml2js').parseString;

// Test XML Data
var xml = "<fruits shop='AAA'>" +
    "<item price='140'>Banana</item>" +
    "<item price='200'>Apple</item>" +
    "</fruits>";


// Parse XML
parseString(xml, function (err, result) {
        console.log(JSON.stringify(result));
    });

