#ifndef TRANSFERER_H
#define TRANSFERER_H

#define SSH_NO_CPP_EXCEPTIONS
#include "libssh/libsshpp.hpp"

#define HOSTNAME "127.0.0.1"
#define USERNAME "user"
#define PASSWORD "pass"
#define PORT 22

class Transferer {
private:
  ssh::Session session;
  ssh::Channel channel;
public:
  Transferer();
  bool openConnection();
  bool closeConnection();
  void restartConnection();

  void test();
};

#endif
