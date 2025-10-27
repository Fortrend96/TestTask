#pragma once
#include <QVector>
#include <QString>

class INetworkClient {
public:
    virtual ~INetworkClient() = default;
    virtual void connectToHost(const QString& host, quint16 port) = 0;
    virtual void disconnectFromHost() = 0;
    virtual bool isConnected() const = 0;
    virtual void sendAdd(const QVector<int>& values) = 0;
    virtual void sendRequest() = 0;
    virtual void sendClear() = 0;
};
