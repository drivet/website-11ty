const folders = [
    './_cache/webmentions',
    './_cache/previews',
    './_cache/avatars',
    './_cache/scraped',
    './_cache/images'
];
module.exports = {
    // Before the build runs,
    // restore a directory we cached in a previous build.
    // Does not do anything if:
    //  - the directory already exists locally
    //  - the directory has never been cached
    async onPreBuild({ utils }) {
        await utils.cache.restore(folders);
    },
    // After the build is done,
    // cache directory for future builds.
    // Does not do anything if:
    //  - the directory does not exist
    async onPostBuild({ utils }) {
        await utils.cache.save(folders);
    }
}
