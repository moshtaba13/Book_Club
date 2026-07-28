#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QScrollBar>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void onStartStopClicked();
    void updateStats();
    void onLogMessage(const QString &message);
    void onClientConnected(int clientId);
    void onClientDisconnected(int clientId);

private:
    void setupConnections();
    void startServer();
    void stopServer();
    void updateServerStatus(bool isRunning);
    void addLog(const QString &message);

    Ui::ServerWindow *ui;
    QTimer *m_statsTimer;
    bool m_isRunning;
    int m_totalConnections;
};

#endif