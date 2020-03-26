#pragma once

#include <string>

#include "serialport.h"

class Serial {
  SerialPort port;

 public:
  Serial();
  ~Serial();

  void Send(std::string message);
  std::string Read(int timeout = 10000);
};
