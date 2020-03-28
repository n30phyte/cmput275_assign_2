#ifndef __SERIAL_HANDLING_H
#define __SERIAL_HANDLING_H
/**
 * Mackenzie Malainey, 1570494
 * Michael Kwok, 1548454
 * Assign #2 Part 2
 * CMPUT 275, Winter 2020
 */

#include "consts_and_types.h"
#include "map_drawing.h"

/**
 * Communicates with server over serial
 * 
 * This function runs until last waypoint has been sent.
 * 
 * @param start: Start waypoint
 * @param end: End waypoint
 * @return: 0 if failed for any reason, 1 if not.
 */
uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end);

#endif
