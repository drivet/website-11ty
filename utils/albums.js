function albumImageUrl(albumPath, index) {
  const indexSlug = `${index}`.padStart(6, '0');
  return `${albumPath}/${indexSlug}`;
}

module.exports = {
  albumImageUrl
}
