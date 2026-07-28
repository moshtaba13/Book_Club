#include "ServerWindow.h"
#include "ui_ServerWindow.h"
#include "../network/Server.h"
#include <QDateTime>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
    , m_isRunning(false)
    , m_totalConnections(0)
{
    ui->setupUi(this);
    setupConnections();
    updateServerStatus(false);

    ui->tableClients->setColumnCount(3);
    ui->tableClients->setHorizontalHeaderLabels({"Client ID", "Connected At", "Status"});
    ui->tableClients->horizontalHeader()
        ->setStretchLastSection(true);
    ui->tableClients->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_statsTimer = new QTimer(this);
    connect(m_statsTimer, &QTimer::timeout,
            this, &ServerWindow::updateStats);
}

ServerWindow::~ServerWindow() {
    if (m_isRunning)
        stopServer();
    delete ui;
}

void ServerWindow::setupConnections()
{
    connect(ui->btnStartStop, &QPushButton::clicked,
            this, &ServerWindow::onStartStopClicked);

    connect(&Server::instance(), &Server::logMessage,
            this, &ServerWindow::onLogMessage);
    connect(&Server::instance(), &Server::clientConnected,
            this, &ServerWindow::onClientConnected);
    connect(&Server::instance(), &Server::clientDisconnected,
            this, &ServerWindow::onClientDisconnected);
}

void ServerWindow::onStartStopClicked() {
    if (m_isRunning)
        stopServer();
    else
        startServer();
}

void ServerWindow::startServer() {
    if (Server::instance().startServer(1234)) {
        m_isRunning = true;
        updateServerStatus(true);
        m_statsTimer->start(2000);
        addLog("[SERVER] Server started on port 1234");
        addSystemEvent("Server started");

        updateStats();
    } 
    else {
        addLog("[ERROR] Failed to start server!");
        addSystemEvent("Server failed to start");
    }
}

void ServerWindow::stopServer() {
    Server::instance().stopServer();
    m_isRunning = false;
    updateServerStatus(false);
    m_statsTimer->stop();
    ui->tableClients->setRowCount(0);
    addLog("[SERVER] Server stopped");
    addSystemEvent("Server stopped");
}

void ServerWindow::updateServerStatus(bool isRunning)
{
    if (isRunning) {
        ui->btnStartStop->setText("Stop Server");
        ui->btnStartStop->setStyleSheet(
            "QPushButton {"
            "background-color: #e74c3c;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 8px 16px;"
            "font-weight: bold; }"
            "QPushButton:hover {"
            "background-color: #c0392b; }"
            );
        ui->lblStatus->setText("● Running");
        ui->lblStatus->setStyleSheet("color: #27ae60; font-weight: bold;");
    } 
    else {
        ui->btnStartStop->setText("Start Server");
        ui->btnStartStop->setStyleSheet(
            "QPushButton {"
            "background-color: #27ae60;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 8px 16px;"
            "font-weight: bold; }"
            "QPushButton:hover {"
            "background-color: #2ecc71; }"
            );
        ui->lblStatus->setText("● Stopped");
        ui->lblStatus->setStyleSheet(
            "color: #e74c3c; font-weight: bold;");

        ui->lblOnlineClients->setText("Online: 0");
        ui->lblTotalConnections->setText(QString("Connections: %1").arg(m_totalConnections));

        ui->lblHealth->setText("● Offline");
        ui->lblHealth->setStyleSheet("color: gray;");
        ui->lblCpu->setText("CPU: N/A");
        ui->lblMemory->setText("RAM: N/A");
    }
}

void ServerWindow::updateStats()
{
    if (!m_isRunning) return;

    int online    = Server::instance().connectedClientsCount();
    double cpu    = getCpuUsage();
    double memory = getMemoryUsage();

    ui->lblOnlineClients->setText(QString("Online: %1").arg(online));

    ui->lblCpu->setText(QString("CPU: %1%").arg(cpu, 0, 'f', 1));
    ui->lblMemory->setText(QString("RAM: %1%").arg(memory, 0, 'f', 1));
    ui->lblTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    updateHealthStatus(online, cpu, memory);
}

