#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>
#include <QTableWidget>
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
    void addSystemEvent(const QString &event);

    void addClientToTable(int clientId);
    void removeClientFromTable(int clientId);

    double getCpuUsage();
    double getMemoryUsage();
    void updateHealthStatus(int onlineClients,
                            double cpu,
                            double memory);

    Ui::ServerWindow *ui;
    QTimer *m_statsTimer;
    bool m_isRunning;
    int m_totalConnections;
    quint64 m_lastIdleTime = 0;
    quint64 m_lastKernelTime = 0;
    quint64 m_lastUserTime = 0;
};

#endif