var PDFDocument = require('pdfkit');
var fs = require('fs');

var doc = new PDFDocument();

doc.pipe(fs.createWriteStream('output.pdf'));

doc.fontSize(30)
    .text('Hello World', 90, 100);
doc.fontSize(20)
    .text("求めつづけなさい。そうすれば与えられます。\n\n" +
          "探しつづけなさい。そうすれば見いだせます。\n\n" +
          "たたきつづけなさい。そうすれば開かれます。",
          100, 180);
doc.save()
    .moveTo(80, 80)
    .lineTo(250, 80)
    .lineTo(250, 150)
    .lineTo(80, 150)
    .lineTo(80, 80)
    .stroke('#0000FF');

doc.addPage();

doc.save()
    .moveTo(100, 150)
    .lineTo(100, 250)
    .lineTo(200, 250)
    .fill('#FF0000');

doc.end();


