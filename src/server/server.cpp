#include <server/server.h>

#include <concurrency/futures/futures.hpp>

#include <fmt/printf.h>
#include <fmt/color.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <unistd.h>

#include <errors/errors.h>
#include <buffers/buffer.h>

Server::Server(in_addr_t addr, uint16_t port) : server_address_(addr), port_(port), handler_(*this, 4), workers_(4) {}

void Server::Start() {
  StartImpl();
  workers_.Start();
  handler_.Start();

  try {
    WaitingRequestsConnectionsLoop();
  } catch (...) {
    Stop();
    throw;
  }
}

void Server::StartImpl() {
  if ((server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1) {
    throw SocketInitException();
  }

  sockaddr_in socket_addr{};
  socket_addr.sin_addr.s_addr = server_address_;
  socket_addr.sin_port = htons(port_);
  socket_addr.sin_family = AF_INET;

  try {
    int flag = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, (void*) &flag, sizeof(flag)) == -1 ||
        bind(server_socket_, (struct sockaddr*) &socket_addr, sizeof(socket_addr)) < 0) {
      throw SocketBindException();
    }
    if (listen(server_socket_, SOMAXCONN) < 0) {
      throw SocketListenException();
    }
  } catch (...) {
    close(server_socket_);
    throw;
  }

  state_.store(ServerState::Up);
}

void Server::Stop() {
  state_.store(ServerState::Stopped);
  workers_.Stop();
  handler_.Stop();

  close(server_socket_);
}

void Server::WaitingRequestsConnectionsLoop() {
  int kqueue_fd;
  struct kevent change_event{};

  if ((kqueue_fd = kqueue()) < 0) {
    throw KQueueException();
  }
  EV_SET(&change_event, server_socket_, EVFILT_READ, EV_ADD, 0, 0, 0);
  if (kevent(kqueue_fd, &change_event, 1, nullptr, 0, nullptr) == -1) {
    throw KQueueException();
  }

  struct kevent events[SOMAXCONN];
  while (state_.load() == ServerState::Up) {
    int events_cnt = kevent(kqueue_fd, nullptr, 0, events, SOMAXCONN, nullptr);

    for (int i = 0; i < events_cnt; ++i) {
      executors::Submit(workers_, [&, i] {
        int event_fd = events[i].ident;
        if (events[i].flags & EV_EOF) {
          // client eds session
          CloseConnection(event_fd, kqueue_fd, &change_event);
        } else if (event_fd == server_socket_) {
          // new client
          OpenConnection(event_fd, kqueue_fd, &change_event);
        } else if (events[i].filter & EVFILT_READ) {
          // client sends request
          auto buffer = LoadData(event_fd);
          handler_.Handle(buffer);
        }
      });
    }

    // wait until all clients are handled
    workers_.WaitIdle();
  }
}

void Server::OpenConnection(int fd, int kqueue_fd, struct kevent* change_event) {
  try {
    auto socket_connection_fd = accept(fd, nullptr, nullptr);
    if (socket_connection_fd < 0) {
      throw SocketConnectionException();
    }
    EV_SET(change_event, socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0, 0);
    if (kevent(kqueue_fd, change_event, 1, nullptr, 0, nullptr) < 0) {
      throw KQueueException();
    }
    fmt::print("Open new connection!\n");
  } catch (...) {
    fmt::print(fmt::fg(fmt::color::crimson), "Can't open connection with client!\n");
  }
}

void Server::CloseConnection(int fd, int kqueue_fd, struct kevent* change_event) {
  EV_SET(change_event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
  if (kevent(kqueue_fd, change_event, 1, NULL, 0, NULL) == -1) {
    throw KQueueException();
  }

  close(fd);
  fmt::print("Close connection!\n");
}

std::shared_ptr<Buffer> Server::LoadData(int fd) {
  auto buffer_ptr = std::make_shared<Buffer>();
  auto bytes = recv(fd, buffer_ptr->data, sizeof(buffer_ptr->data), 0);
  buffer_ptr->from_fd = fd;
  buffer_ptr->size = bytes;
  return buffer_ptr;
}

void Server::SendData(std::shared_ptr<Buffer> buffer) {
  send(buffer->to_fd, buffer->data, buffer->size, 0);
}
