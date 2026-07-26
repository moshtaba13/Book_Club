#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QStackedWidget>
#include "login.h"
#include "home.h"
#include "register.h"
#include "cartwidget.h"
#include "Cart.h"
#include "bookdetailwindow.h"
#include "UserManager.h"
#include "User.h"
#include "genreselectionwidget.h"
#include "forgotpasswordwidget.h"
#include "profilewidget.h"
#include "personallibrarywidget.h"
#include "registerpublisher.h"
#include "Admin.h"
#include "adminloginwidget.h"
#include "notificationwidget.h"
#include "publisherdashboardwidget.h"
#include "PublisherManager.h"
#include "adminpanelwidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    QStackedWidget *stack;
    login *LoginPage;
    home *HomePage;
    Register *RegisterPage;
    Cart mainCartData;

    CartWidget *CartPage;
    Cart *mainCart;
    BookDetailWidget *BookDetailPage;
    GenreSelectionWidget *GenreSelectionPage;
    UserManager *userManager;
    User currentUser;
    ForgotPasswordWidget *ForgotPasswordPage;
    ProfileWidget *ProfilePage;
    PersonalLibraryWidget *LibraryPage;
    QVector<Book> allBooks;
    void loadHomePageContent();
    PublisherManager *publisherManager;
    PublisherDashboardWidget *PublisherDashboardPage;
    RegisterPublisher *RegisterPublisherPage;
    Admin systemAdmin;
    AdminLoginWidget *AdminLoginPage;
    NotificationWidget *NotificationPage;
    AdminPanelWidget *AdminPanelPage;
};
#endif // MAINWINDOW_H