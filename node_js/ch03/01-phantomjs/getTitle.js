// Display Title from Web-Site

var TARGET_URL = "http://kujirahand.com";

// Make CasperJS object
var casper = require('casper').create();

// Open specified Web-Site
casper.start(TARGET_URL, function() {
        // Display Title
        this.echo(casper.getTitle());
    });

// Run
casper.run();

