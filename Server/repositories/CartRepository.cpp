#include "CartRepository.h"
#include "BookRepository.h"

CartRepository& CartRepository::instance() {
    static CartRepository instance;
    return instance;
}

bool CartRepository::addBook(int userId, int bookId)
{
    if (hasBook(userId, bookId))
        return false;

    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO carts (user_id, book_id) "
        "VALUES (:uid, :bid)"
    );
    query.bindValue(":uid", userId);
    query.bindValue(":bid", bookId);

    if (!query.exec()) {
        qWarning() << "CartRepository::addBook error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CartRepository::removeBook(int userId, int bookId)
{
    if (!hasBook(userId, bookId))
        return false;
        
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "DELETE FROM carts "
        "WHERE user_id = :uid AND book_id = :bid"
    );
    query.bindValue(":uid", userId);
    query.bindValue(":bid", bookId);

    if (!query.exec()) {
        qWarning() << "CartRepository::removeBook error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CartRepository::clear(int userId)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("DELETE FROM carts WHERE user_id = :uid");
    query.bindValue(":uid", userId);

    if (!query.exec()) {
        qWarning() << "CartRepository::clear error:" << query.lastError().text();
        return false;
    }
    return true;
}

Cart CartRepository::loadCart(int userId)
{
    Cart cart;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT b.* FROM carts c "
        "JOIN books b ON c.book_id = b.id "
        "WHERE c.user_id = :uid AND b.is_active = 1"
    );
    query.bindValue(":uid", userId);

    if (!query.exec())
        return cart;

    while (query.next()) {
        Book book = BookRepository::instance().findById(query.value("id").toInt());
        cart.AddItem(book);
    }

    return cart;
}

bool CartRepository::hasBook(int userId, int bookId) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT user_id FROM carts "
        "WHERE user_id = :uid AND book_id = :bid"
    );
    query.bindValue(":uid", userId);
    query.bindValue(":bid", bookId);
    query.exec();
    return query.next();
}