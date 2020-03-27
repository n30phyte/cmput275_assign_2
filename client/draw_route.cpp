#include "draw_route.h"

#include "map_drawing.h"

extern shared_vars shared;

void draw_route() {
  auto zoom_level = shared.map_number;

  for (int16_t i = 0; i < shared.num_waypoints - 1; i++) {
    lon_lat_32 cord = shared.waypoints[i];
    lon_lat_32 cord2 = shared.waypoints[i + 1];
    xy_pos true_cord = xy_pos(longitude_to_x(zoom_level, cord.lon),
                              latitude_to_y(zoom_level, cord.lat));
    xy_pos true_cord2 = xy_pos(longitude_to_x(zoom_level, cord2.lon),
                               latitude_to_y(zoom_level, cord2.lat));
    shared.tft->fillCircle(true_cord.x - shared.map_coords.x,
                           true_cord.y - shared.map_coords.y, 2, 0xFF0000);
    shared.tft->drawLine(true_cord.x - shared.map_coords.x,
                         true_cord.y - shared.map_coords.y,
                         true_cord2.x - shared.map_coords.x,
                         true_cord2.y - shared.map_coords.y, 0xFF0000);
  }

  lon_lat_32 cord = shared.waypoints[shared.num_waypoints - 1];
  xy_pos true_cord = xy_pos(longitude_to_x(zoom_level, cord.lon),
                            latitude_to_y(zoom_level, cord.lat));
  shared.tft->fillCircle(true_cord.x - shared.map_coords.x,
                         true_cord.y - shared.map_coords.y, 4, 0xFF0000);
}
