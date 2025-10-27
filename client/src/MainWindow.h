#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TcpClient;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onConnectClicked();
    void onAddClicked();
    void onRequestClicked();
    void onClearClicked();

    void onConnected();
    void onDisconnected();
    void onAck();
    void onDataReceived(const QVector<int>& data);
    void onNetworkError(const QString& msg);

private:
    Ui::MainWindow* ui;
    TcpClient* client_;

    QVector<int> parseInputArray(const QString& s, bool& ok) const;
    QString joinArray(const QVector<int>& arr) const;
    void setConnectedState(bool on);
};
