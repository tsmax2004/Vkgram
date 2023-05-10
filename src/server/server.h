#pragma once

#include <atomic>
#include <netinet/in.h>

#include <concurrency/executors/thread_pool.hpp>
#include <concurrency/result/types/result.hpp>
#include <concurrency/result/types/unit.hpp>

#include <server/requests_handler.h>

using namespace concurrency;

class Server {
  using ThreadPool = executors::ThreadPoolFast;

  friend class RequestsHandler;

 public:
  Server(in_addr_t addr, uint16_t port);

  void Start();
  void Stop();

 private:
  void StartImpl();

  void WaitingRequestsConnectionsLoop();

  void OpenConnection(int fd, int kqueue_fd, struct kevent* change_event);
  void CloseConnection(int fd, int kqueue_fd, struct kevent* change_event);
  std::shared_ptr<Buffer> LoadData(int fd);

  void SendData(std::shared_ptr<Buffer> buffer);

 private:

  enum class ServerState {
    Stopped = 0,
    Up = 1,
  };

  std::atomic<ServerState> state_{ServerState::Stopped};

  in_addr_t server_address_;
  uint16_t port_;
  int server_socket_;

  RequestsHandler handler_;
  ThreadPool workers_;
};