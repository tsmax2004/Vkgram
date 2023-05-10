#pragma once

#include <concurrency/executors/thread_pool.hpp>

#include <server/server_database_module.h>
#include <buffers/buffer.h>
#include <buffers/request.h>

using namespace concurrency;

class Server;

class RequestsHandler {
  using ThreadPool = executors::ThreadPoolFast;

  friend class Server;

 private:
  explicit RequestsHandler(Server& host, size_t threads);

  void Start();
  void Stop();

  void Handle(std::shared_ptr<Buffer>);
  void LogInHandle(const std::string& username, const std::string& password, int from_fd);
  void RegisterHandle(const std::string& username, const std::string& password, int from_fd);
  void GetContactsHandle(int user_id, int from_fd);
  void GetHistoryHandle(int user1_id, int user2_id, int from_fd);
  void GetUserHandle(const std::string& username, int from_fd);
  void SendMessageHandle(int sender_id, int receiver_id, const std::string& text);

 private:
  Server& host_;
  ServerDatabaseModule database_module_;

  ThreadPool workers_;
};