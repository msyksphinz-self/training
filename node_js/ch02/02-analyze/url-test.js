var URL = require('url');

var base = "http://kujirahand.com/url/test/index.html";
var u1 = URL.resolve(base, 'a.html');
console.log("u1 = " + u1);
var u2 = URL.resolve(base, '../b.html');
console.log("u2 = " + u2);
var u3 = URL.resolve(base, '/c.html');
console.log("u3 = " + u3);
