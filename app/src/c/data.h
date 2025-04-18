/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <pebble.h>

typedef enum {
    ScrollDirectionDown,
    ScrollDirectionUp,
} ScrollDirection;

typedef enum {
    STREETCAR = 0, // Represents Calgary's LRT
    BUS = 1,       // Represents Calgary's Bus
    // Removed unused types (SUBWAY, REGIONAL_TRAIN)
} VehicleType;

typedef enum {
    ROUNDRECT = 0,
    RECT = 1,
    CIRCLE = 2,
} RouteShape;

typedef enum {
    LOADING = 0,
    NO_CONNECTION = -1,
    INVALID_API_KEY = -2,
    NO_RESULTS = -3,
    UNKNOWN_API_ERROR = -4,
    LOCATION_ACCESS_DENIED = -5,
    UNKNOWN_LOCATION_ERROR = -6,
    COULD_NOT_SEND_MESSAGE = -7,
    COULD_NOT_DECODE_MESSAGE = -8,
} Error;

typedef struct {
    int16_t time;
    char* unit;
    char* stop_name;
    char* dest_name;
    char* route_number;
    char* route_name;
    VehicleType vehicle_type; // Limited to STREETCAR and BUS
    GColor color;
    RouteShape shape;
} WindowData;

typedef struct {
    GColor* color;
    int16_t* time;
} AnimIntermediates;

typedef struct {
    WindowData* array;
    int data_len;
    int data_index;
    AnimIntermediates anim_intermediates;
} WindowDataArray;

WindowData* window_data_current(WindowDataArray*);
WindowData* window_data_next(WindowDataArray*);
WindowData* window_data_prev(WindowDataArray*);
int window_data_inc(WindowDataArray*);
int window_data_dec(WindowDataArray*);
int window_data_can_inc(WindowDataArray*);
int window_data_can_dec(WindowDataArray*);
GColor* get_display_gcolor(WindowDataArray*);
int16_t* get_display_time(WindowDataArray*);