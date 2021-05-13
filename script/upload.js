// to ensure that env not in the CI server log

const path = require('path')
const { spawnSync } = require('child_process')
const { writeFileSync } = require('fs')

const newPrebuild = `
var path = require('path')
var github = require('github-from-package')
var ghreleases = require('ghreleases')
var error = require('./error')

function upload (opts, cb) {
  var pkg = opts.pkg
  var files = opts.files
  var gh = opts.gh || ghreleases
  var tagPrefix = opts['tag-prefix']

  var url = github(pkg)
  if (!url) {
    return process.nextTick(function () {
      cb(error.noRepository())
    })
  }

  var user = url.split('/')[3]
  var repo = url.split('/')[4]
  var auth = { user: 'x-oauth', token: opts.upload }
  var tag = \`\${tagPrefix}\${pkg.version}\`

  console.error("before create", tag)
  gh.create(auth, user, repo, { tag_name: tag }, function (e, r) {
    console.error("after create", tag, e, r)
    gh.getByTag(auth, user, repo, tag, function (err, release) {
      console.error("after getbytag", tag, err, release)
      if (err) return cb(err)

      var assets = release.assets.map(function (asset) {
        return asset.name
      })

      var filtered = files.filter(function (file) {
        return !assets.some(function (asset) {
          return asset === path.basename(file)
        })
      })

      console.error("before uploadAssets")
      gh.uploadAssets(auth, user, repo, 'tags/' + tag, filtered, function (err) {
        console.error("after uploadAssets")
        if (err) return cb(err)
        cb(null, { new: filtered, old: assets })
      })
    })
  })
}

module.exports = upload
`

writeFileSync(
  path.join(__dirname, '../node_modules/prebuild/upload.js'),
  newPrebuild
)

spawnSync(
  path.join(
    __dirname,
    '../node_modules/.bin/prebuild' +
      (process.platform === 'win32' ? '.cmd' : '')
  ),
  ['--upload-all', process.env.GITHUB_AUTH_TOKEN],
  { stdio: 'inherit' }
)
