const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CompressionPlugin = require('compression-webpack-plugin');
const WebpackPwaManifest = require("webpack-pwa-manifest");
const SWPrecacheWebpackPlugin = require("sw-precache-webpack-plugin");
const outputDir = path.join(__dirname, 'build');

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
  entry: './src/Index.bs.js',
  mode: isProd ? 'production' : 'development',
  output: {
    path: outputDir,
    filename: 'Index.js'
  },
  plugins: [
    new CompressionPlugin(),
    new HtmlWebpackPlugin({
      template: 'src/index.html',
      inject: false
    }),
    new WebpackPwaManifest({
      name: "Reason React Spotify Player",
      short_name: "TMDb React",
      description: "Client for Spotify web api built with ReasonML.",
      display: "standalone",
      start_url: "./",
      scope: ".",
      background_color: "#000000",
      theme_color: "#B03D32",
      fingerprints: false,
      "index.html": "/index.html",
      "index.js": "/index.js",
      icons: [
        {
          src: path.resolve("src/assets/icon.png"),
          sizes: [192, 512],
        }
      ]
    }),
    new SWPrecacheWebpackPlugin({
      cacheId: "reason-react-spotify-player",
      dontCacheBustUrlsMatching: /\.\w{8}\./,
      filename: "service-worker.js",
      // minify: true,
      navigateFallback: "/",
      staticFileGlobsIgnorePatterns: [/\.map$/, /asset-manifest\.json$/]
    }),
  ],
  devServer: {
    compress: true,
    contentBase: outputDir,
    port: process.env.PORT || 8000,
    historyApiFallback: true
  }
};
