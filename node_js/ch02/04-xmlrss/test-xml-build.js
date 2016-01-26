// Loading Module
var xml2js = require('xml2js');

// Convert object
var obj = {
    item: {name:"Banana", price:150}
};
// Convert XML
var builder = new xml2js.Builder();
var xml = builder.buildObject(obj);
console.log(xml);

