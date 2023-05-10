#include <buffers/response.h>
#include <string>

std::shared_ptr<Buffer> SerializeAuthenticationResponse(ResponseType type, bool is_success, int user_id) {
  auto response_size = (uint32_t) (sizeof(ResponseHeader) + sizeof(int) + sizeof(int));
  auto buffer = std::make_shared<Buffer>();
  buffer->size = response_size;
  auto* dest = buffer->data;

  new(dest) ResponseHeader{response_size, type};
  dest += sizeof(ResponseHeader);
  new(dest) bool(is_success);
  dest += sizeof(bool);
  new(dest) int(user_id);

  return buffer;
}

concurrency::Result<int> DeserializeAuthenticationResponse(const Buffer& buffer) {
  using namespace concurrency::result;

  bool is_success = *(bool*) (buffer.data + sizeof(ResponseHeader));
  if (!is_success) {
    return Err(std::make_error_code(std::errc::bad_message));
  }

  int user_id = *(int*) (buffer.data + sizeof(ResponseHeader) + sizeof(bool));
  return Ok(user_id);
}

std::shared_ptr<Buffer> SerializeGetContactsResponse(const std::vector<User>& contacts) {
  auto response_size = sizeof(ResponseHeader);
  auto buffer = std::make_shared<Buffer>();
  auto* dest = buffer->data + sizeof(ResponseHeader);

  for (auto& contact: contacts) {
    new(dest) int(contact.id);
    dest += sizeof(int);

    uint16_t size = contact.username.size();
    new(dest) uint16_t(size);
    dest += sizeof(uint16_t);

    memcpy(dest, contact.username.c_str(), size);
    dest += size;

    response_size += size + sizeof(uint16_t) + sizeof(int);
  }

  new(buffer->data) ResponseHeader{response_size, ResponseType::Contacts};
  buffer->size = response_size;
  return buffer;
}

std::vector<User> DeserializeGetContactsResponse(const Buffer& buffer) {
  std::vector<User> contacts;

  auto response_size = ((ResponseHeader*) buffer.data)->size;
  auto src = buffer.data + sizeof(ResponseHeader);

  while (src - buffer.data < response_size) {
    int user_id = *(int*) src;
    src += sizeof(int);

    auto size = *(uint16_t*) src;
    src += sizeof(uint16_t);

    contacts.push_back({user_id, std::string(src, size)});
    src += size;
  }

  return contacts;
}

std::shared_ptr<Buffer> SerializeGetHistoryResponse(const std::vector<Message>& messages) {
  auto response_size = sizeof(ResponseHeader);
  auto buffer = std::make_shared<Buffer>();
  auto* dest = buffer->data + sizeof(ResponseHeader);

  for (auto& message: messages) {
    new(dest) int(message.mes_id);
    dest += sizeof(int);
    new(dest) int(message.sender_id);
    dest += sizeof(int);
    new(dest) int(message.receiver_id);
    dest += sizeof(int);

    memcpy(dest, message.dt, sizeof(message.dt));
    dest += sizeof(message.dt);

    uint16_t size = message.text.size();
    new(dest) uint16_t(size);
    dest += sizeof(uint16_t);

    memcpy(dest, message.text.c_str(), size);
    dest += size;

    response_size += size + sizeof(uint16_t) + 3 * sizeof(int) + sizeof(message.dt);
  }

  new(buffer->data) ResponseHeader{response_size, ResponseType::Contacts};
  buffer->size = response_size;
  return buffer;
}

std::vector<Message> DeserializeGetHistoryResponse(const Buffer& buffer) {
  std::vector<Message> messages;

  auto response_size = ((ResponseHeader*) buffer.data)->size;
  auto src = buffer.data + sizeof(ResponseHeader);

  while (src - buffer.data < response_size) {
    Message message;

    message.mes_id = *(int*) src;
    src += sizeof(int);
    message.sender_id = *(int*) src;
    src += sizeof(int);
    message.receiver_id = *(int*) src;
    src += sizeof(int);

    memcpy(message.dt, src, sizeof(message.dt));
    src += sizeof(message.dt);

    auto size = *(uint16_t*) src;
    src += sizeof(uint16_t);

    message.text.assign(src, size);
    messages.push_back(std::move(message));

    src += size;
  }

  return messages;
}

std::shared_ptr<Buffer> SerializeGetUserResponse(bool is_success, int user_id) {
  return SerializeAuthenticationResponse(ResponseType::User, is_success, user_id);
}

concurrency::Result<int> DeserializeGetUserResponse(const Buffer& buffer) {
  return DeserializeAuthenticationResponse(buffer);
}
