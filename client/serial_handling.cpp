#include "serial_handling.h"

extern shared_vars shared;

/**
 * Function to get pointer to beginning of string, for use like an iterator
 * 
 * Inspired by my use of this function while writing the assignment using
 * an updated version of the Arduino SDK
 * 
 * @param str: String object to get "iterator" for.
 * @return: "Iterator" for use
 */
char* begin(const String& str) {
    // Force cast from const char* to char* 
  return (char*)str.c_str();
}

/**
 * Function to get pointer to end of string, for use like an iterator
 * 
 * Inspired by my use of this function while writing the assignment using
 * an updated version of the Arduino SDK, similar to the above.
 * 
 * @param str: String object to get "iterator" for.
 * @return: "Iterator" for use
 */
char* end(const String& str) {
  // Force cast from const char* to char* 
  return (char*)(str.c_str() + str.length());
}

/**
 * Process Waypoint message and stores it into the waypoint array
 * 
 * @param line: Waypoint message to process.
 * @param waypoint_array: Pointer to array entry where the data should be stored.
 */
void process_waypoint(String& line, lon_lat_32* const waypoint_array) {
  for (auto it = begin(line); it != end(line); it++) {
    if (*it == ' ') {
      // Space means time to split
      int32_t latitude = line.substring(0, it -begin(line)).toInt();
      int32_t longitude =
          line.substring(it - begin(line) + 1, line.length() - 1).toInt();
      *waypoint_array = lon_lat_32(longitude, latitude);
      // Stop function early
      return;
    }
  }
}

/**
 * Read a line from serial, with timeout set to 1000ms by default
 * 
 * Text is continuously read from serial until \n or \r is hit.
 * 
 * @param line: Reference to where the read line should be store
 * @param timeout: How long the timeout should be in milliseconds
 * @return: true if success, false if timed out.
 */
bool read_line(String& line, unsigned long timeout = 1000) {
  char buffer[22];

  auto start_time = millis();
  uint8_t i = 0;

  char input = ' ';

  while ((millis() - start_time) <= timeout) {
    if (Serial.available()) {
      input = Serial.read();
      buffer[i++] = input;
    }
    if (input == '\n' || input == '\r') {
      buffer[i] = '\0';
      line = String(buffer);
      return true;
    }
  }
  return false;
}

/**
 * Communicates with server over serial
 * 
 * This function runs until last waypoint has been sent.
 * 
 * @param start: Start waypoint
 * @param end: End waypoint
 * @return: 0 if failed for any reason, 1 if not.
 */
uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  String line;

  // Send request
  Serial.print("R ");
  Serial.print(start.lat);
  Serial.print(" ");
  Serial.print(start.lon);
  Serial.print(" ");
  Serial.print(end.lat);
  Serial.print(" ");
  Serial.println(end.lon);

  // Wait for number of waypoints
  if (read_line(line, 10000) && line[0] == 'N') {
    // Did not timeout, send ACK
    line = line.substring(2);
    shared.num_waypoints = line.toInt();
    Serial.println("A");
  } else {
    return 0;
  }

  // Process following waypoints
  for (auto i = 0; i < shared.num_waypoints; i++) {
    if (read_line(line)  && line[0] == 'W') {
      // Process new input
      line = line.substring(2);
      process_waypoint(line, shared.waypoints + i);
      Serial.println("A");
    } else {
      // Timed out
      return 0;
    }
  }
  // Skip waiting for E by server because num of waypoints defined beforehand.
  return 1;
}
