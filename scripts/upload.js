const fs = require('fs');
const axios = require('axios');
const path = require('path');
const FormData = require('form-data');
const { ArgumentParser } = require('argparse');

const parser = new ArgumentParser({
  description: 'Image uploader'
});

parser.add_argument('files', {help: 'image files', nargs: '*'});
parser.add_argument('server', {help: 'media server'});
parser.add_argument('token', {help: 'indieweb token for media server'});

const args = parser.parse_args();

const files = args.files;
const server = args.server;
const token = args.token;

async function processImageFile(imgPath) {
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

const imageUrls = [];

(async () => {
  for (let i = 0; i < files.length; i ++) {
    const absFile = path.resolve(files[i]);
    const url = await processImageFile(absFile);
    imageUrls.push(url);
  }
  imageUrls.forEach(u => console.log(u));
})();
