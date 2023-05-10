#include <memory>
#include <netinet/in.h>
#include <buffers/buffer.h>

class ClientNetworkModule {
 public:
  ClientNetworkModule(in_addr_t addr, uint16_t port);

  void ConnectToServer(in_addr_t server_addr, uint16_t server_port);
  void Disconnect();

  bool SendData(std::shared_ptr<Buffer>);
  std::shared_ptr<Buffer> LoadData();

 private:
  enum class ClientNetworkModuleState {
    Disconnected = 0,
    Connected = 1,
  };

  ClientNetworkModuleState state_{ClientNetworkModuleState::Disconnected};

  in_addr_t client_address_;
  uint16_t client_port_;
  int client_socket_;
};