// Getting screenshot by CasperJS
// Make Casper Object
var casper = require('casper').create();

// Start
casper.start();

// Open Page
casper.open('http://google.com');

// Capture screenshot
casper.then(function() {
    casper.capture("screenshot.png");
});

// Run
casper.run();

