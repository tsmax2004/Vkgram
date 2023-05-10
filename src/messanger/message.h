#pragma once

#include <messanger/user.h>
#include <chrono>

class Message {
 public:
  int mes_id;
  int sender_id;
  int receiver_id;

  char dt[19];        // format YYYY-MM-DD hh:mm:ss
  std::string text;
};