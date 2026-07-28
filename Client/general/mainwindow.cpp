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
#include "adminloginwidget.h"
#include "notificationwidget.h"
#include "networkclient.h"
#include "protocol.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(700,700);
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    move(
        screenGeometry.center().x() - width() / 2,
        screenGeometry.center().y() - height() / 2
        );

    // Defaults to 127.0.0.1:1234 (server on the same machine); set
    // BOOKCLUB_SERVER_HOST / BOOKCLUB_SERVER_PORT to point this client at a
    // server running on a different machine (e.g. testing a Windows client
    // against a Linux server, or vice versa).
    QString serverHost = qEnvironmentVariable("BOOKCLUB_SERVER_HOST", "127.0.0.1");
    quint16 serverPort = static_cast<quint16>(qEnvironmentVariableIntValue("BOOKCLUB_SERVER_PORT") == 0
                                                  ? 1234
                                                  : qEnvironmentVariableIntValue("BOOKCLUB_SERVER_PORT"));

    if (!NetworkClient::instance().connectToServer(serverHost, serverPort)) {
        QMessageBox::warning(this, "Connection Error",
                             QString("Could not connect to the BookClub server at %1:%2. Some features will be unavailable until the connection succeeds.")
                                 .arg(serverHost).arg(serverPort));
    }

    userManager = new UserManager();

    publisherManager = new PublisherManager();

    stack = new QStackedWidget(this);
    mainCart = &mainCartData;

    RegisterPage = new Register(userManager, this);
    RegisterPublisherPage = new RegisterPublisher(publisherManager, this);
    PublisherDashboardPage = new PublisherDashboardWidget(publisherManager, this);

    AdminLoginPage = new AdminLoginWidget(this);
    AdminPanelPage = new AdminPanelWidget(userManager, publisherManager, this);



    LoginPage = new login(this);
    HomePage = new home(this);

    CartPage = new CartWidget(mainCart, this);
    GenreSelectionPage = new GenreSelectionWidget(this);
    ProfilePage = new ProfileWidget(userManager, this);
    LibraryPage = new PersonalLibraryWidget(this);
    NotificationPage = new NotificationWidget(this);
    SeeAllPage = new SeeAllBooksWidget(this);
    ReaderPage = new PdfReaderWidget(this);
    readerReturnPage = nullptr;
    notificationsReturnPage = nullptr;


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
    stack->addWidget(SeeAllPage);
    stack->addWidget(ReaderPage);

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
        QString errorMessage;
        if (!userManager->updateFavoriteGenres(genres, errorMessage)) {
            QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not save your genres." : errorMessage);
            return;
        }

        currentUser.setFavoriteGenres(genres);
        currentUser.setFirstLogin(false);

        loadHomePageContent();

        stack->setCurrentWidget(HomePage);
    });

    connect(HomePage, &home::cartRequested, this, [this]() {
        CartPage->refreshFromServer();
        stack->setCurrentWidget(CartPage);
    });

    connect(CartPage, &CartWidget::backToHomeRequested, this, [this]() {
        stack->setCurrentWidget(HomePage);
    });

    connect(CartPage, &CartWidget::checkoutSuccessful, this, [this]() {
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

    connect(BookDetailPage, &BookDetailWidget::readRequested, this, [this](int bookId, const QString &title) {
        readerReturnPage = stack->currentWidget();
        ReaderPage->openBook(bookId, title);
        stack->setCurrentWidget(ReaderPage);
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
    });
    connect(HomePage, &home::libraryRequested, this, [this]() {
        LibraryPage->loadUser(currentUser);
        stack->setCurrentWidget(LibraryPage);
    });

    connect(HomePage, &home::searchRequested, this, [this](const QString &query) {
        QString errorMessage;
        QVector<Book> results = publisherManager->searchBooks(query, errorMessage);
        if (!errorMessage.isEmpty())
            QMessageBox::warning(this, "Search Error", errorMessage);
        HomePage->showSearchResults(results, query);
    });

    connect(HomePage, &home::seeAllRequested, this, [this](const QString &title, const QVector<Book> &books) {
        SeeAllPage->loadBooks(title, books);
        stack->setCurrentWidget(SeeAllPage);
    });

    connect(HomePage, &home::seeAllGenresRequested, this, [this](const QVector<Book> &highlights) {
        SeeAllPage->loadGenreHighlights("Genres", highlights);
        stack->setCurrentWidget(SeeAllPage);
    });

    auto openGenreBooks = [this](genre g) {
        QString errorMessage;
        QVector<Book> books = publisherManager->getBooksByGenre(g, errorMessage);
        if (!errorMessage.isEmpty())
            QMessageBox::warning(this, "Error", errorMessage);
        SeeAllPage->loadBooks(genreDisplayName(g), books);
        stack->setCurrentWidget(SeeAllPage);
    };

    connect(HomePage, &home::genreClicked, this, openGenreBooks);
    connect(SeeAllPage, &SeeAllBooksWidget::genreClicked, this, openGenreBooks);

    connect(SeeAllPage, &SeeAllBooksWidget::backRequested, this, [this]() {
        stack->setCurrentWidget(HomePage);
    });

    connect(SeeAllPage, &SeeAllBooksWidget::bookClicked, this, [this](const Book &book) {
        BookDetailPage->loadBook(book);
        stack->setCurrentWidget(BookDetailPage);
    });

    connect(LibraryPage, &PersonalLibraryWidget::backToHomeRequested, this, [this]() {
        stack->setCurrentWidget(HomePage);
    });

    connect(LibraryPage, &PersonalLibraryWidget::readRequested, this, [this](int bookId, const QString &title) {
        readerReturnPage = stack->currentWidget();
        ReaderPage->openBook(bookId, title);
        stack->setCurrentWidget(ReaderPage);
    });

    connect(ReaderPage, &PdfReaderWidget::backRequested, this, [this]() {
        stack->setCurrentWidget(readerReturnPage ? readerReturnPage : HomePage);
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
        notificationsReturnPage = HomePage;
        NotificationPage->refreshFromServer();
        stack->setCurrentWidget(NotificationPage);
    });

    connect(PublisherDashboardPage, &PublisherDashboardWidget::notificationsRequested, this, [this]() {
        notificationsReturnPage = PublisherDashboardPage;
        NotificationPage->refreshFromServer();
        stack->setCurrentWidget(NotificationPage);
    });

    connect(NotificationPage, &NotificationWidget::backToHomeRequested, this, [this]() {
        stack->setCurrentWidget(notificationsReturnPage ? notificationsReturnPage : HomePage);
    });

    connect(LoginPage, &login::PublisherSignInSuccess, this, [this](Publisher publisher) {
        PublisherDashboardPage->loadPublisher(publisher);
        stack->setCurrentWidget(PublisherDashboardPage);
    });

    connect(PublisherDashboardPage, &PublisherDashboardWidget::backToLoginRequested, this, [this]() {
        NetworkClient::instance().sendRequest(RequestType::Logout);
        stack->setCurrentWidget(LoginPage);
    });

    connect(PublisherDashboardPage, &PublisherDashboardWidget::catalogChanged, this, [this]() {
        if (stack->currentWidget() == HomePage)
            loadHomePageContent();
    });

    connect(ProfilePage, &ProfileWidget::logoutRequested, this, [this]() {
        NetworkClient::instance().sendRequest(RequestType::Logout);
        currentUser = User();
        stack->setCurrentWidget(LoginPage);
    });

    connect(AdminPanelPage, &AdminPanelWidget::logoutRequested, this, [this]() {
        NetworkClient::instance().sendRequest(RequestType::Logout);
        stack->setCurrentWidget(LoginPage);
    });

    connect(AdminPanelPage, &AdminPanelWidget::catalogChanged, this, [this]() {
        if (stack->currentWidget() == HomePage)
            loadHomePageContent();
    });
}
void MainWindow::loadHomePageContent()
{
    QString errorMessage;

    allBooks = publisherManager->getAllActiveBooks(errorMessage);
    QVector<Book> recommended = publisherManager->getRecommendedBooks(errorMessage);
    QVector<Book> newReleases = publisherManager->getNewestBooks(errorMessage);
    QVector<Book> featured = publisherManager->getMostRatedBooks(errorMessage);
    QVector<Book> freeBooks = publisherManager->getFreeBooks(errorMessage);
    QVector<Book> bestSellers = publisherManager->getTopSellingBooks(errorMessage);

    // Genres row: one "most popular" (highest rated, ties broken by sales)
    // book per genre - genres with no books yet are simply skipped.
    QVector<Book> genreHighlights;
    for (genre g : allGenres()) {
        const Book *best = nullptr;
        for (const Book &b : allBooks) {
            if (b.getGenre() != g)
                continue;
            if (!best
                || b.getAverageRating() > best->getAverageRating()
                || (b.getAverageRating() == best->getAverageRating() && b.getSalesCount() > best->getSalesCount())) {
                best = &b;
            }
        }
        if (best)
            genreHighlights.append(*best);
    }

    HomePage->loadRecommendedBooks(recommended);
    HomePage->loadGenreHighlights(genreHighlights);
    HomePage->loadFeaturedBooks(featured);
    HomePage->loadNewReleases(newReleases);
    HomePage->loadBestSellers(bestSellers);
    HomePage->loadFreeBooks(freeBooks);
}
