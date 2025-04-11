/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <pebble.h>
#include "anim_vehicle.h"
#include "data.h"

#define VEHICLE_SCROLL_DURATION_MS 200
#define VEHICLE_SCROLL_DIST 40

Animation *create_vehicle_outbound_anim(ScrollDirection direction, Layer *vehicle_layer, AnimationStoppedHandler stopped_handler) {
    int16_t to_dy = (direction == ScrollDirectionDown) ? -VEHICLE_SCROLL_DIST : VEHICLE_SCROLL_DIST;
    GPoint to_origin = GPoint(0, to_dy);

    Animation *out_anim = (Animation *)property_animation_create_bounds_origin(vehicle_layer, NULL, &to_origin);
    animation_set_duration(out_anim, VEHICLE_SCROLL_DURATION_MS);
    animation_set_curve(out_anim, AnimationCurveLinear);
    animation_set_handlers(out_anim, (AnimationHandlers){
                                        .stopped = stopped_handler,
                                    },
                           NULL);

    return out_anim;
}

Animation *create_vehicle_inbound_anim(ScrollDirection direction, Layer *vehicle_layer) {
    int16_t from_dy = (direction == ScrollDirectionDown) ? -VEHICLE_SCROLL_DIST : VEHICLE_SCROLL_DIST;
    GPoint from_origin = GPoint(0, from_dy);

    Animation *in_anim = (Animation *)property_animation_create_bounds_origin(vehicle_layer, &from_origin, NULL);
    animation_set_duration(in_anim, VEHICLE_SCROLL_DURATION_MS);
    animation_set_curve(in_anim, AnimationCurveEaseOut);

    return in_anim;
}