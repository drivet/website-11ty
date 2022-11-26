const fs = require('fs');
const axios = require('axios');
const path = require('path');
const FormData = require('form-data');
const yaml = require('js-yaml');

if (process.argv.length < 5) {
  console.error('Expecting 3 arguments');
  process.exit(1);
}

const imgFolder = process.argv[2];
const mediaServer = process.argv[3];
const token = process.argv[4];
const body = process.argv[5];

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

async function processImageFile(filename) {
  const imgPath = path.join(imgFolder, filename); 
  const formData = new FormData();
  formData.append('file', fs.createReadStream(imgPath));

  try {
    const res = await axios.post(mediaServer, formData, {
      headers: {
        ...formData.getHeaders(),
        'Authorization': `Bearer ${token}`
      }
    });
    const location = res.headers['location'];
    return location;
  } catch(e) {
    console.error('ERROR: ' + e);
  }
}

const files = fs.readdirSync(imgFolder);

const imageUrls = [];

(async () => {
  for (let i = 0; i < files.length; i ++) {
    const url = await processImageFile(files[i]);
    imageUrls.push(url);
  }

  const data = {
    date: toIsoString(new Date()),
    photo: imageUrls.map(u => ({alt: '', value: u}))
  };

  process.stdout.write('---\n');
  process.stdout.write(yaml.dump(data));
  process.stdout.write('---\n\n');
  if (body) {
    console.log(body);
  }

  //imageUrls.forEach(u => console.log(u));
})();
