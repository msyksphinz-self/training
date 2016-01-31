// Get favorite music for CasperJS

var BBS_USER = "JS-TESTER";
var BBS_PASS = "ipCU12ySxI";

var casper  = require('casper').create();
casper.start();

casper.open("http://uta.pw/sakusibbs/users.php?action=login");

casper.then(function(){
	this.fill("form", {
		username_mmlbbs6: BBS_USER,
		password_mmlbbs6: BBS_PASS
		    }, true);
    });

casper.then(function() {
	var getLink = function () {
	    var q = document.querySelector('#header_menu_linkbar a');
	    return q.href;
	};
	var mypage_url = this.evaluate(getLink);
	this.echo("mypage url= " + mypage_url);
	this.open(mypage_url);
    });


casper.run();
