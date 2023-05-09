#pragma once

#include <memory>
#include <string>

#include <buffers/buffer.h>
#include <messanger/message.h>

enum RequestType : uint16_t {
  LogIn = 0,
  Register = 1,
  GetContacts = 2,
  GetHistory = 3,
  SendMessage = 4,
  GetUser = 5,
};

struct RequestHeader {
  uint32_t size;
  RequestType request_type;
};

RequestType GetRequestType(const Buffer& buffer);

std::shared_ptr<Buffer> SerializeAuthenticationRequest(RequestType type, const std::string& username, const std::string& password);
std::pair<std::string, std::string> DeserializeAuthenticationRequest(const Buffer& buffer);

std::shared_ptr<Buffer> SerializeGetContactsRequest(int user_id);
int DeserializeGetContactsRequest(const Buffer& buffer);

std::shared_ptr<Buffer> SerializeGetHistoryRequest(int user1_id, int user2_id);
std::pair<int, int> DeserializeGetHistoryRequest(const Buffer& buffer);

std::shared_ptr<Buffer> SerializeGetUserRequest(const std::string& username);
std::string DeserializeGetUserRequest(const Buffer& buffer);

std::shared_ptr<Buffer> SerializeSendMessageRequest(int sender_id, int receiver_id, const std::string& text);
std::tuple<int, int, std::string> DeserializeSendMessageRequest(const Buffer& buffer);

