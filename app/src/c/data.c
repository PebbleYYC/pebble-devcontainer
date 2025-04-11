/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "data.h"

// Retrieves the current data object in the array
WindowData* window_data_current(WindowDataArray* array) {
    return &array->array[array->data_index];
}

/*
These functions assume you've already checked that you can increment or decrement the index
*/
WindowData* window_data_next(WindowDataArray* array) {
    return &array->array[array->data_index + 1];
}

WindowData* window_data_prev(WindowDataArray* array) {
    return &array->array[array->data_index - 1];
}

// Increment the index in the data array
int window_data_inc(WindowDataArray* array) {
    if (array->data_index < array->data_len - 1) {
        array->data_index += 1;
        return 0;
    } else {
        return 1;
    }
}

// Check if the index can be incremented
int window_data_can_inc(WindowDataArray* array) {
    return (array->data_index < array->data_len - 1) ? 0 : 1;
}

// Decrement the index in the data array
int window_data_dec(WindowDataArray* array) {
    if (array->data_index > 0) {
        array->data_index -= 1;
        return 0;
    } else {
        return 1;
    }
}

// Check if the index can be decremented
int window_data_can_dec(WindowDataArray* array) {
    return (array->data_index > 0) ? 0 : 1;
}

/*
Get the color that should currently be displayed on the side
bar, either the set color or an animation intermediate
*/
GColor* get_display_gcolor(WindowDataArray* array) {
    if (array->anim_intermediates.color != NULL) {
        return array->anim_intermediates.color;
    } else {
        WindowData* data = window_data_current(array);
        return &(data->color);
    }
}

/*
Get the time that should currently be displayed, either the set time
or an animation intermediate
*/
int16_t* get_display_time(WindowDataArray* array) {
    if (array->anim_intermediates.time != NULL) {
        return array->anim_intermediates.time;
    } else {
        WindowData* data = window_data_current(array);
        return &(data->time);
    }
}