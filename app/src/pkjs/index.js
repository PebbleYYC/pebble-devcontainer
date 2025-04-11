/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

const apikey = require('./apikey');
const keys = require('message_keys');
const corrections = require('./operator_corrections');
const { VehicleType, RouteShape, GColor, ErrorCode } = require("./data");

const MAX_WATCH_DATA = 12;
const SEARCH_RADIUS_M = 500;

function rgb_to_pebble_colour(hexstr) {
    // adapted from https://github.com/pebble-examples/cards-example/blob/master/tools/pebble_image_routines.py
    let r = parseInt(hexstr.substr(0, 2), 16);
    let g = parseInt(hexstr.substr(2, 2), 16);
    let b = parseInt(hexstr.substr(4, 2), 16);

    r = (r / 85) * 85;
    g = (g / 85) * 85;
    b = (b / 85) * 85;

    r >>= 6;
    g >>= 6;
    b >>= 6;
    argb8 = (0x3 << 6) | (r << 4) | (g << 2) | b;
    return argb8;
}

function transsee_dep_to_watch_data(stop, route, direction, prediction) {
    let watch_data = {};
    watch_data[keys.time] = parseInt(prediction.minutes);
    watch_data[keys.unit] = "min";
    watch_data[keys.stop_name] = stop.stop_name;
    watch_data[keys.dest_name] = direction;
    watch_data[keys.route_number] = route.routeTag;
    watch_data[keys.route_name] = route.routeTitle.replace(watch_data[keys.route_number] + "-", "");
    watch_data[keys.vehicle_type] = VehicleType.BUS;
    if (route.hasOwnProperty("color")) {
        watch_data[keys.color] = rgb_to_pebble_colour(route.color);
    } else {
        watch_data[keys.color] = GColor.GColorBlackARGB8;
    }
    watch_data[keys.shape] = RouteShape.ROUNDRECT;

    if (corrections.transsee.hasOwnProperty(route.agencyTitle)) {
        corrections.transsee[route.agencyTitle](stop, route, direction, prediction, watch_data);
    }

    return watch_data;
}

function compare_distance_to_here_stops(lat, lon) {
    return function(stop1, stop2) {
        const stop1_lat = stop1.stop_lat;
        const stop1_lon = stop1.stop_lon;
        const stop2_lat = stop2.stop_lat;
        const stop2_lon = stop2.stop_lon;
        const dist1 = Math.sqrt(
            Math.pow(stop1_lat - lat, 2) + Math.pow(stop1_lon - lon, 2)
        );
        const dist2 = Math.sqrt(
            Math.pow(stop2_lat - lat, 2) + Math.pow(stop2_lon - lon, 2)
        );
        if (dist1 > dist2) {
            return 1;
        } else if (dist1 < dist2) {
            return -1;
        } else {
            return 0;
        }
    }
}

function send_error(error) {
    Pebble.sendAppMessage({"num_routes": error}, function() {
        console.log('Error message sent successfully');
    }, function(e) {
        console.log('Error message failed: ' + JSON.stringify(e));
    });
}

function send_to_watch(departures_for_watch) {