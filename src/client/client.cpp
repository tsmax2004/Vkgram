#include <concurrency/futures/futures.hpp>

#include <client/client.h>
#include <server/config.h>
#include <buffers/request.h>
#include <buffers/response.h>

using namespace concurrency;

Client::Client(in_addr_t addr, uint16_t port) : workers_(2), network_(addr, port) {
}

void Client::Run(int argc, char** argv) {
  workers_.Start();
  Connect(server_config::ip_adr, server_config::port);

  QApplication app(argc, argv);
  ui_ = std::make_shared<Ui::MainWindow>(*this);
  ui_->show();

  app.exec();

  is_stopped_.store(true);
  workers_.Stop();
  Disconnect();
}

void Client::Connect(in_addr_t server_addr, uint16_t server_port) {
  network_.ConnectToServer(server_addr, server_port);
}

void Client::Disconnect() {
  network_.Disconnect();
}

bool Client::LogIn(const std::string& username, const std::string& password) {
  return AuthenticationRequestImpl(RequestType::LogIn, username, password);
}

bool Client::Register(const std::string& username, const std::string& password) {
  return AuthenticationRequestImpl(RequestType::Register, username, password);
}

bool Client::AuthenticationRequestImpl(RequestType type, const std::string& username, const std::string& password) {
  auto [is_success_future, promise] = futures::Contract<bool>();

  fibers::Go(workers_, [&, p = std::move(promise)]() mutable {

    std::unique_lock guard(connection_mutex_);
    auto result = futures::Just()
        | futures::AndThen([&](Unit) {
          network_.SendData(SerializeAuthenticationRequest(type, username, password));
          return result::Ok();
        })
        | futures::AndThen([&](Unit) {
          auto id = DeserializeAuthenticationResponse(*network_.LoadData());
          guard.unlock();
          return id;
        })
        | futures::AndThen([&](int id) {
          // Successful
          current_user_ = {id, username};
          contacts_sync_.store(true);
          return result::Ok(true);
        })
        | futures::OrElse([&](Error) { return result::Ok(false);  /* Unsuccessful */ })
        | futures::Await();
    std::move(p).SetValue(*result);
  });

  auto is_success = std::move(is_success_future) | futures::Get();
  return *is_success;
}

void Client::LogOut() {
  contacts_sync_.store(false);
}

std::vector<User> Client::GetContacts() {
  std::lock_guard guard(connection_mutex_);
  network_.SendData(SerializeGetContactsRequest(current_user_.id));
  return DeserializeGetContactsResponse(*network_.LoadData());
}

std::vector<Message> Client::GetHistoryWith(const User& user) {
  std::lock_guard guard(connection_mutex_);
  network_.SendData(SerializeGetHistoryRequest(current_user_.id, user.id));
  return DeserializeGetHistoryResponse(*network_.LoadData());
}

void Client::OpenChat(const User& user) {
  current_contact_ = user;
  messages_sync_.store(true);
}

void Client::CloseChat() {
  messages_sync_.store(false);
}

void Client::SendMessage(const User& receiver, const std::string& text) {
  fibers::Go(workers_, [&, receiver, text] {
    std::lock_guard guard(connection_mutex_);
    network_.SendData(SerializeSendMessageRequest(current_user_.id, receiver.id, text));
  });
}

Result<int> Client::FindUser(const std::string& username) {
  bool is_success = false;
  auto [user_id_future, promise] = futures::Contract<int>();

  fibers::Go(workers_, [&, p = std::move(promise)]() mutable {
    std::lock_guard guard(connection_mutex_);
    network_.SendData(SerializeGetUserRequest(username));
    std::move(p).Set(DeserializeGetUserResponse(*network_.LoadData()));
  });

  return std::move(user_id_future) | futures::Get();
}

void Client::Sync() {
  auto [contacts_future, contacts_promise] = futures::Contract<std::vector<User>>();
  auto [messages_future, messages_promise] = futures::Contract<std::vector<Message>>();

  fibers::Go(workers_, [&, c_p = std::move(contacts_promise), m_p = std::move(messages_promise)]() mutable {
    if (contacts_sync_.load()) {
      std::move(c_p).SetValue(GetContacts());
    } else {
      std::move(c_p).SetError(std::make_error_code(std::errc::bad_message));
    }

    if (messages_sync_.load()) {
      std::move(m_p).SetValue(GetHistoryWith(current_contact_));
    } else {
      std::move(m_p).SetError(std::make_error_code(std::errc::bad_message));
    }
  });

  auto contacts = std::move(contacts_future) | futures::Get();
  contacts.and_then([&](const std::vector<User>& contacts) {
    ui_->UpdateContacts(contacts);
    return result::Ok();
  });

  auto messages = std::move(messages_future) | futures::Get();
  messages.and_then([&](const std::vector<Message>& messages) {
    ui_->UpdateChat(messages);
    return result::Ok();
  });
}

