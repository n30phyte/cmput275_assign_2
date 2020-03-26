#include "serial_handler.h"

Serial::Serial() { port = SerialPort("/dev/ttyUSB0"); }

void Serial::Send(std::string message) {
  message.append("\n");
  port.writeline(message);
}

std::string Serial::Read(int timeout = 10000) { port.readline(timeout); }
