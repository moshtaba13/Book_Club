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
#include "adminloginwidget.h"
#include "notificationwidget.h"
#include "publisherdashboardwidget.h"
#include "publishermanager.h"
#include "adminpanelwidget.h"
#include "seeallbookswidget.h"
#include "pdfreaderwidget.h"

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
    AdminLoginWidget *AdminLoginPage;
    NotificationWidget *NotificationPage;
    AdminPanelWidget *AdminPanelPage;
    SeeAllBooksWidget *SeeAllPage;
    PdfReaderWidget *ReaderPage;
    // Whichever page (BookDetailPage or LibraryPage) opened the reader, so
    // its "Back" button can return there instead of always going Home.
    QWidget *readerReturnPage;
    // Whichever page (HomePage or PublisherDashboardPage) opened
    // notifications, so its "Back" button returns there instead of always
    // going Home.
    QWidget *notificationsReturnPage;
};
#endif // MAINWINDOW_H