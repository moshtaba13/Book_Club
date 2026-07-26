#ifndef ADMINPANELWIDGET_H
#define ADMINPANELWIDGET_H
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "UserManager.h"
#include "PublisherManager.h"
#include "Book.h"

class AdminPanelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdminPanelWidget(UserManager *userManager, PublisherManager *publisherManager, QWidget *parent = nullptr);
    void refreshAll();

signals:
    void logoutRequested();
    void catalogChanged();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onSearchTextChanged(const QString &text);


private:
    void refreshUsersList();
    void refreshPublishersList();
    void refreshBooksList();
    QVBoxLayout *booksListLayout;

    UserManager *userManager;
    PublisherManager *publisherManager;

    QLineEdit *leSearch;
    QVBoxLayout *usersListLayout;
    QVBoxLayout *publishersListLayout;
};
#endif // ADMINPANELWIDGET_H