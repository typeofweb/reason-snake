const path = require("path");

module.exports = {
  entry: "./lib/es6/src/Index.bs.js",
  mode: "development",
  output: {
    path: path.join(__dirname, "web"),
    filename: "index.js"
  },
  devServer: {
    hot: true,
    injectHot: true,
    contentBase: path.join(__dirname, "web")
  },
  watchOptions: {
    ignored: ["src/**/*.*", "node_modules"]
  }
};
