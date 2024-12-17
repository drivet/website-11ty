PATH=$PATH:~/.nvm/versions/node/v22.12.0/bin
WEBMENTION_IO_TOKEN=rh7Wtv9MIwDKvpcrp1TdzA
npm install
npm run build
rsync -a _site /var/www/desmondrivet.com/
node ./send-webmentions.js
