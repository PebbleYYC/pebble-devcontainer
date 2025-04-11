const webpack = require('webpack');
const path = require('path');
const os = require('os');
const fs = require('fs');
const { CleanWebpackPlugin } = require('clean-webpack-plugin');

function sdkPath(p) {
  const baseSdkPath = os.type() === 'Darwin' ? 'Library/Application Support/Pebble SDK/' : '.pebble-sdk/';
  const resolvedPath = path.resolve(os.homedir(), baseSdkPath, 'SDKs/current/sdk-core', p);
  if (!fs.existsSync(resolvedPath)) {
    throw new Error(`Pebble SDK path does not exist: ${resolvedPath}`);
  }
  return path.relative(__dirname, resolvedPath);
}

module.exports = {
  mode: process.env.NODE_ENV || 'development',
  devtool: 'source-map',
  entry: [
    'core-js/stable',
    'regenerator-runtime/runtime',
    'whatwg-fetch',
    sdkPath('pebble/common/include/_pkjs_shared_additions.js'),
    './src/pkjs/index.js',
    './src/pkjs/apikey.js',
    './src/pkjs/data.js',
    './src/pkjs/operator_corrections.js',
    './src/pkjs/title_caps.js',
  ],
  output: {
    chunkFormat: 'commonjs',
    filename: 'pebble-js-app.js',
    path: path.resolve(__dirname, './build'),
    sourceMapFilename: 'pebble-js-app.js.map',
    devtoolModuleFilenameTemplate: '[resource-path]',
    devtoolFallbackModuleFilenameTemplate: '[resource-path]?[hash]',
  },
  target: 'es5',
  resolve: {
    roots: [sdkPath('pebble/common/include')],
    extensions: ['.js', '.json'],
    alias: {
      'app_package.json': path.resolve(__dirname, './package.json'),
      'message_keys': path.resolve(__dirname, './build/js/message_keys.json'),
    },
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['@babel/preset-env'],
          },
        },
      },
    ],
  },
  plugins: [
    new CleanWebpackPlugin(),
    new webpack.DefinePlugin({
      'process.env.NODE_ENV': JSON.stringify(process.env.NODE_ENV || 'development'),
    }),
  ],
};