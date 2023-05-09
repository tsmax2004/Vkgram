#include <buffers/request.h>
#include <string>

RequestType GetRequestType(const Buffer& buffer) {
  return ((RequestHeader*) buffer.data)->request_type;
}

std::shared_ptr<Buffer> SerializeAuthenticationRequest(RequestType type,
                                                       const std::string& username,
                                                       const std::string& password) {
  auto request_size = (uint32_t) (sizeof(RequestHeader) + sizeof(uint16_t) + username.size() + password.size());
  auto buffer = std::make_shared<Buffer>();
  buffer->size = request_size;
  auto* dest = buffer->data;

  new(dest) RequestHeader{request_size, type};
  dest += sizeof(RequestHeader);
  new(dest) uint16_t(username.size());
  dest += sizeof(uint16_t);
  memcpy(dest, username.c_str(), username.size());
  dest += username.size();
  memcpy(dest, password.c_str(), password.size());

  return buffer;
}

std::pair<std::string, std::string> DeserializeAuthenticationRequest(const Buffer& buffer) {
  std::pair<std::string, std::string> request;

  RequestHeader header = *(RequestHeader*) buffer.data;

  uint16_t username_size = *(uint16_t*) (buffer.data + sizeof(RequestHeader));
  uint16_t password_size = header.size - (sizeof(RequestHeader) + sizeof(uint16_t) + username_size);

  auto username_offset = buffer.data + sizeof(RequestHeader) + sizeof(uint16_t);
  auto password_offset = username_offset + username_size;

  request.first.assign(username_offset, username_size);
  request.second.assign(password_offset, password_size);

  return request;
}

std::shared_ptr<Buffer> SerializeGetContactsRequest(int user_id) {
  auto request_size = (uint32_t) (sizeof(RequestHeader) + sizeof(int));
  auto buffer = std::make_shared<Buffer>();
  buffer->size = request_size;
  auto* dest = buffer->data;

  new(dest) RequestHeader{request_size, RequestType::GetContacts};
  dest += sizeof(RequestHeader);
  new(dest) int(user_id);

  return buffer;
}

int DeserializeGetContactsRequest(const Buffer& buffer) {
  int user_id = *(int*) (buffer.data + sizeof(RequestHeader));
  return user_id;
}

std::shared_ptr<Buffer> SerializeGetHistoryRequest(int user1_id, int user2_id) {
  auto request_size = (uint32_t) (sizeof(RequestHeader) + sizeof(int) + sizeof(int));
  auto buffer = std::make_shared<Buffer>();
  buffer->size = request_size;
  auto* dest = buffer->data;

  new(dest) RequestHeader{request_size, RequestType::GetHistory};
  dest += sizeof(RequestHeader);
  new(dest) int(user1_id);
  dest += sizeof(int);
  new(dest) int(user2_id);

  return buffer;
}

std::pair<int, int> DeserializeGetHistoryRequest(const Buffer& buffer) {
  int user1_id = *(int*) (buffer.data + sizeof(RequestHeader));
  int user2_id = *(int*) (buffer.data + sizeof(RequestHeader) + sizeof(int));
  return {user1_id, user2_id};
}

std::shared_ptr<Buffer> SerializeSendMessageRequest(int sender_id, int receiver_id, const std::string& text) {
  auto request_size = (uint32_t) (sizeof(RequestHeader) + 2 * sizeof(int) + sizeof(uint16_t) + text.size());
  auto buffer = std::make_shared<Buffer>();
  buffer->size = request_size;
  auto* dest = buffer->data;

  new(dest) RequestHeader{request_size, RequestType::SendMessage};
  dest += sizeof(RequestHeader);
  new(dest) int(sender_id);
  dest += sizeof(int);
  new(dest) int(receiver_id);
  dest += sizeof(int);
  new(dest) uint16_t(text.size());
  dest += sizeof(uint16_t);

  memcpy(dest, text.c_str(), text.size());

  return buffer;
}

std::tuple<int, int, std::string> DeserializeSendMessageRequest(const Buffer& buffer) {
  std::tuple<int, int, std::string> message;

  get<0>(message) = *(int*) (buffer.data + sizeof(RequestHeader));
  get<1>(message) = *(int*) (buffer.data + sizeof(RequestHeader) + sizeof(int));

  uint16_t size = *(uint16_t*) (buffer.data + sizeof(RequestHeader) + 2 * sizeof(int));
  auto* src = buffer.data + sizeof(RequestHeader) + 2 * sizeof(int) + sizeof(uint16_t);
  get<2>(message).assign(std::string(src, size));

  return message;
}

std::shared_ptr<Buffer> SerializeGetUserRequest(const std::string& username) {
  auto request_size = (uint32_t) (sizeof(RequestHeader) + sizeof(uint16_t) + username.size());
  auto buffer = std::make_shared<Buffer>();
  buffer->size = request_size;
  auto* dest = buffer->data;

  new(dest) RequestHeader{request_size, RequestType::GetUser};
  dest += sizeof(RequestHeader);
  new(dest) uint16_t(username.size());
  dest += sizeof(uint16_t);
  memcpy(dest, username.c_str(), username.size());

  return buffer;
}

std::string DeserializeGetUserRequest(const Buffer& buffer) {
  std::string request;

  RequestHeader header = *(RequestHeader*) buffer.data;

  uint16_t username_size = *(uint16_t*) (buffer.data + sizeof(RequestHeader));
  auto username_offset = buffer.data + sizeof(RequestHeader) + sizeof(uint16_t);
  request.assign(username_offset, username_size);

  return request;
}