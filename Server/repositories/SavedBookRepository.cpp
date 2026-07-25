#include "SavedBookRepository.h"
#include "BookRepository.h"
#include "PurchaseRepository.h"

SavedBookRepository& SavedBookRepository::instance() {
    static SavedBookRepository instance;
    return instance;
}

bool SavedBookRepository::save(int userId, int bookId)
{
    if (hasBook(userId, bookId))
        return false;

    if (PurchaseRepository::instance().hasPurchased(userId, bookId))
        return false;

    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO saved_books (user_id, book_id) "
        "VALUES (:uid, :bid)"
    );
    query.bindValue(":uid", userId);
    query.bindValue(":bid", bookId);

    if (!query.exec()) {
        qWarning() << "SavedBookRepository::save error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool SavedBookRepository::remove(int userId, int bookId)
{
    if (!hasBook(userId, bookId))
        return false;
        
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "DELETE FROM saved_books "
        "WHERE user_id = :uid AND book_id = :bid"
    );
    query.bindValue(":uid", userId);
    query.bindValue(":bid", bookId);

    if (!query.exec()) {
        qWarning() << "SavedBookRepository::remove error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool SavedBookRepository::hasBook(int userId, int bookId) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT user_id FROM saved_books "
        "WHERE user_id = :uid AND book_id = :bid"
    );
    query.bindValue(":uid", userId);
    query.bindValue(":bid", bookId);
    query.exec();
    return query.next();
}

QVector<Book> SavedBookRepository::findByUser(int userId)
{
    QVector<Book> books;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT book_id FROM saved_books  "
        "WHERE user_id = :uid "
    );
    query.bindValue(":uid", userId);

    if (!query.exec())
        return books;

    while (query.next())
        books.append(BookRepository::instance().findById(query.value("id").toInt()));

    return books;
}

QVector<int> SavedBookRepository::findUsersByBook(int bookId)
{
    QVector<int> userIds;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT user_id FROM saved_books WHERE book_id = :bid");
    query.bindValue(":bid", bookId);

    if (!query.exec())
        return userIds;

    while (query.next())
        userIds.append(query.value("user_id").toInt());

    return userIds;
}