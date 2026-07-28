#ifndef PUBLISHERDASHBOARDWIDGET_H
#define PUBLISHERDASHBOARDWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "Publisher.h"
#include "publishermanager.h"

class PublisherDashboardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PublisherDashboardWidget(PublisherManager *manager, QWidget *parent = nullptr);
    void loadPublisher(const Publisher &publisher);

signals:
    void backToLoginRequested();
    void catalogChanged();
    void notificationsRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onSaveInfoClicked();
    void onAddBookClicked();
    void onChangePasswordClicked();
    void onLogoutClicked();

private:
    QString genreToString(genre g) const;
    void refreshBookList();
    void refreshStats();

    PublisherManager *publisherManager;
    Publisher currentPublisher;
    QVector<Book> publishedBooks;

    QPushButton *logoutButton;
    QPushButton *btnNotification;


    QLineEdit *leUsername;

    QPushButton *btnSaveInfo;

    QLineEdit *leCurrentPassword;
    QLineEdit *leNewPassword;
    QLineEdit *leConfirmPassword;
    QPushButton *btnChangePassword;


    QVBoxLayout *bookListLayout;
    QPushButton *btnAddBook;


    QLabel *lblTotalBooks;
    QLabel *lblTotalRevenue;
    QVBoxLayout *bestSellersLayout;
    QVBoxLayout *worstSellersLayout;
};
#endif // PUBLISHERDASHBOARDWIDGET_H