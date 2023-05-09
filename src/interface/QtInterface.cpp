#include <interface/QtInterface.hpp>
#include <client/client.h>
#include <fmt/format.h>
#include <concurrency/result/result.hpp>

namespace Ui {

enum Pages : size_t {
  Authentication = 0,
  AllChats = 1,
  Chat = 2,
};

void MainWindow::LoginClicked() {
  auto username = username_input->text().toStdString();
  auto password = password_input->text().toStdString();

  if (host_.LogIn(username, password)) {
    authentication_error_label->setText(QCoreApplication::translate("MainWindow", "", nullptr));
    username_input->clear();
    password_input->clear();
    pages->setCurrentIndex(Pages::AllChats);
  } else {
    authentication_error_label->setText(QCoreApplication::translate("MainWindow",
                                                                    "Incorrect username or password!",
                                                                    nullptr));
  }
}

void MainWindow::RegisterClicked() {
  auto username = username_input->text().toStdString();
  auto password = password_input->text().toStdString();

  if (host_.Register(username, password)) {
    authentication_error_label->setText(QCoreApplication::translate("MainWindow", "", nullptr));
    username_input->clear();
    password_input->clear();
    pages->setCurrentIndex(Pages::AllChats);
  } else {
    authentication_error_label->setText(QCoreApplication::translate("MainWindow",
                                                                    "User with the same username is already existed!",
                                                                    nullptr));
  }
}

void MainWindow::LogoutClicked() {
  host_.LogOut();
  pages->setCurrentIndex(Pages::Authentication);
  contacts_.clear();
}

void MainWindow::UpdateContacts(const std::vector<User>& contacts) {
  contact_buttons_.clear();
  for (auto& contact: contacts) {
    auto button = std::make_shared<ContactButton>(scrollAreaWidgetContentsContacts, this, contact);
    contact_buttons_.push_back(button);
    verticalLayoutContacts->addWidget(button.get());
  }
}

void MainWindow::AllChatsClicked() {
  host_.CloseChat();
  pages->setCurrentIndex(Pages::AllChats);
  messages_.clear();
}

void MainWindow::FindClicked() {
  auto username = new_chat_input->text().toStdString();
  auto user_id = host_.FindUser(username);
  user_id.and_then([&](int user_id) {
    OpenChat({user_id, username});
    return result::Ok();
  });
}

void MainWindow::OpenChat(const User& user) {
  host_.OpenChat(user);

  send_button->disconnect(this);
  QObject::connect(send_button, &QPushButton::clicked, this, [this, user] {
    SendMessage(user, messenger_input->toPlainText().toStdString());
  });

  new_chat_input->clear();
  messanger_scroll_area->verticalScrollBar()->setSliderPosition(messanger_scroll_area->verticalScrollBar()->maximum());
  messenger_title->setText(QString::fromUtf8(user.username));

  pages->setCurrentIndex(Pages::Chat);
}

void MainWindow::UpdateChat(const std::vector<Message>& messages) {
  messages_.clear();

  for (auto& message: messages) {
    auto message_label = std::make_shared<QLabel>(scrollAreaWidgetContentsMessages);
    messages_.push_back(message_label);
    gridLayoutMessages->addWidget(message_label.get());

    auto& sender =
        host_.current_user_.id == message.sender_id ? host_.current_contact_.username : host_.current_user_.username;
    auto message_text =
        fmt::format("[{}] {}: {}", std::string(message.dt, sizeof(message.dt)), sender, message.text);
    message_label->setText(QString::fromUtf8(message_text));
  }
}

void MainWindow::SendMessage(const User& user, const std::string& text) {
  messenger_input->clear();
  host_.SendMessage(user, text);
}

void MainWindow::Sync() {
  host_.Sync();
}

} // namespace Ui