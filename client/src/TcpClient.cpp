#include "TcpClient.h"
#include <QHostAddress>
#include <QDebug>

TcpClient::TcpClient(QObject* parent)
    : QObject(parent), socket_(new QTcpSocket(this)) {
    connect(socket_, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(socket_, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(socket_, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &TcpClient::onSocketError);
}

TcpClient::~TcpClient() {
    if (socket_->state() == QAbstractSocket::ConnectedState) socket_->disconnectFromHost();
}

void TcpClient::connectToHost(const QString& host, quint16 port) {
    socket_->connectToHost(host, port);
}

void TcpClient::disconnectFromHost() {
    socket_->disconnectFromHost();
}

bool TcpClient::isConnected() const {
    return socket_->state() == QAbstractSocket::ConnectedState;
}

void TcpClient::sendAdd(const QVector<int>& values) {
    if (!isConnected()) { emit errorOccurred("Not connected"); return; }
    socket_->write(MessageParser::packAdd(values));
}

void TcpClient::sendRequest() {
    if (!isConnected()) { emit errorOccurred("Not connected"); return; }
    socket_->write(MessageParser::packRequest());
}

void TcpClient::sendClear() {
    if (!isConnected()) { emit errorOccurred("Not connected"); return; }
    socket_->write(MessageParser::packClear());
}

void TcpClient::onConnected() { emit connected(); }
void TcpClient::onDisconnected() { emit disconnected(); }

void TcpClient::onSocketError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    emit errorOccurred(socket_->errorString());
}

void TcpClient::onReadyRead() {
    QByteArray chunk = socket_->readAll();
    parser_.append(chunk);
    while (true) {
        auto maybe = parser_.nextMessage();
        if (!maybe.has_value()) break;
        auto msg = maybe.value();
        switch (msg.type) {
        case MessageParser::Parsed::Type::Ok:
            emit ackReceived();
            break;
        case MessageParser::Parsed::Type::Data:
            emit dataReceived(msg.data);
            break;
        case MessageParser::Parsed::Type::Error:
            emit errorOccurred(msg.errorText);
            break;
        case MessageParser::Parsed::Type::Unknown:
            emit errorOccurred("Unknown response from server");
            break;
        }
    }
}
