#include "serial_handling.h"

extern shared_vars shared;

// TODO: Documentation

void process_waypoint(String& line, lon_lat_32* waypoint_array) {
  int offset = 0;
  line.remove(0, 2);

  // Find space
  for (auto it = line.begin(); it != line.end(); it++) {
    if (*it == ' ') {
      // We found space
      int32_t latitude = line.substring(0, it - line.begin()).toInt();
      int32_t longitude =
          line.substring(it - line.begin() + 1, line.length() - 1).toInt();
      *waypoint_array = lon_lat_32(longitude, latitude);
    }
  }
}

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

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  String line;

  unsigned int N = 0;

  // Send request
  Serial.print("R ");
  Serial.print(start.lat);
  Serial.print(" ");
  Serial.print(start.lon);
  Serial.print(" ");
  Serial.print(end.lat);
  Serial.print(" ");
  Serial.println(end.lon);

  if (read_line(line, 10000) && line[0] == 'N') {
    line.remove(0, 2);
    shared.num_waypoints = line.toInt();
    Serial.println("A");
  } else {
    return 0;
  }

  for (auto i = 0; i < shared.num_waypoints; i++) {
    if (read_line(line)) {
      process_waypoint(line, shared.waypoints + i);
      Serial.println("A");
    } else {
      return 0;
    }
  }

  // 1 indicates a successful exchange, of course you should only output 1
  // in your final solution if the exchange was indeed successful
  // (otherwise, return 0 if the communication failed)
  return 1;
}
