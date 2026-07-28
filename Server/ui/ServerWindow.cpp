#include "ServerWindow.h"
#include "ui_ServerWindow.h"
#include "../network/Server.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
    , m_isRunning(false)
    , m_totalConnections(0)
{
    ui->setupUi(this);
    setupConnections();
    updateServerStatus(false);

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
    } 
    else {
        addLog("[ERROR] Failed to start server!");
    }
}

void ServerWindow::stopServer() {
    Server::instance().stopServer();
    m_isRunning = false;
    updateServerStatus(false);
    m_statsTimer->stop();
    addLog("[SERVER] Server stopped");
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
            "font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "background-color: #c0392b;"
            "}"
        );
        ui->lblStatus->setText("● Running");
        ui->lblStatus->setStyleSheet(
            "color: #27ae60; font-weight: bold; font-size: 14px;");

    } 
    else {
        ui->btnStartStop->setText("Start Server");
        ui->btnStartStop->setStyleSheet(
            "QPushButton {"
            "background-color: #27ae60;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 8px 16px;"
            "font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "background-color: #2ecc71;"
            "}"
        );
        ui->lblStatus->setText("● Stopped");
        ui->lblStatus->setStyleSheet("color: #e74c3c; font-weight: bold; font-size: 14px;");
        ui->lblOnlineClients->setText("0");
        ui->lblHealth->setText("● Offline");
        ui->lblHealth->setStyleSheet("color: gray;");
    }
}

void ServerWindow::updateStats()
{
    if (!m_isRunning) return;

    int online = Server::instance().connectedClientsCount();
    ui->lblOnlineClients->setText(QString::number(online));

    QString health;
    QString style;

    if (online < 10) {
        health = "● Healthy";
        style  = "color: #27ae60; font-weight: bold;";
    } else if (online < 50) {
        health = "● Busy";
        style  = "color: #f39c12; font-weight: bold;";
    } else {
        health = "● Overloaded";
        style  = "color: #e74c3c; font-weight: bold;";
    }

    ui->lblHealth->setText(health);
    ui->lblHealth->setStyleSheet(style);

    ui->lblTime->setText(
        QDateTime::currentDateTime()
        .toString("yyyy-MM-dd hh:mm:ss"));
}

void ServerWindow::onLogMessage(const QString &message) {addLog(message);}

void ServerWindow::addLog(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss]");
    ui->logTextEdit->append(timestamp + " " + message);
    ui->logTextEdit->verticalScrollBar()->setValue(ui->logTextEdit->verticalScrollBar()->maximum());
}

void ServerWindow::onClientConnected(int clientId)
{
    m_totalConnections++;
    ui->lblTotalConnections->setText(QString::number(m_totalConnections));
    addLog("[CONNECT] Client #" + QString::number(clientId)
           + " connected | Online: "
           + QString::number(Server::instance().connectedClientsCount()));
}

void ServerWindow::onClientDisconnected(int clientId)
{
    addLog("[DISCONNECT] Client #" + QString::number(clientId)
           + " disconnected | Online: "
           + QString::number(Server::instance().connectedClientsCount()));
}