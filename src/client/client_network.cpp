#include "client_network.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <errors/errors.h>

ClientNetworkModule::ClientNetworkModule(in_addr_t addr, uint16_t port) : client_address_(addr), client_port_(port) {
}

void ClientNetworkModule::ConnectToServer(in_addr_t server_addr, uint16_t server_port) {
  if ((client_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0) {
    throw SocketInitException();
  }

  sockaddr_in socket_addr{};
  socket_addr.sin_addr.s_addr = server_addr;
  socket_addr.sin_port = htons(server_port);
  socket_addr.sin_family = AF_INET;

  if (connect(client_socket_, (sockaddr*) (&socket_addr), sizeof(socket_addr)) != 0) {
    close(client_socket_);
    throw SocketConnectionException();
  }

  state_ = ClientNetworkModuleState::Connected;
}

void ClientNetworkModule::Disconnect() {
  shutdown(client_socket_, 0);
  close(client_socket_);
  state_ = ClientNetworkModuleState::Disconnected;
}

bool ClientNetworkModule::SendData(std::shared_ptr<Buffer> buffer) {
  send(client_socket_, buffer->data, buffer->size, 0);
}

std::shared_ptr<Buffer> ClientNetworkModule::LoadData() {
  auto buffer_ptr = std::make_shared<Buffer>();
  auto bytes = recv(client_socket_, buffer_ptr->data, sizeof(buffer_ptr->data), 0);
  buffer_ptr->size = bytes;
  return buffer_ptr;
}

