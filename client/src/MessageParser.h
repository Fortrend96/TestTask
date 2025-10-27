#pragma once
#include <QByteArray>
#include <QVector>
#include <QString>
#include <optional>

class MessageParser {
public:
    struct Parsed {
        enum class Type { Ok, Data, Error, Unknown } type;
        QVector<int> data;
        QString errorText;
    };

    static QByteArray packAdd(const QVector<int>& values); // creates "ADD x,y,z\n"
    static QByteArray packRequest(); // "GET\n"
    static QByteArray packClear();   // "CLEAR\n"

    void append(const QByteArray& bytes);
    std::optional<Parsed> nextMessage();

private:
    QByteArray buffer_;
};
