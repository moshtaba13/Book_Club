#include "mainwindow.h"
#include "login.h"
#include "home.h"
#include "register.h"
#include "Cart.h"
#include "cartwidget.h"
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QMessageBox>
#include "bookdetailwindow.h"
#include "Book.h"
#include "User.h"
#include "genreselectionwidget.h"
#include "personallibrarywidget.h"
#include "publishermanager.h"
#include "registerpublisher.h"
#include "Admin.h"
#include "adminloginwidget.h"
#include "notificationwidget.h"
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), systemAdmin("admin", "admin123", "N/A")
{
    resize(700,700);
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    move(
        screenGeometry.center().x() - width() / 2,
        screenGeometry.center().y() - height() / 2
        );

    userManager = new UserManager();

    publisherManager = new PublisherManager();

    stack = new QStackedWidget(this);
    mainCart = &mainCartData;

    RegisterPage = new Register(userManager, publisherManager, this);
    RegisterPublisherPage = new RegisterPublisher(publisherManager, userManager, this);
    PublisherDashboardPage = new PublisherDashboardWidget(publisherManager, this);

    AdminLoginPage = new AdminLoginWidget(this);
    AdminLoginPage->setAdminCredentials(systemAdmin.getUsername(), systemAdmin.getPassword());
    AdminPanelPage = new AdminPanelWidget(userManager, publisherManager, this);



    LoginPage = new login(userManager,publisherManager, this);
    HomePage = new home(this);

    CartPage = new CartWidget(mainCart, this);
    GenreSelectionPage = new GenreSelectionWidget(this);
    ProfilePage = new ProfileWidget(userManager, this);
    LibraryPage = new PersonalLibraryWidget(this);
    NotificationPage = new NotificationWidget(this);


    stack->addWidget(LoginPage);
    stack->addWidget(HomePage);
    stack->addWidget(RegisterPage);
    stack->addWidget(CartPage);
    stack->addWidget(GenreSelectionPage);
    stack->addWidget(ProfilePage);
    stack->addWidget(LibraryPage);
    stack->addWidget(RegisterPublisherPage);
    stack->addWidget(AdminLoginPage);
    stack->addWidget(NotificationPage);
    stack->addWidget(PublisherDashboardPage);
    stack->addWidget(AdminPanelPage);

    connect(LoginPage, &login::SignInSuccess, this, [this](User user) {
        currentUser = user;

        if (currentUser.isFirstLogin()) {
            stack->setCurrentWidget(GenreSelectionPage);
            return;
        }

        loadHomePageContent();

        stack->setCurrentWidget(HomePage);
    });

    connect(LoginPage, &login::GoToSignUp, this, [this]() {
        stack->setCurrentWidget(RegisterPage);
    });

    connect(RegisterPage, &Register::goToLogin, this, [this]() {
        stack->setCurrentWidget(LoginPage);
    });

    connect(RegisterPage, &Register::SignUpSuccess, this, [this]() {
        stack->setCurrentWidget(LoginPage);
    });


    connect(GenreSelectionPage, &GenreSelectionWidget::genresSelected, this, [this](QVector<genre> genres) {
        currentUser.setFavoriteGenres(genres);
        currentUser.setFirstLogin(false);
        userManager->updateUser(currentUser);

        loadHomePageContent();

        stack->setCurrentWidget(HomePage);
    });

    connect(HomePage, &home::cartRequested, this, [this]() {
        CartPage->updateUI();
        stack->setCurrentWidget(CartPage);
    });

    connect(CartPage, &CartWidget::backToHomeRequested, this, [this]() {
        stack->setCurrentWidget(HomePage);
    });

    connect(CartPage, &CartWidget::checkoutSuccessful, this, [this](const QVector<Book> &purchasedBooks) {
        for (const Book &book : purchasedBooks) {
            if (!currentUser.hasPurchasedBook(book.getId())) {
                currentUser.addPurchasedBook(Purchase(book, book.getFinalPrice(), QDateTime::currentDateTime()));
            }
        }
        userManager->updateUser(currentUser);

        QMessageBox::information(this, "Success", "Thank you for your purchase! Books added to your library.");
        stack->setCurrentWidget(HomePage);
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(stack);

    BookDetailPage = new BookDetailWidget(mainCart, &currentUser, this);
    stack->addWidget(BookDetailPage);

    connect(HomePage, &home::bookClicked, this, [this](const Book &book) {
        BookDetailPage->loadBook(book);
        stack->setCurrentWidget(BookDetailPage);
    });

    connect(BookDetailPage, &BookDetailWidget::backToHomeRequested, this, [this]() {
        stack->setCurrentWidget(HomePage);
    });

    stack->setCurrentWidget(LoginPage);
    ForgotPasswordPage = new ForgotPasswordWidget(userManager, this);
    stack->addWidget(ForgotPasswordPage);

    connect(LoginPage, &login::ForgotPasswordRequested, this, [this]() {
        stack->setCurrentWidget(ForgotPasswordPage);
    });
    connect(ForgotPasswordPage, &ForgotPasswordWidget::backToLoginRequested, this, [this]() {
        stack->setCurrentWidget(LoginPage);
    });
    connect(HomePage, &home::profileRequested, this, [this]() {
        ProfilePage->loadUser(currentUser);
        stack->setCurrentWidget(ProfilePage);
    });

    connect(ProfilePage, &ProfileWidget::backToHomeRequested, this, [this]() {
        stack->setCurrentWidget(HomePage);
    });

    connect(ProfilePage, &ProfileWidget::userUpdated, this, [this](User updatedUser) {
        currentUser = updatedUser;
        userManager->updateUser(currentUser);
    });
    connect(HomePage, &home::libraryRequested, this, [this]() {
        LibraryPage->loadUser(currentUser);
        stack->setCurrentWidget(LibraryPage);
    });

    connect(HomePage, &home::searchRequested, this, [this](const QString &query) {
        QVector<Book> results;
        for (const Book &book : allBooks) {
            if (book.getTitle().contains(query, Qt::CaseInsensitive) ||
                book.getAuthor().contains(query, Qt::CaseInsensitive) ||
                book.getPublisherUsername().contains(query, Qt::CaseInsensitive)) {
                results.append(book);
            }
        }
        HomePage->showSearchResults(results, query);
    });

    connect(LibraryPage, &PersonalLibraryWidget::backToHomeRequested, this, [this]() {
        stack->setCurrentWidget(HomePage);
    });

    connect(LibraryPage, &PersonalLibraryWidget::userUpdated, this, [this](User updatedUser) {
        currentUser = updatedUser;
        userManager->updateUser(currentUser);
    });
    connect(LoginPage, &login::GoToSignUpPublisher, this, [this]() {
        stack->setCurrentWidget(RegisterPublisherPage);
    });

    connect(RegisterPublisherPage, &RegisterPublisher::goToLogin, this, [this]() {
        stack->setCurrentWidget(LoginPage);
    });

    connect(RegisterPublisherPage, &RegisterPublisher::SignUpSuccess, this, [this]() {
        stack->setCurrentWidget(LoginPage);
    });


    connect(LoginPage, &login::EnterAsAdminRequested, this, [this]() {
        stack->setCurrentWidget(AdminLoginPage);
    });

    connect(AdminLoginPage, &AdminLoginWidget::backToLoginRequested, this, [this]() {
        stack->setCurrentWidget(LoginPage);
    });

    connect(AdminLoginPage, &AdminLoginWidget::adminSignInSuccess, this, [this]() {
        AdminPanelPage->refreshAll();
        stack->setCurrentWidget(AdminPanelPage);
    });

    connect(HomePage, &home::notificationsRequested, this, [this]() {
        NotificationPage->loadUser(currentUser);
        stack->setCurrentWidget(NotificationPage);
    });

    connect(NotificationPage, &NotificationWidget::backToHomeRequested, this, [this]() {
        stack->setCurrentWidget(HomePage);
    });

    connect(NotificationPage, &NotificationWidget::userUpdated, this, [this](User updatedUser) {
        currentUser = updatedUser;
        userManager->updateUser(currentUser);
    });

    connect(LoginPage, &login::PublisherSignInSuccess, this, [this](Publisher publisher) {
        PublisherDashboardPage->loadPublisher(publisher);
        stack->setCurrentWidget(PublisherDashboardPage);
    });

    connect(PublisherDashboardPage, &PublisherDashboardWidget::backToLoginRequested, this, [this]() {
        stack->setCurrentWidget(LoginPage);
    });

    connect(PublisherDashboardPage, &PublisherDashboardWidget::publisherUpdated, this, [this](Publisher updated) {
        publisherManager->updatePublisher(updated);
    });

    connect(PublisherDashboardPage, &PublisherDashboardWidget::catalogChanged, this, [this]() {
        // به‌روزرسانی allBooks از همه ناشران — فعلاً ساده: فقط دوباره صفحه اصلی رو لود کن اگه باز باشه
        loadHomePageContent();
    });

    connect(PublisherDashboardPage, &PublisherDashboardWidget::bookPublished, this, [this](Book newBook) {
        QVector<User> allUsers = userManager->getAllUsers();
        for (User u : allUsers) {
            if (u.getfavoriteGenres().contains(newBook.getGenre())) {
                Notification n(QString("📚 New book in your favorite genre: %1").arg(newBook.getTitle()));
                u.addNotification(n);
                userManager->updateUser(u);

                if (u.getId() == currentUser.getId()) {
                    currentUser = u;
                }
            }
        }
    });

    connect(ProfilePage, &ProfileWidget::logoutRequested, this, [this]() {
        currentUser = User();
        stack->setCurrentWidget(LoginPage);
    });

    connect(AdminPanelPage, &AdminPanelWidget::logoutRequested, this, [this]() {
        stack->setCurrentWidget(LoginPage);
    });

    connect(AdminPanelPage, &AdminPanelWidget::catalogChanged, this, [this]() {
        loadHomePageContent();
    });
}
void MainWindow::loadHomePageContent()
{
    allBooks = publisherManager->getAllActiveBooks();

    if (allBooks.isEmpty()) {
        allBooks.append(Book("Fantasy Best", "Author A", genre::Fiction, 15.0));
        allBooks.append(Book("Mystery Case", "Author B", genre::Mystery, 12.0));
        allBooks.append(Book("World History", "Author C", genre::History, 20.0));
        allBooks.append(Book("Love Story", "Author D", genre::Romance, 10.0));
    }

    // ۱. Recommended: کتاب‌های ژانر مورد علاقه، مرتب بر اساس محبوبیت
    QVector<Book> recommended;
    for (const Book &book : allBooks) {
        if (currentUser.getfavoriteGenres().contains(book.getGenre())) {
            recommended.append(book);
        }
    }
    std::sort(recommended.begin(), recommended.end(), [](const Book &a, const Book &b) {
        if (a.getAverageRating() != b.getAverageRating())
            return a.getAverageRating() > b.getAverageRating();
        return a.getSalesCount() > b.getSalesCount();
    });

    // ۲. Featured: بالاترین امتیاز کل فروشگاه
    QVector<Book> featured = allBooks;
    std::sort(featured.begin(), featured.end(), [](const Book &a, const Book &b) {
        return a.getAverageRating() > b.getAverageRating();
    });

    // ۳. New Releases: جدیدترین‌ها
    QVector<Book> newReleases = allBooks;
    std::sort(newReleases.begin(), newReleases.end(), [](const Book &a, const Book &b) {
        return a.getPublishDate() > b.getPublishDate();
    });

    // ۴. Best Sellers: بیشترین فروش
    QVector<Book> bestSellers = allBooks;
    std::sort(bestSellers.begin(), bestSellers.end(), [](const Book &a, const Book &b) {
        return a.getSalesCount() > b.getSalesCount();
    });

    // ۵. Free Books: قیمت نهایی صفر
    QVector<Book> freeBooks;
    for (const Book &book : allBooks) {
        if (book.getFinalPrice() <= 0.0) {
            freeBooks.append(book);
        }
    }

    HomePage->loadRecommendedBooks(recommended);
    HomePage->loadGenreBooks(allBooks);
    HomePage->loadFeaturedBooks(featured);
    HomePage->loadNewReleases(newReleases);
    HomePage->loadBestSellers(bestSellers);
    HomePage->loadFreeBooks(freeBooks);
}