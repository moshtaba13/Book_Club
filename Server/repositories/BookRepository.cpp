#include "BookRepository.h"
#include <QDateTime>

#include "../managers/AuthManager.h"

Book BookRepository::mapToBook(QSqlQuery &query)
{
    Book book;
    book.setId(query.value("id").toInt());
    book.setTitle(query.value("title").toString());
    book.setAuthor(query.value("author").toString());
    book.setGenre(stringToGenre(query.value("genre").toString()));
    book.setDescription(query.value("description").toString());
    book.setPrice(query.value("price").toDouble());
    book.setDiscount(query.value("discount_percent").toDouble());
    book.setImagePath(query.value("cover_image_path").toString());
    book.setPdfPath(query.value("pdf_file_path").toString());
    book.setisActive(query.value("is_active").toInt() == 1);
    book.setSalesCount(query.value("sales_count").toInt());
    book.setAverageRating(query.value("average_rating").toDouble());
    book.setPublishDate(QDateTime::fromString(query.value("publish_date").toString(), Qt::ISODate));
    book.setPublisherUsername(query.value("publisher_username").toString());

    QVector<Review> reviews = findReviews(book.getId());
    for (const Review &r : reviews)
        book.addReview(r);

    return book;
}

Review BookRepository::mapToReview(QSqlQuery &query)
{
    Review review(
        query.value("user_id").toInt(),
        query.value("stars").toInt(),
        query.value("comment").toString()
    );

    review.setUsername(AuthManager::instance().decrypt(query.value("username").toString()));

    QString lastEdited = query.value("last_edited").toString();
    if (!lastEdited.isEmpty())
        review.Edit(review.getStars(), review.getComment());

    return review;
}

BookRepository& BookRepository::instance() {
    static BookRepository instance;
    return instance;
}

bool BookRepository::save(int publisherId, Book &book) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO books "
        "(title, author, publisher_id, publisher_username , genre, description, "
        "price, discount_percent, cover_image_path, pdf_file_path, "
        "is_active, sales_count, average_rating, publish_date) "
        "VALUES "
        "(:title, :author, :publisher_id, :publisher_username, :genre, :description, "
        ":price, :discount, :cover, :pdf, "
        ":is_active, :sales_count, :avg_rating, :publish_date)"
    );

    query.bindValue(":title", book.getTitle());
    query.bindValue(":author", book.getAuthor());
    query.bindValue(":publisher_id", publisherId);
    query.bindValue(":genre", genreToString(book.getGenre()));
    query.bindValue(":description", book.getDiscription());
    query.bindValue(":price", book.getPrice());
    query.bindValue(":discount", book.getDiscount());
    query.bindValue(":cover", book.getImagePath());
    query.bindValue(":pdf", book.getPdfPath());
    query.bindValue(":is_active", book.getisActive() ? 1 : 0);
    query.bindValue(":sales_count", book.getSalesCount());
    query.bindValue(":avg_rating", book.getAverageRating());
    query.bindValue(":publish_date", book.getPublishDate().toString(Qt::ISODate));

    if (!query.exec()) {
        qWarning() << "BookRepository::save error:" << query.lastError().text();
        return false;
    }

    book.setId(query.lastInsertId().toInt());
    return true;
}

Book BookRepository::findById(int id) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT b.*, p.username as publisher_username "
        "FROM books b "
        "JOIN members p ON b.publisher_id = p.id "
        "WHERE b.id = :id"
    );
    query.bindValue(":id", id);

    if (!query.exec() || !query.next())
        return Book();

    return mapToBook(query);
}

QVector<Book> BookRepository::findAll()
{
    QVector<Book> books;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT b.*, p.username as publisher_username "
        "FROM books b "
        "JOIN members p ON b.publisher_id = p.id"
    );

    if (!query.exec())
        return books;

    while (query.next())
        books.append(mapToBook(query));

    return books;
}

QVector<Book> BookRepository::findAllActive() {
    QVector<Book> books;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT b.*, p.username as publisher_username "
        "FROM books b "
        "JOIN members p ON b.publisher_id = p.id "
        "WHERE b.is_active = 1"
    );

    if (!query.exec())
        return books;

    while (query.next())
        books.append(mapToBook(query));

    return books;
}

QVector<Book> BookRepository::findByPublisher(int publisherId)
{
    QVector<Book> books;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT b.*, p.username as publisher_username "
        "FROM books b "
        "JOIN members p ON b.publisher_id = p.id "
        "WHERE b.publisher_id = :publisher_id"
    );
    query.bindValue(":publisher_id", publisherId);

    if (!query.exec())
        return books;

    while (query.next())
        books.append(mapToBook(query));

    return books;
}

QVector<Book> BookRepository::findByGenre(genre g)
{
    QVector<Book> books;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT b.*, p.username as publisher_username "
        "FROM books b "
        "JOIN members p ON b.publisher_id = p.id "
        "WHERE b.genre = :genre AND b.is_active = 1"
    );
    query.bindValue(":genre", genreToString(g));

    if (!query.exec())
        return books;

    while (query.next())
        books.append(mapToBook(query));

    return books;
}

QVector<Book> BookRepository::search(const QString &keyword) 
{
    QVector<Book> books;
    QSqlQuery query = Database::instance().createQuery();

    query.prepare(
        "SELECT b.*, u.username as publisher_username "
        "FROM books b "
        "JOIN users u ON b.publisher_id = u.id "
        "WHERE b.is_active = 1 AND ("
        "b.title  LIKE :keyword OR "
        "b.author LIKE :keyword OR "
        "u.username LIKE :keyword"
        ")"
    );
    query.bindValue(":keyword", "%" + keyword + "%");

    if (!query.exec())
        return books;

    while (query.next())
        books.append(mapToBook(query));

    return books;
}

