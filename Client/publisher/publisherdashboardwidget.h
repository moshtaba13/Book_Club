#ifndef PUBLISHERDASHBOARDWIDGET_H
#define PUBLISHERDASHBOARDWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "Publisher.h"
#include "PublisherManager.h"

class PublisherDashboardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PublisherDashboardWidget(PublisherManager *manager, QWidget *parent = nullptr);
    void loadPublisher(const Publisher &publisher);

signals:
    void backToLoginRequested();
    void publisherUpdated(const Publisher &updatedPublisher);
    void catalogChanged();
    void bookPublished(const Book &newBook);

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

    QPushButton *logoutButton;

    // اطلاعات حساب
    QLineEdit *leUsername;

    QPushButton *btnSaveInfo;

    QLineEdit *leCurrentPassword;
    QLineEdit *leNewPassword;
    QLineEdit *leConfirmPassword;
    QPushButton *btnChangePassword;

    // لیست کتاب‌ها
    QVBoxLayout *bookListLayout;
    QPushButton *btnAddBook;

    // آمار
    QLabel *lblTotalBooks;
    QLabel *lblTotalRevenue;
    QVBoxLayout *bestSellersLayout;
    QVBoxLayout *worstSellersLayout;
};
#endif // PUBLISHERDASHBOARDWIDGET_H