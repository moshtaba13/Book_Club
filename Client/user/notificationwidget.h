#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "Notification.h"

class NotificationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationWidget(QWidget *parent = nullptr);
    void refreshFromServer();

signals:
    void backToHomeRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onMarkAllReadClicked();
    void onBackClicked();

private:
    void refreshNotifications();

    QVector<Notification> notifications;

    QPushButton *backButton;
    QPushButton *btnMarkAllRead;
    QVBoxLayout *notificationsLayout;
};
#endif // NOTIFICATIONWIDGET_H