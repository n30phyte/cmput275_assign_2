#include "serial_handling.h"

extern shared_vars shared;

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  shared.num_waypoints = 0;

  // 1 indicates a successful exchange, of course you should only output 1
  // in your final solution if the exchange was indeed successful
  // (otherwise, return 0 if the communication failed)
  return 1;
}
