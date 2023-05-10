#pragma once

#ifndef MAINWINDOW_20__2D__20_UNTITLEDHQEFUR_H
#define MAINWINDOW_20__2D__20_UNTITLEDHQEFUR_H

#include <QtWidgets/QApplication>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QScrollBar>
#include <QTimer>

#include <messanger/user.h>
#include <messanger/message.h>

class Client;

QT_BEGIN_NAMESPACE

class Ui_MainWindow : public QObject {
 public:
  Ui_MainWindow() = default;

 public:
  QWidget* central_widget;
  QStackedWidget* pages;
  QWidget* authentication_page;
  QWidget* widget;
  QGridLayout* gridLayout;
  QPushButton* register_button;
  QLabel* username_label;
  QLineEdit* username_input;
  QLabel* password_label;
  QLineEdit* password_input;
  QLabel* authentication_title;
  QPushButton* login_button;
  QWidget* chats_page;
  QScrollArea* chats_scroll;
  QWidget* scrollAreaWidgetContentsContacts;
  QVBoxLayout* verticalLayoutContacts;
  QLineEdit* new_chat_input;
  QLabel* new_chat_label;
  QLabel* chats_title;
  QPushButton* logout_button;
  QLabel* prev_chats_label;
  QWidget* messenger_page;
  QScrollArea* messanger_scroll_area;
  QWidget* scrollAreaWidgetContentsMessages;
  QGridLayout* gridLayoutMessages;
  QTextEdit* messenger_input;
  QPushButton* send_button;
  QLabel* messenger_title;
  QPushButton* all_chats_button;
  QPushButton* find_button;
  QLabel* authentication_error_label;

