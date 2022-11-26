function albumImageUrl(albumPath, index) {
  console.log(`DDDDD ${albumPath}, ${index}`);
  const indexSlug = `${index}`.padStart(6, '0');
  return `${albumPath}/${indexSlug}`;
}

module.exports = {
  albumImageUrl
}
