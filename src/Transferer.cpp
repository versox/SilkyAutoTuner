#include "Transferer.h"

#include <iostream>

Transferer::Transferer() {
  openConnection();
}

bool Transferer::openConnection() {
  try {
    session = new Session();

    //Set Options
    session.setOption(SSH_OPTIONS_HOST, HOSTNAME);
    session.setOption(SSH_OPTIONS_USER, USERNAME);
    session.setOption(SSH_OPTIONS_PORT, PORT);

    //Connect
    session.connect();

    //Authenticate
    session.userauthPassword(PASSWORD);

    //Open Channel
    channel = new Channel(session);
    channel.openSession();
  } catch (ssh::SshException e) {
    std::cout << "There was an error while trying to OPEN the connection." << std::endl;
    std::cout << e.getError() << std::endl;
    return false;
  }
  return true;
}

bool Transferer::closeConnection() {
  try {
    //Close channel
    channel.sendEof();
    channel.close();
    //Disconnect
    session.disconnect();
  } catch(ssh::SshException e) {
    std::cout << "There was an error while trying to CLOSE the connection." << std::endl;
    std::cout << e.getError() << std::endl;
    return false;
  }
  return true;
}

bool Transferer::restartConnection() {
  closeConnection();
  openConnection();
}

void Transferer::test() {
  channel.requestExec("echo 'hello' >> test");
}
