var BBS_USER = "JS-TESTER";
var BBS_PASS = "ipCU12ySxI";

var DEFAULT_USER_ID = 1;
var target_user_id  = DEFAULT_USER_ID;

var casper = require('casper').create();
var utils  = require('utils');

var args = casper.cli.args;
if (args.length >= 1) {
    target_user_id = parseInt(args[0]);
    if (target_user_id == 0) target_user_id = DEFAULT_USER_ID;
}
casper.echo("target_user_id = " + target_user_id);
casper.start();

casper.open("http://uta.pw/sakusibbs/users.php?action=login");
casper.then(function(){
	this.fill("form", {
		username_mmlbbs6: BBS_USER,
		password_mmlbbs6: BBS_PASS
		    }, true);
    });

casper.thenOpen("http://uta.pw/sakusibbs/users.php?user_id=" + target_user_id);

casper.then(function() {
	var getList = function () {
	    var links = [];
	    var list = document.querySelectorAll("ul#mmlist a");
	    for (var i = 0; i < list.length; i++) {
		var a = list[i];
		if (a.href.indexOf('post.php') > 0) {
		    links.push(a.href);
		}
	    }
	    return links;
	};
	
	var links = this.evaluate(getList);
	utils.dump(links);
	
	casper.each(links, function (self, link) {
		self.thenOpen (link, function() {
			if (this.exists('#fav_add_btn')) {
			    this.mouseEvent('click', '#fav_add_btn');
			    this.echo('- click:' + link);
			} else {
			    this.echo('- already:' + link);
			}
		    });
	    });
    });

casper.then(function() {
	this.echo('ok');
    });

casper.run();
