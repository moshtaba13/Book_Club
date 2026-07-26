#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "User.h"

class NotificationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationWidget(QWidget *parent = nullptr);
    void loadUser(User &user);

signals:
    void backToHomeRequested();
    void userUpdated(const User &updatedUser);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onMarkAllReadClicked();
    void onBackClicked();

private:
    void refreshNotifications();

    User *currentUser;

    QPushButton *backButton;
    QPushButton *btnMarkAllRead;
    QVBoxLayout *notificationsLayout;
};
#endif // NOTIFICATIONWIDGET_H