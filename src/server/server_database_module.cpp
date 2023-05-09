#include <server/server_database_module.h>
#include <database/database_config.h>

#include <concurrency/executors/thread_pool.hpp>
#include <fmt/format.h>

using namespace concurrency::executors::tp::fast;

ServerDatabaseModule::ServerDatabaseModule(size_t threads) : num_connections_(threads) {

}

void ServerDatabaseModule::Connect() {
  for (size_t i = 0; i < num_connections_; ++i) {
    connections_.emplace_back(database_config);
  }
}

concurrency::Result<User> ServerDatabaseModule::LogIn(const std::string& username, const std::string& password) {
  using namespace concurrency::result;

  auto& connection = connections_[Worker::CurrentId()];
  pqxx::work tx{connection};

  for (auto [id]: tx.query<int>(
      fmt::format("SELECT user_id FROM vkgram.users WHERE username = '{}' AND password = '{}';", username, password))) {
    return Ok(User{id, username});
  }
  return Err(std::make_error_code(std::errc::bad_message));
}

concurrency::Result<User> ServerDatabaseModule::Register(const std::string& username, const std::string& password) {
  using namespace concurrency::result;

  auto& connection = connections_[Worker::CurrentId()];
  pqxx::work tx{connection};

  bool is_already_registered = false;
  for (auto _: tx.query<int>(
      fmt::format("SELECT user_id FROM vkgram.users WHERE username = '{}';", username))) {
    is_already_registered = true;
  }

  if (is_already_registered) {
    return Err(std::make_error_code(std::errc::bad_message));
  }

  tx.exec(fmt::format("INSERT INTO vkgram.users (username, password) VALUES ('{}', '{}');", username, password));
  tx.commit();

  return LogIn(username, password);
}

concurrency::Result<int> ServerDatabaseModule::GetUserId(const std::string& username) {
  using namespace concurrency::result;

  auto& connection = connections_[Worker::CurrentId()];
  pqxx::work tx{connection};

  for (auto [user_id]: tx.query<int>(
      fmt::format("SELECT user_id FROM vkgram.users WHERE username = '{}';", username))) {
    return Ok(user_id);
  }

  return Err(std::make_error_code(std::errc::bad_message));
}

std::vector<User> ServerDatabaseModule::GetContacts(int user_id) {
  std::vector<User> contacts;

  auto& connection = connections_[Worker::CurrentId()];
  pqxx::work tx{connection};

  for (auto [id, username]: tx.query<int, std::string>(
      fmt::format(
          "WITH contacts_ids AS (\n"
          "    SELECT DISTINCT receiver_id AS id\n"
          "    FROM vkgram.messages\n"
          "    WHERE sender_id = {}\n"
          "\n"
          "    UNION DISTINCT\n"
          "\n"
          "    SELECT DISTINCT sender_id AS id\n"
          "    FROM vkgram.messages\n"
          "    WHERE receiver_id = {}\n"
          ")\n"
          "SELECT u.user_id, username\n"
          "FROM vkgram.users u\n"
          "JOIN contacts_ids i ON (u.user_id = i.id);",
          user_id, user_id)
  )) {
    contacts.push_back({id, username});
  }

  return contacts;
}

std::vector<Message> ServerDatabaseModule::GetHistory(int user1_id, int user2_id) {
  std::vector<Message> messages;

  auto& connection = connections_[Worker::CurrentId()];
  pqxx::work tx{connection};

  for (auto [mes_id, sender_id, receiver_id, dt, text]: tx.query<int, int, int, std::string, std::string>(fmt::format(
      "SELECT mes_id, sender_id, receiver_id, dt, text\n"
      "FROM vkgram.messages\n"
      "WHERE {} IN (sender_id, receiver_id) AND {} IN (sender_id, receiver_id)\n"
      "ORDER BY dt;",
      user1_id, user2_id
  ))) {
    Message message;
    message.mes_id = mes_id;
    message.sender_id = sender_id;
    message.receiver_id = receiver_id;
    message.text = text;
    memcpy(message.dt, dt.c_str(), sizeof(message.dt));

    messages.push_back(message);
  }

  return messages;
}

Message ServerDatabaseModule::SendMessage(int sender_id, int receiver_id, std::string text) {
  auto& connection = connections_[Worker::CurrentId()];
  pqxx::work tx{connection};

  tx.exec(fmt::format(
      "INSERT INTO vkgram.messages (sender_id, receiver_id, dt, text)\n"
      "VALUES ({}, {}, NOW(),'{}');", sender_id, receiver_id, text));

  Message message;
  for (auto [mes_id, dt]: tx.query<int, std::string>(fmt::format(
      "SELECT mes_id, dt\n"
      "FROM vkgram.messages\n"
      "WHERE sender_id = {} AND receiver_id = {}\n"
      "ORDER BY dt DESC;",
      sender_id, receiver_id
  ))) {
    message.mes_id = mes_id;
    message.sender_id = sender_id;
    message.receiver_id = receiver_id;
    message.text = text;
    memcpy(message.dt, dt.c_str(), sizeof(message.dt));
    break;
  }

  tx.commit();
  return message;
}