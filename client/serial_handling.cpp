#include "serial_handling.h"

extern shared_vars shared;

// TODO: Documentation

char* begin(const String& str) {
  return (char*)str.c_str();
}

char* end(const String& str) {
  return (char*)(str.c_str() + str.length());
}


void process_waypoint(String& line, lon_lat_32* waypoint_array) {
  // Find space
  for (auto it = begin(line); it != end(line); it++) {
    if (*it == ' ') {
      // We found space
      int32_t latitude = line.substring(0, it -begin(line)).toInt();
      int32_t longitude =
          line.substring(it - begin(line) + 1, line.length() - 1).toInt();
      *waypoint_array = lon_lat_32(longitude, latitude);
      return;
    }
  }
}

bool read_line(String& line, unsigned long timeout = 1000) {
  char buffer[22];

  auto start_time = millis();
  uint8_t i = 0;

  char input = ' ';

  // while ((millis() - start_time) <= timeout) {
    while (true) {
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
    line = line.substring(2);
    shared.num_waypoints = line.toInt();
    Serial.println("A");
  } else {
    return 0;
  }

  for (auto i = 0; i < shared.num_waypoints; i++) {
    if (read_line(line)  && line[0] == 'W') {
      line = line.substring(2);
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
