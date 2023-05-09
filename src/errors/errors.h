#pragma once

#include <exception>

class SocketInitException : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "Can't create socket";
  }
};

class SocketBindException : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "Can't bind socket";
  }
};

class SocketConnectionException : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "Can't connect to socket";
  }
};

class SocketListenException : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "Can't start listening on socket";
  }
};

class KQueueException : public std::exception {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "KQueue error";
  }
};