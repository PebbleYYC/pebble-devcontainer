const webpack = require('webpack');
const path = require('path');
const os = require('os');
const fs = require('fs');
const { CleanWebpackPlugin } = require('clean-webpack-plugin');

// Function to resolve the Pebble SDK path
function sdkPath(p) {
  const baseSdkPath = '.pebble-sdk/';
  const resolvedPath = path.resolve(os.homedir(), baseSdkPath, 'SDKs', '4.3', 'sdk-core', p); // Updated to match the correct SDK path
  if (!fs.existsSync(resolvedPath)) {
    throw new Error(`Pebble SDK path does not exist: ${resolvedPath}`);
  }
  return path.relative(__dirname, resolvedPath);
}

module.exports = {
  mode: process.env.NODE_ENV || 'development', // Use 'development' mode by default
  devtool: 'source-map', // Generate source maps for easier debugging
  entry: [
    'core-js/stable', // Ensure compatibility with older environments
    'regenerator-runtime/runtime', // Enable async/await support
    'whatwg-fetch', // Polyfill for fetch API
    sdkPath('pebble/common/include/_pkjs_shared_additions.js'), // Pebble shared additions
    './src/pkjs/index.js', // Main JavaScript entry point
    './src/pkjs/apikey.js',
    './src/pkjs/data.js',
    './src/pkjs/operator_corrections.js',
    './src/pkjs/title_caps.js',
  ],
  output: {
    chunkFormat: 'commonjs', // Output as CommonJS modules
    filename: 'pebble-js-app.js', // Output file for the bundled JavaScript
    path: path.resolve(__dirname, './build'), // Output directory
    sourceMapFilename: 'pebble-js-app.js.map', // Source map file
    devtoolModuleFilenameTemplate: '[resource-path]', // Source map resource paths
    devtoolFallbackModuleFilenameTemplate: '[resource-path]?[hash]', // Fallback resource paths
  },
  target: 'es5', // Ensure compatibility with ES5 environments
  resolve: {
    roots: [sdkPath('pebble/common/include')], // Include Pebble SDK roots
    extensions: ['.js', '.json'], // Resolve .js and .json files
    alias: {
      'app_package.json': path.resolve(__dirname, './package.json'), // Alias for app package
      'message_keys': path.resolve(__dirname, './build/js/message_keys.json'), // Alias for message keys
    },
  },
  module: {
    rules: [
      {
        test: /\.js$/, // Process JavaScript files
        exclude: /node_modules/, // Exclude node_modules
        use: {
          loader: 'babel-loader', // Use Babel loader
          options: {
            presets: ['@babel/preset-env'], // Use Babel preset-env for transpiling
          },
        },
      },
    ],
  },
  plugins: [
    new CleanWebpackPlugin(), // Clean the build folder before each build
    new webpack.DefinePlugin({
      'process.env.NODE_ENV': JSON.stringify(process.env.NODE_ENV || 'development'), // Define environment variable
    }),
  ],
};