#include "PurchaseRepository.h"
#include "BookRepository.h"

Purchase PurchaseRepository::mapToPurchase(QSqlQuery &query)
{
    Book book;
    book.setId(query.value("book_id").toInt());
    book.setTitle(query.value("title").toString());
    book.setAuthor(query.value("author").toString());
    book.setGenre(stringToGenre(query.value("genre").toString()));
    book.setPrice(query.value("price").toDouble());
    book.setDiscount(query.value("discount_percent").toDouble());
    book.setImagePath(query.value("cover_image_path").toString());
    book.setPdfPath(query.value("pdf_file_path").toString());

    Purchase purchase(
        book,
        query.value("price_paid").toDouble(),
        QDateTime::fromString(query.value("purchase_date").toString(),Qt::ISODate)
    );
    purchase.setLastReadPage(query.value("last_read_page").toInt());

    return purchase;
}

PurchaseRepository& PurchaseRepository::instance() {
    static PurchaseRepository instance;
    return instance;
}

bool PurchaseRepository::save(int userId, const Purchase &purchase)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO purchases "
        "(user_id, book_id, purchase_date, price_paid, last_read_page) "
        "VALUES (:user_id, :book_id, :date, :price, :page)"
    );

    query.bindValue(":user_id", userId);
    query.bindValue(":book_id", purchase.getBook().getId());
    query.bindValue(":date", purchase.getPurchaseDate().toString(Qt::ISODate));
    query.bindValue(":price", purchase.getPricePaid());
    query.bindValue(":page", purchase.getLastReadPage());

    if (!query.exec()) {
        qWarning() << "PurchaseRepository::save error:" << query.lastError().text();
        return false;
    }

    BookRepository::instance().updateSalesCount(
        purchase.getBook().getId(),
        purchase.getBook().getSalesCount() + 1
    );

    return true;
}

QVector<Purchase> PurchaseRepository::findByUser(int userId)
{
    QVector<Purchase> purchases;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT p.*, b.* FROM purchases p "
        "JOIN books b ON p.book_id = b.id "
        "WHERE p.user_id = :user_id"
    );
    query.bindValue(":user_id", userId);

    if (!query.exec())
        return purchases;

    while (query.next())
        purchases.append(mapToPurchase(query));

    return purchases;
}

bool PurchaseRepository::hasPurchased(int userId, int bookId) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT user_id FROM purchases "
        "WHERE user_id = :uid AND book_id = :bid"
    );
    query.bindValue(":uid", userId);
    query.bindValue(":bid", bookId);
    query.exec();
    return query.next();
}

bool PurchaseRepository::updateLastReadPage(int userId,int bookId,int page)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "UPDATE purchases SET last_read_page = :page "
        "WHERE user_id = :uid AND book_id = :bid"
    );
    query.bindValue(":page", page);
    query.bindValue(":uid",  userId);
    query.bindValue(":bid",  bookId);

    if (!query.exec()) {
        qWarning() << "PurchaseRepository::updateLastReadPage error:" << query.lastError().text();
        return false;
    }
    return true;
}

int PurchaseRepository::getLastReadPage(int userId, int bookId)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT last_read_page FROM purchases "
        "WHERE user_id = :uid AND book_id = :bid"
    );
    query.bindValue(":uid", userId);
    query.bindValue(":bid", bookId);
    query.exec();

    if (query.next())
        return query.value("last_read_page").toInt();
    return 0;
}

