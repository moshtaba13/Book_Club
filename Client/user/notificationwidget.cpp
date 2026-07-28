#include "notificationwidget.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include <QFrame>
#include <QStyleOption>
#include <QPainter>
#include <QJsonArray>
#include "networkclient.h"
#include "modelserializer.h"

NotificationWidget::NotificationWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("notificationPage");
    this->setStyleSheet("QWidget#notificationPage { background-color: #FFF8F2; }");

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // هدر
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setMinimumHeight(75);
    headerWidget->setMaximumHeight(75);
    headerWidget->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-bottom: 1px solid rgba(0,0,0,15);");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(25, 0, 25, 0);

    backButton = new QPushButton("⬅ Back to Club", headerWidget);
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4dbde;"
        "   color: #2C3E50;"
        "   border: none;"
        "   border-radius: 15px;"
        "   padding-left: 15px;"
        "   padding-right: 15px;"
        "   min-height: 35px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    headerLayout->addWidget(backButton);
    headerLayout->addStretch();

    QLabel *lblPageTitle = new QLabel("Notifications", headerWidget);
    lblPageTitle->setFont(QFont("Segoe UI", 18, QFont::Bold));
    lblPageTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    headerLayout->addWidget(lblPageTitle);
    headerLayout->addStretch();

    btnMarkAllRead = new QPushButton("Mark all as read", headerWidget);
    btnMarkAllRead->setCursor(Qt::PointingHandCursor);
    btnMarkAllRead->setStyleSheet("color: #2C3E50; background: transparent; border: none; font-weight: bold; font-size: 12px;");
    headerLayout->addWidget(btnMarkAllRead);

    outerLayout->addWidget(headerWidget);

    // بدنه اسکرول‌شونده
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollArea > QWidget > QWidget { background: transparent; }"
        );

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(12);

    notificationsLayout = new QVBoxLayout();
    notificationsLayout->setSpacing(10);
    mainLayout->addLayout(notificationsLayout);
    mainLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    outerLayout->addWidget(scrollArea);

    connect(backButton, &QPushButton::clicked, this, &NotificationWidget::onBackClicked);
    connect(btnMarkAllRead, &QPushButton::clicked, this, &NotificationWidget::onMarkAllReadClicked);
}

void NotificationWidget::refreshFromServer()
{
    notifications.clear();
    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetNotifications);
    if (response.value("status").toString() == "Success") {
        for (const QJsonValue &v : response.value("data").toObject().value("notifications").toArray())
            notifications.append(ModelSerializer::notificationFromJson(v.toObject()));
    }

    refreshNotifications();
}

void NotificationWidget::refreshNotifications()
{
    QLayoutItem *child;
    while ((child = notificationsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (notifications.isEmpty()) {
        QLabel *lblNone = new QLabel("You have no notifications yet.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        notificationsLayout->addWidget(lblNone);
        return;
    }

    for (const Notification &n : notifications) {
        QFrame *row = new QFrame();
        row->setCursor(Qt::PointingHandCursor);

        if (n.isRead()) {
            row->setStyleSheet("background-color: #F2E8DF; border-radius: 12px;");
        } else {
            row->setStyleSheet("background-color: rgba(255, 192, 159, 195); border-radius: 12px;");
        }

        QVBoxLayout *rowLayout = new QVBoxLayout(row);
        rowLayout->setContentsMargins(15, 12, 15, 12);
        rowLayout->setSpacing(4);

        QLabel *lblMessage = new QLabel(n.getMessage());
        lblMessage->setWordWrap(true);
        lblMessage->setFont(QFont("Segoe UI", 11, n.isRead() ? QFont::Normal : QFont::Bold));
        lblMessage->setStyleSheet("color: #2C3E50; background: transparent;");
        rowLayout->addWidget(lblMessage);

        QLabel *lblTime = new QLabel(n.getCreatedAt().toString("yyyy-MM-dd hh:mm"));
        lblTime->setFont(QFont("Segoe UI", 9));
        lblTime->setStyleSheet("color: #706357; background: transparent;");
        rowLayout->addWidget(lblTime);

        int notifId = n.getId();
        row->installEventFilter(this);

        // کلیک روی کل کارت، پیام رو خوانده‌شده می‌کنه
        QPushButton *invisibleClickCatcher = new QPushButton(row);
        invisibleClickCatcher->setStyleSheet("background: transparent; border: none;");
        invisibleClickCatcher->setGeometry(row->rect());
        invisibleClickCatcher->lower();

        connect(invisibleClickCatcher, &QPushButton::clicked, this, [this, notifId]() {
            QJsonObject data;
            data["notification_id"] = notifId;
            NetworkClient::instance().sendRequest(RequestType::MarkNotificationRead, data);
            refreshFromServer();
        });

        notificationsLayout->addWidget(row);
    }
}

void NotificationWidget::onMarkAllReadClicked()
{
    NetworkClient::instance().sendRequest(RequestType::MarkAllNotificationsRead);
    refreshFromServer();
}

void NotificationWidget::onBackClicked()
{
    emit backToHomeRequested();
}

void NotificationWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}