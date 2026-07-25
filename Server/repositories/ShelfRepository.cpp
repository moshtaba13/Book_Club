#include "ShelfRepository.h"
#include "BookRepository.h"
#include "PurchaseRepository.h"

ShelfRepository& ShelfRepository::instance() {
    static ShelfRepository instance;
    return instance;
}

bool ShelfRepository::addShelf(int userId, const QString &name) {
    if (shelfExists(userId, name))
        return false;

    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO shelves (user_id, name) "
        "VALUES (:uid, :name)"
    );
    query.bindValue(":uid",  userId);
    query.bindValue(":name", name);

    if (!query.exec()) {
        qWarning() << "ShelfRepository::addShelf error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool ShelfRepository::removeShelf(int userId, const QString &name) {
    QSqlQuery booksQuery = Database::instance().createQuery();
    booksQuery.prepare(
        "DELETE FROM shelf_books "
        "WHERE user_id = :uid AND shelf_name = :name"
    );
    booksQuery.bindValue(":uid",  userId);
    booksQuery.bindValue(":name", name);
    booksQuery.exec();

    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "DELETE FROM shelves "
        "WHERE user_id = :uid AND name = :name"
    );
    query.bindValue(":uid",  userId);
    query.bindValue(":name", name);

    if (!query.exec()) {
        qWarning() << "ShelfRepository::removeShelf error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool ShelfRepository::renameShelf(int userId, const QString &oldName, const QString &newName)
{
    QSqlQuery booksQuery = Database::instance().createQuery();
    booksQuery.prepare(
        "UPDATE shelf_books SET shelf_name = :new "
        "WHERE user_id = :uid AND shelf_name = :old"
    );
    booksQuery.bindValue(":new", newName);
    booksQuery.bindValue(":uid", userId);
    booksQuery.bindValue(":old", oldName);
    booksQuery.exec();

    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "UPDATE shelves SET name = :new "
        "WHERE user_id = :uid AND name = :old"
    );
    query.bindValue(":new", newName);
    query.bindValue(":uid", userId);
    query.bindValue(":old", oldName);

    if (!query.exec()) {
        qWarning() << "ShelfRepository::renameShelf error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool ShelfRepository::addBook(int userId, const QString &shelfName, int bookId)
{
    if (!shelfExists(userId, shelfName))
        return false;
    
    if (!PurchaseRepository::instance().hasPurchased(userId, bookId))
        return false;

    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO shelf_books (user_id, shelf_name, book_id) "
        "VALUES (:uid, :name, :bid)"
    );
    query.bindValue(":uid",  userId);
    query.bindValue(":name", shelfName);
    query.bindValue(":bid",  bookId);

    if (!query.exec()) {
        qWarning() << "ShelfRepository::addBook error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool ShelfRepository::removeBook(int userId, const QString &shelfName, int bookId)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "DELETE FROM shelf_books "
        "WHERE user_id = :uid AND shelf_name = :name AND book_id = :bid"
    );
    query.bindValue(":uid",  userId);
    query.bindValue(":name", shelfName);
    query.bindValue(":bid",  bookId);

    if (!query.exec()) {
        qWarning() << "ShelfRepository::removeBook error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool ShelfRepository::moveBook(int userId, const QString &fromShelf, const QString &toShelf, int bookId) {
    if (!shelfExists(userId, toShelf))
        return false;

    if (!addBook(userId, toShelf, bookId))
        return false;

    if (!removeBook(userId, fromShelf, bookId))
        return false;

    return true;
}

QVector<Shelf> ShelfRepository::findByUser(int userId)
{
    QVector<Shelf> shelves;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT name FROM shelves WHERE user_id = :uid");
    query.bindValue(":uid", userId);

    if (!query.exec())
        return shelves;

    while (query.next()) {
        QString shelfName = query.value("name").toString();
        Shelf shelf(shelfName);

        QSqlQuery booksQuery = Database::instance().createQuery();
        booksQuery.prepare(
            "SELECT b.* FROM shelf_books sb "
            "JOIN books b ON sb.book_id = b.id "
            "WHERE sb.user_id = :uid AND sb.shelf_name = :name"
        );
        booksQuery.bindValue(":uid",  userId);
        booksQuery.bindValue(":name", shelfName);

        if (booksQuery.exec()) {
            while (booksQuery.next())
                shelf.addBook(BookRepository::instance().findById(booksQuery.value("id").toInt()));
        }

        shelves.append(shelf);
    }

    return shelves;
}

bool ShelfRepository::shelfExists(int userId, const QString &name) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT user_id FROM shelves "
        "WHERE user_id = :uid AND name = :name"
    );
    query.bindValue(":uid",  userId);
    query.bindValue(":name", name);
    query.exec();
    return query.next();
}