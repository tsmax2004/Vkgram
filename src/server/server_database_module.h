#include <pqxx/pqxx>
#include <vector>
#include <string>
#include <messanger/user.h>
#include <messanger/message.h>
#include <concurrency/result/result.hpp>

class ServerDatabaseModule {
 public:
  ServerDatabaseModule(size_t threads);

  void Connect();

  concurrency::Result<User> LogIn(const std::string& username, const std::string& password);
  concurrency::Result<User> Register(const std::string& username, const std::string& password);

  concurrency::Result<int> GetUserId(const std::string& username);

  std::vector<User> GetContacts(int user_id);

  std::vector<Message> GetHistory(int user1_id, int user2_id);
  Message SendMessage(int sender_id, int receiver_id, std::string text);

 private:
  size_t num_connections_;
  std::vector<pqxx::connection> connections_;
};