  void setupUi(QMainWindow* MainWindow) {
    if (MainWindow->objectName().isEmpty())
      MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->setEnabled(true);
    MainWindow->resize(450, 550);
    MainWindow->setMinimumSize(QSize(450, 550));
    MainWindow->setMaximumSize(QSize(450, 550));
    central_widget = new QWidget(MainWindow);
    central_widget->setObjectName(QString::fromUtf8("central_widget"));
    central_widget->setEnabled(true);
    pages = new QStackedWidget(central_widget);
    pages->setObjectName(QString::fromUtf8("pages"));
    pages->setGeometry(QRect(0, 0, 450, 550));
    pages->setAutoFillBackground(false);
    authentication_page = new QWidget();
    authentication_page->setObjectName(QString::fromUtf8("authentication_page"));
    widget = new QWidget(authentication_page);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(50, 110, 341, 291));
    gridLayout = new QGridLayout(widget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    register_button = new QPushButton(widget);
    register_button->setObjectName(QString::fromUtf8("register_button"));

    gridLayout->addWidget(register_button, 5, 0, 1, 2);

    username_label = new QLabel(widget);
    username_label->setObjectName(QString::fromUtf8("username_label"));
    QFont font;
    font.setPointSize(15);
    username_label->setFont(font);

    gridLayout->addWidget(username_label, 2, 0, 1, 1);

    authentication_error_label = new QLabel(widget);
    authentication_error_label->setObjectName(QString::fromUtf8("authentication_error_label"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(authentication_error_label->sizePolicy().hasHeightForWidth());
    authentication_error_label->setSizePolicy(sizePolicy1);
    authentication_error_label->setStyleSheet("color: red;");

    gridLayout->addWidget(authentication_error_label, 1, 0, 1, 2);

    username_input = new QLineEdit(widget);
    username_input->setObjectName(QString::fromUtf8("username_input"));

    gridLayout->addWidget(username_input, 2, 1, 1, 1);

    password_label = new QLabel(widget);
    password_label->setObjectName(QString::fromUtf8("password_label"));
    password_label->setFont(font);

    gridLayout->addWidget(password_label, 3, 0, 1, 1);

    password_input = new QLineEdit(widget);
    password_input->setObjectName(QString::fromUtf8("password_input"));

    gridLayout->addWidget(password_input, 3, 1, 1, 1);

    authentication_title = new QLabel(widget);
    authentication_title->setObjectName(QString::fromUtf8("authentication_title"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(authentication_title->sizePolicy().hasHeightForWidth());
    authentication_title->setSizePolicy(sizePolicy);
    QFont font1;
    font1.setPointSize(22);
    authentication_title->setFont(font1);
    authentication_title->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(authentication_title, 0, 0, 1, 2);

    login_button = new QPushButton(widget);
    login_button->setObjectName(QString::fromUtf8("login_button"));

    gridLayout->addWidget(login_button, 4, 0, 1, 2);

    pages->addWidget(authentication_page);
    chats_page = new QWidget();
    chats_page->setObjectName(QString::fromUtf8("chats_page"));
    chats_scroll = new QScrollArea(chats_page);
    chats_scroll->setObjectName(QString::fromUtf8("chats_scroll"));
    chats_scroll->setGeometry(QRect(20, 160, 411, 371));
    chats_scroll->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
    chats_scroll->setWidgetResizable(true);
    chats_scroll->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    scrollAreaWidgetContentsContacts = new QWidget();
    scrollAreaWidgetContentsContacts->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContentsContacts->setGeometry(QRect(0, 0, 409, 369));
    verticalLayoutContacts = new QVBoxLayout(scrollAreaWidgetContentsContacts);
    verticalLayoutContacts->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayoutContacts->setContentsMargins(10, 10, 10, 10);
    verticalLayoutContacts->setAlignment(Qt::AlignTop);

    chats_scroll->setWidget(scrollAreaWidgetContentsContacts);
    new_chat_input = new QLineEdit(chats_page);
    new_chat_input->setObjectName(QString::fromUtf8("new_chat_input"));
    new_chat_input->setGeometry(QRect(150, 80, 191, 31));
    new_chat_label = new QLabel(chats_page);
    new_chat_label->setObjectName(QString::fromUtf8("new_chat_label"));
    new_chat_label->setGeometry(QRect(40, 80, 111, 31));
    QFont font3;
    font3.setPointSize(16);
    new_chat_label->setFont(font3);
    chats_title = new QLabel(chats_page);
    chats_title->setObjectName(QString::fromUtf8("chats_title"));
    chats_title->setGeometry(QRect(180, 30, 111, 31));
    QFont font4;
    font4.setPointSize(25);
    chats_title->setFont(font4);
    logout_button = new QPushButton(chats_page);
    logout_button->setObjectName(QString::fromUtf8("logout_button"));
    logout_button->setGeometry(QRect(10, 10, 100, 32));
    prev_chats_label = new QLabel(chats_page);
    prev_chats_label->setObjectName(QString::fromUtf8("prev_chats_label"));
    prev_chats_label->setGeometry(QRect(40, 110, 111, 31));
    prev_chats_label->setFont(font3);
    find_button = new QPushButton(chats_page);
    find_button->setObjectName(QString::fromUtf8("find_button"));
    find_button->setGeometry(QRect(360, 80, 71, 32));
    pages->addWidget(chats_page);

    messenger_page = new QWidget();
    messenger_page->setObjectName(QString::fromUtf8("messenger_page"));
    messanger_scroll_area = new QScrollArea(messenger_page);
    messanger_scroll_area->setObjectName(QString::fromUtf8("messenget_scroll_area"));
    messanger_scroll_area->setGeometry(QRect(10, 60, 431, 384));
    messanger_scroll_area->setWidgetResizable(true);
    scrollAreaWidgetContentsMessages = new QWidget();
    scrollAreaWidgetContentsMessages->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
    scrollAreaWidgetContentsMessages->setGeometry(QRect(0, 0, 429, 382));
    gridLayoutMessages = new QGridLayout(scrollAreaWidgetContentsMessages);
    gridLayoutMessages->setObjectName(QString::fromUtf8("gridLayout_3"));
    gridLayoutMessages->setAlignment(Qt::AlignTop);

    messanger_scroll_area->setWidget(scrollAreaWidgetContentsMessages);
    messenger_input = new QTextEdit(messenger_page);
    messenger_input->setObjectName(QString::fromUtf8("messenger_input"));
    messenger_input->setGeometry(QRect(10, 480, 361, 41));
    send_button = new QPushButton(messenger_page);
    send_button->setObjectName(QString::fromUtf8("send_button"));
    send_button->setGeometry(QRect(380, 480, 61, 32));
    messenger_title = new QLabel(messenger_page);
    messenger_title->setObjectName(QString::fromUtf8("messenger_title"));
    messenger_title->setGeometry(QRect(180, 10, 131, 41));
    messenger_title->setFont(font1);
    all_chats_button = new QPushButton(messenger_page);
    all_chats_button->setObjectName(QString::fromUtf8("all_chats_button"));
    all_chats_button->setGeometry(QRect(10, 10, 100, 32));
    pages->addWidget(messenger_page);
    MainWindow->setCentralWidget(central_widget);

    retranslateUi(MainWindow);

    pages->setCurrentIndex(0);

    QMetaObject::connectSlotsByName(MainWindow);
  } // setupUi

  void retranslateUi(QMainWindow* MainWindow) {
    MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
    register_button->setText(QCoreApplication::translate("MainWindow", "Register", nullptr));
    username_label->setText(QCoreApplication::translate("MainWindow", "Username:", nullptr));
    password_label->setText(QCoreApplication::translate("MainWindow", "Password:", nullptr));
    authentication_title->setText(QCoreApplication::translate("MainWindow", "Authentication", nullptr));
    login_button->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
    new_chat_label->setText(QCoreApplication::translate("MainWindow", "New chat with:", nullptr));
    chats_title->setText(QCoreApplication::translate("MainWindow", "Chats", nullptr));
    logout_button->setText(QCoreApplication::translate("MainWindow", "LogOut", nullptr));
    prev_chats_label->setText(QCoreApplication::translate("MainWindow", "Previous chats:", nullptr));
    find_button->setText(QCoreApplication::translate("MainWindow", "Find", nullptr));
    send_button->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
    messenger_title->setText(QCoreApplication::translate("MainWindow", "Contact", nullptr));
    all_chats_button->setText(QCoreApplication::translate("MainWindow", "All Chats", nullptr));
  } // retranslateUi


  void setSignals() {
    QObject::connect(login_button, &QPushButton::clicked, this, &Ui_MainWindow::LoginClicked);
    QObject::connect(register_button, &QPushButton::clicked, this, &Ui_MainWindow::RegisterClicked);
    QObject::connect(logout_button, &QPushButton::clicked, this, &Ui_MainWindow::LogoutClicked);
    QObject::connect(all_chats_button, &QPushButton::clicked, this, &Ui_MainWindow::AllChatsClicked);
    QObject::connect(find_button, &QPushButton::clicked, this, &Ui_MainWindow::FindClicked);
  }

 public slots:
  virtual void OpenChat(const User& user) = 0;
  virtual void SendMessage(const User& user, const std::string& text) = 0;
  virtual void UpdateContacts(const std::vector<User>&) = 0;

 private slots:
  virtual void LoginClicked() = 0;
  virtual void RegisterClicked() = 0;
  virtual void LogoutClicked() = 0;
  virtual void AllChatsClicked() = 0;
  virtual void FindClicked() = 0;
};

class ContactButton : public QPushButton {
 public:
  ContactButton(QWidget* parent, Ui_MainWindow* main, const User& user) {
    moveToThread(main->thread());
    setParent(parent);
    setObjectName(QString::fromUtf8("ChatButton_" + user.username));
    setMinimumSize(QSize(0, 50));
    QFont font;
    font.setPointSize(18);
    setFont(font);

    QObject::connect(this, &QPushButton::clicked, main, [main, user] {
      main->OpenChat(user);
    });

    setText(QString::fromUtf8(user.username));
  }
};

namespace Ui {
class MainWindow : public Ui_MainWindow {

 public:
  MainWindow(Client& host) : host_(host) {
    setupUi(&window_);
    setSignals();

    auto activeTimer = new QTimer(this);
    activeTimer->setInterval(1000);
    activeTimer->setSingleShot(false);
    connect(activeTimer, &QTimer::timeout, this, &MainWindow::Sync);
    activeTimer->start();
  }

  void show() {
    window_.show();
  }

  void UpdateContacts(const std::vector<User>& contacts);
  void UpdateChat(const std::vector<Message>& messages);

 public slots:
  void OpenChat(const User& username) override;
  void SendMessage(const User& user, const std::string& text) override;
  void Sync();

 private slots:
  void LoginClicked() override;
  void RegisterClicked() override;
  void LogoutClicked() override;
  void AllChatsClicked() override;
  void FindClicked() override;

 private:
  QMainWindow window_;
  Client& host_;

  std::vector<User> contacts_;

  std::vector<std::shared_ptr<ContactButton>> contact_buttons_;
  std::vector<std::shared_ptr<QLabel>> messages_;
};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOW_20__2D__20_UNTITLEDHQEFUR_H
