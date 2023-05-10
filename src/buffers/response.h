#include <sys/types.h>
#include <memory>
#include <string>
#include <vector>

#include <concurrency/result/result.hpp>
#include <messanger/user.h>
#include <messanger/message.h>
#include <buffers/buffer.h>

enum class ResponseType : uint8_t {
  LogIn = 0,
  Register = 1,
  Contacts = 2,
  History = 3,
  User = 4,
};

struct ResponseHeader {
  size_t size;
  ResponseType type;
};

std::shared_ptr<Buffer> SerializeAuthenticationResponse(ResponseType type, bool is_success, int user_id);
concurrency::Result<int> DeserializeAuthenticationResponse(const Buffer& buffer);

std::shared_ptr<Buffer> SerializeGetContactsResponse(const std::vector<User>& contacts);
std::vector<User> DeserializeGetContactsResponse(const Buffer& buffer);

std::shared_ptr<Buffer> SerializeGetHistoryResponse(const std::vector<Message>& messages);
std::vector<Message> DeserializeGetHistoryResponse(const Buffer& buffer);

std::shared_ptr<Buffer> SerializeGetUserResponse(bool is_success, int user_id);
concurrency::Result<int> DeserializeGetUserResponse(const Buffer& buffer);