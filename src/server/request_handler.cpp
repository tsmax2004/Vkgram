#include <server/requests_handler.h>

#include <concurrency/futures/futures.hpp>

#include <server/server.h>
#include <buffers/buffer.h>
#include <buffers/request.h>
#include <buffers/response.h>
#include <iostream>

RequestsHandler::RequestsHandler(Server& host, size_t threads)
    : host_(host), database_module_(threads), workers_(threads) {}

void RequestsHandler::Start() {
  workers_.Start();
  database_module_.Connect();
}

void RequestsHandler::Stop() {
  workers_.Stop();
}

void RequestsHandler::Handle(std::shared_ptr<Buffer> buffer) {
  executors::Submit(workers_, [&, buffer]() mutable {
    auto type = GetRequestType(*buffer);

    switch (type) {
      case RequestType::LogIn: {
        auto [username, password] = DeserializeAuthenticationRequest(*buffer);
        LogInHandle(username, password, buffer->from_fd);
      }
        break;

      case RequestType::Register: {
        auto [username, password] = DeserializeAuthenticationRequest(*buffer);
        RegisterHandle(username, password, buffer->from_fd);
      }
        break;

      case RequestType::GetContacts:GetContactsHandle(DeserializeGetContactsRequest(*buffer), buffer->from_fd);
        break;

      case RequestType::GetHistory: {
        auto [user1_id, user2_id] = DeserializeGetHistoryRequest(*buffer);
        GetHistoryHandle(user1_id, user2_id, buffer->from_fd);
      }
        break;

      case RequestType::SendMessage: {
        auto [sender_id, receiver_id, text] = DeserializeSendMessageRequest(*buffer);
        SendMessageHandle(sender_id, receiver_id, text);
      }
        break;

      case RequestType::GetUser:GetUserHandle(DeserializeGetUserRequest(*buffer), buffer->from_fd);
        break;
    }

  });
}

void RequestsHandler::LogInHandle(const std::string& username, const std::string& password, int from_fd) {
  auto result = futures::Just()
      | futures::AndThen([&](Unit) { return database_module_.LogIn(username, password); })
      | futures::AndThen([&](User user) {
        return result::Ok(SerializeAuthenticationResponse(ResponseType::LogIn, true, user.id));
      })
      | futures::OrElse([&](Error) {
        return result::Ok(SerializeAuthenticationResponse(ResponseType::LogIn, false, -1));
      })
      | futures::Get();

  auto buffer = *result;
  buffer->to_fd = from_fd;
  host_.SendData(buffer);
}

void RequestsHandler::RegisterHandle(const std::string& username, const std::string& password, int from_fd) {
  auto result = futures::Just()
      | futures::AndThen([&](Unit) { return database_module_.Register(username, password); })
      | futures::AndThen([&](User user) {
        return result::Ok(SerializeAuthenticationResponse(ResponseType::Register, true, user.id));
      })
      | futures::OrElse([&](Error) {
        return result::Ok(SerializeAuthenticationResponse(ResponseType::Register, false, -1));
      })
      | futures::Get();

  auto buffer = *result;
  buffer->to_fd = from_fd;
  host_.SendData(buffer);
}

void RequestsHandler::GetContactsHandle(int user_id, int from_fd) {
  auto contacts = database_module_.GetContacts(user_id);
  auto buffer = SerializeGetContactsResponse(contacts);
  buffer->to_fd = from_fd;
  host_.SendData(buffer);
}

void RequestsHandler::GetHistoryHandle(int user1_id, int user2_id, int from_fd) {
  auto messages = database_module_.GetHistory(user1_id, user2_id);
  auto buffer = SerializeGetHistoryResponse(messages);
  buffer->to_fd = from_fd;
  host_.SendData(buffer);
}

void RequestsHandler::SendMessageHandle(int sender_id, int receiver_id, const std::string& text) {
  database_module_.SendMessage(sender_id, receiver_id, text);
}

void RequestsHandler::GetUserHandle(const std::string& username, int from_fd) {
  auto result = futures::Just()
      | futures::AndThen([&](Unit) { return database_module_.GetUserId(username); })
      | futures::AndThen([&](int user_id) {
        return result::Ok(SerializeGetUserResponse(true, user_id));
      })
      | futures::OrElse([&](Error) {
        return result::Ok(SerializeGetUserResponse(false, -1));
      })
      | futures::Get();

  auto buffer = *result;
  buffer->to_fd = from_fd;
  host_.SendData(buffer);
}