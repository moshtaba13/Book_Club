#include "BookManager.h"
#include "NotificationManager.h"


void BookManager::notifUsersNewBook(const Book &book)
{
    QVector<shared_ptr<Member>> allUsers = MemberRepository::instance().findAllUsers();

    QVector<int> userIds;
    for (const auto &member : allUsers) {
        User *user = dynamic_cast<User*>(member.get());
        if (user && user->getfavoriteGenres().contains(book.getGenre()))
            userIds.append(user->getId());
    }

    if (!userIds.isEmpty())
        NotificationManager::instance().notifNewBookInFavoriteGenre(book, userIds);
}

void BookManager::notifUsersDiscount(const Book &book) {
    QVector<int> userIds = SavedBookRepository::instance().findUsersByBook(book.getId());

    if (!userIds.isEmpty())
        NotificationManager::instance().notifDiscountOnSavedBook(book, userIds);
}

BookManager& BookManager::instance() {
    static BookManager instance;
    return instance;
}

bool BookManager::addBook(int publisherId, const QString &publisherUsername, Book &book)
{
    book.setPublishDate(QDateTime::currentDateTime());
    book.setisActive(true);
    book.setSalesCount(0);
    book.setAverageRating(0.0);

    if (!BookRepository::instance().save(publisherId, book))
        return false;

    notifUsersNewBook(book);
    return true;
}

bool BookManager::applyDiscount(int bookId, double discount){
    if (!BookRepository::instance().updateDiscount(bookId, discount))
        return false;

    Book book = BookRepository::instance().findById(bookId);
    notifUsersDiscount(book);
    return true;
}

bool BookManager::addReview(int bookId, Review &review) {
    if (!BookRepository::instance().saveReview(bookId, review))
        return false;

    Book book = BookRepository::instance().findById(bookId);
    auto publisher = MemberRepository::instance().findByUsername(book.getPublisherUsername());

    if (publisher)
        NotificationManager::instance().notifNewReview(publisher->getId(), book);

    return true;
}

bool BookManager::activeBook(int bookId) {
    if(!BookRepository::instance().updateActiveStatus(bookId, true))
        return false;
    return true;
}

bool BookManager::inactiveBook(int bookId) {
    if(!BookRepository::instance().updateActiveStatus(bookId, false))
        return false;
    return true;
}

QVector<Book> BookManager::getTopSellingBooks(int limit)
{
    QVector<Book> books = BookRepository::instance().findAllActive();
    sort(books.begin(), books.end(),
        [](const Book &a, const Book &b) {
            return a.getSalesCount() > b.getSalesCount();
        });

    if (books.size() > limit)
        books = books.mid(0, limit);
    return books;
}

QVector<Book> BookManager::getLeastSellingBooks(int limit)
{
    QVector<Book> books = BookRepository::instance().findAllActive();

    sort(books.begin(), books.end(),
        [](const Book &a, const Book &b) {
            return a.getSalesCount() < b.getSalesCount();
        });

    if (books.size() > limit)
        books = books.mid(0, limit);

    return books;
}

QVector<Book> BookManager::getNewestBooks(int limit)
{
    QVector<Book> books = BookRepository::instance().findAllActive();
    sort(books.begin(), books.end(),
        [](const Book &a, const Book &b) {
            return a.getPublishDate() > b.getPublishDate();
        });

    if (books.size() > limit)
        books = books.mid(0, limit);
    return books;
}

QVector<Book> BookManager::getMostRatedBooks(int limit)
{
    QVector<Book> books = BookRepository::instance().findAllActive();
    sort(books.begin(), books.end(),
        [](const Book &a, const Book &b) {
            return a.getAverageRating() > b.getAverageRating();
        });

    if (books.size() > limit)
        books = books.mid(0, limit);
    return books;
}

QVector<Book> BookManager::getFreeBooks()
{
    QVector<Book> allBooks = BookRepository::instance().findAllActive();
    QVector<Book> freeBooks;

    for (const Book &b : allBooks)
        if (b.getFinalPrice() == 0.0)
            freeBooks.append(b);

    return freeBooks;
}

QVector<Book> BookManager::getRecommendedBooks(const QVector<genre> &genres)
{
    QVector<Book> recommended;
    for (const genre &g : genres) {
        QVector<Book> books = BookRepository::instance().findByGenre(g);
        for (const Book &b : books)
            recommended.append(b);
    }
    return recommended;
}
