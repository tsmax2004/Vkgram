#include <iostream>
#include <signal.h>

#include <pqxx/pqxx>

#include <server/server.h>
#include <server/config.h>

Server app_server{server_config::ip_adr, server_config::port};

void sig_handler(int signal) {
  app_server.Stop();
}

int main() {
//   Handle SIGINT
  struct sigaction sigterm_action = {
      .sa_handler = sig_handler, .sa_flags = SA_RESTART};
  sigaction(SIGINT, &sigterm_action, NULL);

  std::cout << "Server starting...\n";

  app_server.Start();

  std::cout << "\nServer stopped!\n";

}