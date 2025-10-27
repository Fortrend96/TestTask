#pragma once
#include <QObject>
#include <QTcpSocket>
#include "INetworkClient.h"
#include "MessageParser.h"

class TcpClient : public QObject, public INetworkClient {
    Q_OBJECT
public:
    explicit TcpClient(QObject* parent = nullptr);
    ~TcpClient() override;

    void connectToHost(const QString& host, quint16 port) override;
    void disconnectFromHost() override;
    bool isConnected() const override;

    void sendAdd(const QVector<int>& values) override;
    void sendRequest() override;
    void sendClear() override;

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString& msg);
    void ackReceived();
    void dataReceived(const QVector<int>& sorted);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError err);

private:
    QTcpSocket* socket_;
    MessageParser parser_;
};
