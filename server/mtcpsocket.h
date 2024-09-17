#ifndef MTCPSOCKET_H
#define MTCPSOCKET_H

#include <QTcpSocket>

class MTcpSocket : public QTcpSocket {
    Q_OBJECT
public:
    MTcpSocket();
private:
    QTcpSocket *m_tcp_socket;
public:
    inline void setTcpSocket(QTcpSocket *socket) {
        this->m_tcp_socket = socket;
        connect(this->m_tcp_socket, SIGNAL(disconnected()),
                this, SLOT(onClientDisconnected()));
    }

    inline QTcpSocket* getTcpScoket() {
        return this->m_tcp_socket;
    }
private slots:
    void onClientDisconnected();
signals:
    void disconnected(MTcpSocket *);
};

#endif // MTCPSOCKET_H
