#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TcpClient.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), client_(new TcpClient(this)) {
    ui->setupUi(this);

    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(ui->btnAddToQueue, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->btnRequestQueue, &QPushButton::clicked, this, &MainWindow::onRequestClicked);
    connect(ui->btnClearQueue, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    connect(client_, &TcpClient::connected, this, &MainWindow::onConnected);
    connect(client_, &TcpClient::disconnected, this, &MainWindow::onDisconnected);
    connect(client_, &TcpClient::ackReceived, this, &MainWindow::onAck);
    connect(client_, &TcpClient::dataReceived, this, &MainWindow::onDataReceived);
    connect(client_, &TcpClient::errorOccurred, this, &MainWindow::onNetworkError);

    setConnectedState(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onConnectClicked() {
    if (client_->isConnected()) {
        client_->disconnectFromHost();
    } else {
        QString ip = ui->lineEditIP->text().trimmed();
        bool ok;
        quint16 port = static_cast<quint16>(ui->lineEditPort->text().toUShort(&ok));
        if (!ok) { QMessageBox::warning(this, "Ошибка", "Неверный порт"); return; }
        client_->connectToHost(ip, port);
    }
}

void MainWindow::onAddClicked() {
    bool ok;
    auto arr = parseInputArray(ui->lineEditInput->text(), ok);
    if (!ok) { QMessageBox::warning(this, "Неверный ввод", "Введите массив чисел через запятую"); return; }
    client_->sendAdd(arr);
}

void MainWindow::onRequestClicked() {
    client_->sendRequest();
}

void MainWindow::onClearClicked() {
    client_->sendClear();
}

void MainWindow::onConnected() {
    setConnectedState(true);
    ui->textEditOutput->append("Connected");
}

void MainWindow::onDisconnected() {
    setConnectedState(false);
    ui->textEditOutput->append("Disconnected");
}

void MainWindow::onAck() {
    ui->textEditOutput->append("OK");
}

void MainWindow::onDataReceived(const QVector<int>& data) {
    ui->textEditOutput->append(joinArray(data));
}

void MainWindow::onNetworkError(const QString& msg) {
    ui->textEditOutput->append(QString("Error: %1").arg(msg));
}

QVector<int> MainWindow::parseInputArray(const QString& s, bool& ok) const {
    ok = true;
    QVector<int> out;
    const auto parts = s.split(',', Qt::SkipEmptyParts);
    for (const QString& p : parts) {
        bool lok = false;
        int v = p.trimmed().toInt(&lok);
        if (!lok) { ok = false; return {}; }
        out.append(v);
    }
    return out;
}

QString MainWindow::joinArray(const QVector<int>& arr) const {
    QStringList ls;
    for (int v : arr) ls << QString::number(v);
    return ls.join(", ");
}

void MainWindow::setConnectedState(bool on) {
    ui->btnConnect->setText(on ? "Отключиться" : "Подключиться");
    ui->btnAddToQueue->setEnabled(on);
    ui->btnRequestQueue->setEnabled(on);
    ui->btnClearQueue->setEnabled(on);
    ui->lineEditIP->setEnabled(!on);
    ui->lineEditPort->setEnabled(!on);
}
