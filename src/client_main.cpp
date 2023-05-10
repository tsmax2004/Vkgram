#include <iostream>
#include <arpa/inet.h>

#include <client/client.h>
#include <server/config.h>

int main(int argc, char** argv) {
  auto client_addr = inet_addr("127.0.0.1");
  auto client_port = htons(8080);

  Client client(client_addr, client_port);
  client.Run(argc, argv);
}