void ServerWindow::updateHealthStatus(int onlineClients, double cpu, double memory)
{
    QString health;
    QString style;

    if (onlineClients < 10 && cpu < 80.0 && memory < 90.0) {
        health = "● Healthy";
        style  = "color: #27ae60; font-weight: bold;";
    } 
    else if (onlineClients < 50 && cpu < 90.0 && memory < 95.0) {
        health = "● Busy";
        style  = "color: #f39c12; font-weight: bold;";
    } 
    else {
        health = "● Overloaded";
        style  = "color: #e74c3c; font-weight: bold;";
    }

    ui->lblHealth->setText(health);
    ui->lblHealth->setStyleSheet(style);
}

void ServerWindow::onLogMessage(const QString &message) {addLog(message);}

void ServerWindow::addLog(const QString &message) {
    QString timestamp = QDateTime::currentDateTime()
    .toString("[hh:mm:ss]");
    ui->logTextEdit->append(timestamp + " " + message);
    ui->logTextEdit->verticalScrollBar()->setValue(
        ui->logTextEdit->verticalScrollBar()->maximum());
}

void ServerWindow::addSystemEvent(const QString &event)
{
    QString timestamp = QDateTime::currentDateTime()
    .toString("[hh:mm:ss]");
    ui->eventsTextEdit->append(timestamp + " " + event);
    ui->eventsTextEdit->verticalScrollBar()->setValue(
        ui->eventsTextEdit->verticalScrollBar()->maximum());
}


void ServerWindow::onClientConnected(int clientId)
{
    m_totalConnections++;

    ui->lblTotalConnections->setText(QString("Connections: %1").arg(m_totalConnections));

    addClientToTable(clientId);
    addLog("[CONNECT] Client #" + QString::number(clientId) + " connected");
    addSystemEvent("Client #" + QString::number(clientId) + " connected");
}

void ServerWindow::onClientDisconnected(int clientId)
{
    removeClientFromTable(clientId);
    addLog("[DISCONNECT] Client #" + QString::number(clientId) + " disconnected");
    addSystemEvent("Client #" + QString::number(clientId) + " disconnected");
}

void ServerWindow::addClientToTable(int clientId)
{
    int row = ui->tableClients->rowCount();
    ui->tableClients->insertRow(row);

    ui->tableClients->setItem(row, 0, new QTableWidgetItem(QString::number(clientId)));
    ui->tableClients->setItem(row, 1, new QTableWidgetItem(QDateTime::currentDateTime().toString("hh:mm:ss")));
    ui->tableClients->setItem(row, 2, new QTableWidgetItem("Connected"));

    ui->tableClients->item(row, 2)->setForeground(QColor("#27ae60"));
}

void ServerWindow::removeClientFromTable(int clientId)
{
    for (int i = 0; i < ui->tableClients->rowCount(); ++i) {
        if (ui->tableClients->item(i, 0)->text() == QString::number(clientId)) {
            ui->tableClients->removeRow(i);
            break;
        }
    }
}


double ServerWindow::getCpuUsage()
{
#ifdef Q_OS_WIN
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        quint64 idle = (static_cast<quint64>(idleTime.dwHighDateTime) << 32) | idleTime.dwLowDateTime;
        quint64 kernel = (static_cast<quint64>(kernelTime.dwHighDateTime) << 32) | kernelTime.dwLowDateTime;
        quint64 user = (static_cast<quint64>(userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime;

        if (m_lastIdleTime != 0) {
            quint64 idleDiff = idle - m_lastIdleTime;
            quint64 kernelDiff = kernel - m_lastKernelTime;
            quint64 userDiff = user - m_lastUserTime;
            quint64 sysTotal = kernelDiff + userDiff;

            double cpu = 0.0;
            if (sysTotal > 0) {
                cpu = static_cast<double>(sysTotal - idleDiff) * 100.0 / sysTotal;
            }

            m_lastIdleTime = idle;
            m_lastKernelTime = kernel;
            m_lastUserTime = user;
            return cpu;
        }

        m_lastIdleTime = idle;
        m_lastKernelTime = kernel;
        m_lastUserTime = user;
    }
#endif
    return 0.0;
}

double ServerWindow::getMemoryUsage()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return static_cast<double>(memInfo.dwMemoryLoad);
    }
#endif
    return 0.0;
}