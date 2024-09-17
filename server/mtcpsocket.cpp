#include "mtcpsocket.h"


MTcpSocket::MTcpSocket() {

}


void MTcpSocket::onClientDisconnected() {
    emit disconnected(this);
}
