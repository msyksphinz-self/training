// Loading modules
var xml2js = require('xml2js');
var parseString = xml2js.parseString;
var Builder = xml2js.Builder;

// Test XML Data
var xml = "<fruits shop='AAA'>" +
    "<item price='140'>Banana</item>" +
    "<item price='200'>Apple</item>" +
    "</fruits>";

// Parse XML
parseString(xml, function(err, r) {
        console.log(JSON.stringify(r));
        var xml = new Builder().buildObject(r);
        console.log(xml);
    });

