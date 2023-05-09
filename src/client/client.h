#pragma once

#include <vector>

#include <client/client_network.hpp>
#include <interface/QtInterface.hpp>
#include <messanger/user.h>
#include <messanger/message.h>
#include <buffers/request.h>

#include <concurrency/executors/thread_pool.hpp>
#include <concurrency/fibers/sched/go.hpp>
#include <concurrency/fibers/sync/mutex.hpp>
#include <concurrency/result/result.hpp>

using namespace concurrency;

class Client {
  using ThreadPool = concurrency::executors::ThreadPoolFast;

  friend class Ui::MainWindow;

 public:
  Client(in_addr_t addr, uint16_t port);

  void Run(int argc, char** argv);

  void Connect(in_addr_t server_addr, uint16_t server_port);
  void Disconnect();

  bool LogIn(const std::string& username, const std::string& password);
  bool Register(const std::string& username, const std::string& password);
  void LogOut();

  std::vector<User> GetContacts();
  std::vector<Message> GetHistoryWith(const User&);

  void OpenChat(const User& user);
  void CloseChat();

  Result<int> FindUser(const std::string&);

  void SendMessage(const User& receiver, const std::string& text);

 private:
  void Sync();

  bool AuthenticationRequestImpl(RequestType type, const std::string& username, const std::string& password);

  ThreadPool workers_;
  fibers::Mutex connection_mutex_;

  ClientNetworkModule network_;   // guarded by mutex
  std::shared_ptr<Ui::MainWindow> ui_;

  User current_user_;
  User current_contact_;
  std::atomic<bool> is_stopped_{false};
  std::atomic<bool> contacts_sync_{false};
  std::atomic<bool> messages_sync_{false};
};