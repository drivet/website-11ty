const fs = require('fs');
const axios = require('axios');
const path = require('path');
const FormData = require('form-data');
const yaml = require('js-yaml');
const { ArgumentParser } = require('argparse');

const parser = new ArgumentParser({
  description: 'Album post generator'
});

parser.add_argument('folder', {help: 'folder of images'});
parser.add_argument('server', {help: 'media server'});
// parser.add_argument('output file', {help: 'output file'});
parser.add_argument('token', {help: 'indieweb token for media server'});

parser.add_argument('-f', '--featured', {help: "featured image"});
parser.add_argument('-t', '--title', {help: "album title"});
parser.add_argument('-b', '--body', {help: "album body"});

const args = parser.parse_args();

const folder = args.folder;
const server = args.server;
const token = args.token;
const featured = args.featured;
const body = args.body;
const title = args.title;



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
  const imgPath = path.join(folder, filename); 
  const formData = new FormData();
  formData.append('file', fs.createReadStream(imgPath));

  try {
    const res = await axios.post(server, formData, {
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

async function processFeatured() {
  if (featured) {
    return await processImageFile(featured);
  } else {
    return undefined;
  }
}

const files = fs.readdirSync(folder);

const imageUrls = [];

(async () => {
  for (let i = 0; i < files.length; i ++) {
    const url = await processImageFile(files[i]);
    imageUrls.push(url);
  }

  const furl = await processFeatured();

  const data = {
    date: toIsoString(new Date()),
    title,
    featured: furl ? { alt: '', value: furl }: undefined,
    album: true,
    photo: imageUrls.map(u => ({alt: '', value: u}))
  };

  process.stdout.write('---\n');
  process.stdout.write(yaml.dump(data));
  process.stdout.write('---\n\n');
  if (body) {
    process.stdout.write(body);
    process.stdout.write('\n');
  }

  //imageUrls.forEach(u => console.log(u));
})();
