#include "MessageParser.h"

QByteArray MessageParser::packAdd(const QVector<int>& values) {
    QByteArray out;
    out.append("ADD ");
    bool first = true;
    for (int v : values) {
        if (!first) out.append(",");
        first = false;
        out.append(QByteArray::number(v));
    }
    out.append("\n");
    return out;
}

QByteArray MessageParser::packRequest() {
    return QByteArray("GET\n");
}

QByteArray MessageParser::packClear() {
    return QByteArray("CLEAR\n");
}

void MessageParser::append(const QByteArray& bytes) {
    buffer_.append(bytes);
}

std::optional<MessageParser::Parsed> MessageParser::nextMessage() {
    if (buffer_.isEmpty()) return std::nullopt;
    int pos = buffer_.indexOf('\n');
    if (pos < 0) return std::nullopt;
    QByteArray line = buffer_.left(pos);
    buffer_.remove(0, pos + 1);
    QString s = QString::fromUtf8(line).trimmed();
    if (s == "OK") {
        return Parsed{Parsed::Type::Ok, {}, {}};
    } else if (s.startsWith("DATA ")) {
        QString rest = s.mid(5);
        QVector<int> v;
        for (const QString& token : rest.split(',', Qt::SkipEmptyParts)) {
            bool ok = false;
            int val = token.trimmed().toInt(&ok);
            if (ok) v.push_back(val);
        }
        return Parsed{Parsed::Type::Data, v, {}};
    } else if (s.startsWith("ERR ")) {
        QString msg = s.mid(4);
        return Parsed{Parsed::Type::Error, {}, msg};
    } else {
        return Parsed{Parsed::Type::Unknown, {}, {}};
    }
}