bool BookRepository::update(const Book &book)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "UPDATE books SET "
        "title = :title, "
        "author = :author, "
        "genre = :genre, "
        "description = :description, "
        "price = :price, "
        "discount_percent = :discount, "
        "cover_image_path = :cover, "
        "pdf_file_path = :pdf, "
        "is_active = :is_active "
        "WHERE id = :id"
    );

    query.bindValue(":title", book.getTitle());
    query.bindValue(":author", book.getAuthor());
    query.bindValue(":genre", genreToString(book.getGenre()));
    query.bindValue(":description", book.getDiscription());
    query.bindValue(":price", book.getPrice());
    query.bindValue(":discount", book.getDiscount());
    query.bindValue(":cover", book.getImagePath());
    query.bindValue(":pdf", book.getPdfPath());
    query.bindValue(":is_active", book.getisActive() ? 1 : 0);
    query.bindValue(":id", book.getId());

    if (!query.exec()) {
        qWarning() << "BookRepository::update error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BookRepository::updateDiscount(int bookId, double discount) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE books SET discount_percent = :discount WHERE id = :id");
    query.bindValue(":discount", discount);
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qWarning() << "BookRepository::updateDiscount error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BookRepository::updateActiveStatus(int bookId, bool isActive)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE books SET is_active = :is_active WHERE id = :id");
    query.bindValue(":is_active", isActive ? 1 : 0);
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qWarning() << "BookRepository::updateActiveStatus error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BookRepository::updateSalesCount(int bookId, int count)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE books SET sales_count = :count WHERE id = :id");
    query.bindValue(":count", count);
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qWarning() << "BookRepository::updateSalesCount error:"<< query.lastError().text();
        return false;
    }
    return true;
}

bool BookRepository::updateAverageRating(int bookId, double rating) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE books SET average_rating = :rating WHERE id = :id");
    query.bindValue(":rating", rating);
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qWarning() << "BookRepository::updateAverageRating error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BookRepository::remove(int bookId) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("DELETE FROM books WHERE id = :id");
    query.bindValue(":id", bookId);

    if (!query.exec()) {
        qWarning() << "BookRepository::remove error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool BookRepository::saveReview(int bookId, const Review &review)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO reviews "
        "(user_id, book_id, stars, comment, time_stamp) "
        "VALUES (:user_id, :book_id, :stars, :comment, :date)"
    );

    query.bindValue(":user_id", review.getUserId());
    query.bindValue(":book_id", bookId);
    query.bindValue(":stars", review.getStars());
    query.bindValue(":comment", review.getComment());
    query.bindValue(":date", review.getTimestamp().toString(Qt::ISODate));

    if (!query.exec()) {
        qWarning() << "BookRepository::saveReview error:" << query.lastError().text();
        return false;
    }

    QSqlQuery avgQuery = Database::instance().createQuery();
    avgQuery.prepare(
        "UPDATE books SET average_rating = "
        "(SELECT AVG(stars) FROM reviews WHERE book_id = :bid) "
        "WHERE id = :bid"
    );
    avgQuery.bindValue(":bid", bookId);
    avgQuery.exec();

    return true;
}

bool BookRepository::updateReview(int bookId, const Review &review)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "UPDATE reviews SET "
        "stars = :stars, "
        "comment = :comment, "
        "last_edited = :last_edited "
        "WHERE user_id = :user_id AND book_id = :book_id"
    );

    query.bindValue(":stars", review.getStars());
    query.bindValue(":comment", review.getComment());
    query.bindValue(":last_edited", review.getLastEdited().toString(Qt::ISODate));
    query.bindValue(":user_id", review.getUserId());
    query.bindValue(":book_id", bookId);

    if (!query.exec()) {
        qWarning() << "BookRepository::updateReview error:"<< query.lastError().text();
        return false;
    }

    QSqlQuery avgQuery = Database::instance().createQuery();
    avgQuery.prepare(
        "UPDATE books SET average_rating = "
        "(SELECT AVG(stars) FROM reviews WHERE book_id = :bid) "
        "WHERE id = :bid"
    );
    avgQuery.bindValue(":bid", bookId);
    avgQuery.exec();

    return true;
}

bool BookRepository::removeReview(int bookId, int userId)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "DELETE FROM reviews "
        "WHERE user_id = :user_id AND book_id = :book_id"
    );
    query.bindValue(":user_id", userId);
    query.bindValue(":book_id", bookId);

    if (!query.exec()) {
        qWarning() << "BookRepository::removeReview error:" << query.lastError().text();
        return false;
    }

    QSqlQuery avgQuery = Database::instance().createQuery();
    avgQuery.prepare(
        "UPDATE books SET average_rating = "
        "COALESCE((SELECT AVG(stars) FROM reviews WHERE book_id = :bid), 0) "
        "WHERE id = :bid"
    );
    avgQuery.bindValue(":bid", bookId);
    avgQuery.exec();

    return true;
}

QVector<Review> BookRepository::findReviews(int bookId) {
    QVector<Review> reviews;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "SELECT r.*, u.username FROM reviews r "
        "JOIN members u ON r.user_id = u.id "
        "WHERE r.book_id = :book_id"
    );
    query.bindValue(":book_id", bookId);

    if (!query.exec())
        return reviews;

    while (query.next())
        reviews.append(mapToReview(query));

    return reviews;
}