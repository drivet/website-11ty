const glob = require('glob');
const fs = require('fs');
const matter = require('gray-matter');

function toIsoString(date) {
  var tzo = -date.getTimezoneOffset(),
      dif = tzo >= 0 ? '+' : '-',
      pad = function(num) {
          return (num < 10 ? '0' : '') + num;
      };

  return date.getFullYear() +
      '-' + pad(date.getMonth() + 1) +
      '-' + pad(date.getDate()) +
      'T' + pad(date.getHours()) +
      ':' + pad(date.getMinutes()) +
      ':' + pad(date.getSeconds()) +
      dif + pad(Math.floor(Math.abs(tzo) / 60)) +
      ':' + pad(Math.abs(tzo) % 60);
}

function extractField(block, field) {
  
  const match = yamlString.match(/date:\s*(.+)/);
}

const readFiles = function (forFile) {
  forFile = forFile || function (file, content) {
      console.log(file);
  };

  glob('./src/posts/feed/**/*.md', function (err, files) {
    if (err) {
      console.log(err);
    } else {
      files.forEach(function (file) {
        fs.readFile(file, function (err, data) {
          if (err) {
            console.log(err);
          } else {
            forFile(file, data.toString());
          }
        });
      });
    }
  });
};

function processFile(file, content) {
  p = matter(content);    
    
  if (p.data['mp-syndicate-to'] && p.data['mp-syndicate-to'].findIndex(s => s.indexOf('twitter') != -1) != -1) {
    p.data['mp-syndicate-to'] = p.data['mp-syndicate-to'].filter(s => s.indexOf('twitter') === -1);
    if (p.data['mp-syndicate-to'].length === 0) {
      delete p.data['mp-syndicate-to'];
    }

    if (p.data['date'] instanceof Date) {
      const match = p.matter.match(/date:\s*(.+)/);
      p.data['date'] = match[1];
    }
    if (p.data['modified'] instanceof Date) {
      const match = p.matter.match(/modified:\s*(.+)/);
      p.data['modified'] = match[1];
    }

    fs.writeFile(file, matter.stringify(p.content, p.data), (err) => {
      if (err) {
        console.error(err);
      }
    });
    
  }
    
}

readFiles(processFile);
