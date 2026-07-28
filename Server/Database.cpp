#include "Database.h"
#include <QDir>
#include <QDateTime>

#include "managers/AuthManager.h"

Database& Database::instance() {
    static Database instance;
    return instance;
}

bool Database::connect(const QString &dbPath) {
    QDir dir;
    if (!dir.exists("data"))
        dir.mkpath("data");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Database Error:" << db.lastError().text();
        return false;
    }

    QSqlQuery pragmaQuery(db);
    pragmaQuery.exec("PRAGMA foreign_keys = ON;");

    qDebug() << "Database connected successfully.";
    bool ok = createMembersTable()
        && createBooksTable()
        && createPurchasesTable()
        && createCartsTable()
        && createReviewsTable()
        && createSavedBooksTable()
        && createShelvesTable()
        && createShelfBooksTable()
        && createNotificationsTable();

    if (ok)
        ensureDefaultAdmin();

    return ok;
}

bool Database::isConnected() const{
    return db.isOpen();
}

void Database::close() {
    if (db.isOpen())
        db.close();
}

QSqlQuery Database::createQuery() const{
    return QSqlQuery(db);
}


bool Database::createMembersTable() {
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS members ("
        "id                     INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username               TEXT UNIQUE NOT NULL,"
        "password               TEXT NOT NULL,"
        "salt                   TEXT,"
        "role                   TEXT NOT NULL,"
        "security_answer        TEXT,"
        "is_blocked             INTEGER DEFAULT 0,"
        "is_first_login         INTEGER DEFAULT 1,"
        "favorite_genres        TEXT DEFAULT '[]',"
        "total_revenue          REAL DEFAULT 0.0,"
        "register_date          TEXT"
        ");"
    );
    if (!ok)
        qDebug() << "createUsersTable Error:" << query.lastError().text();
    return ok;
}

bool Database::createBooksTable() {
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS books ("
        "id               INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title            TEXT NOT NULL,"
        "author           TEXT NOT NULL,"
        "publisher_id     INTEGER NOT NULL,"
        "genre            TEXT NOT NULL,"
        "description      TEXT,"
        "price            REAL NOT NULL,"
        "discount_percent REAL DEFAULT 0.0,"
        "cover_image_path TEXT,"
        "pdf_file_path    TEXT,"
        "is_active        INTEGER DEFAULT 1,"
        "sales_count      INTEGER DEFAULT 0,"
        "average_rating   REAL DEFAULT 0.0,"
        "publish_date     TEXT,"
        "FOREIGN KEY(publisher_id) REFERENCES members(id)"
        ");"
    );
    if (!ok)
        qDebug() << "createBooksTable Error:" << query.lastError().text();
    return ok;
}

bool Database::createPurchasesTable() {
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS purchases ("
        "user_id        INTEGER NOT NULL,"
        "book_id        INTEGER NOT NULL,"
        "purchase_date  TEXT NOT NULL,"
        "price_paid     REAL NOT NULL,"
        "last_read_page INTEGER DEFAULT 0,"
        "PRIMARY KEY(user_id, book_id),"
        "FOREIGN KEY(user_id) REFERENCES members(id),"
        "FOREIGN KEY(book_id) REFERENCES books(id)"
        ");"
    );
    if (!ok)
        qDebug() << "createPurchasesTable Error:" << query.lastError().text();
    return ok;
}

bool Database::createCartsTable() {
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS carts ("
        "user_id  INTEGER NOT NULL,"
        "book_id  INTEGER NOT NULL,"
        "PRIMARY KEY(user_id, book_id),"
        "FOREIGN KEY(user_id) REFERENCES members(id),"
        "FOREIGN KEY(book_id) REFERENCES books(id)"
        ");"
    );
    if (!ok)
        qDebug() << "createCartItemsTable Error:" << query.lastError().text();
    return ok;
}

bool Database::createReviewsTable()
{
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS reviews ("
        "user_id     INTEGER NOT NULL,"
        "book_id     INTEGER NOT NULL,"
        "stars       INTEGER NOT NULL,"
        "comment     TEXT,"
        "time_stamp TEXT NOT NULL,"
        "last_edited TEXT,"
        "PRIMARY KEY(user_id, book_id),"
        "FOREIGN KEY(user_id) REFERENCES members(id),"
        "FOREIGN KEY(book_id) REFERENCES books(id)"
        ");"
    );
    if (!ok)
        qDebug() << "createReviewsTable Error:" << query.lastError().text();
    return ok;
}

bool Database::createSavedBooksTable()
{
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS saved_books ("
        "user_id         INTEGER NOT NULL,"
        "book_id         INTEGER NOT NULL,"
        "PRIMARY KEY(user_id, book_id),"
        "FOREIGN KEY(user_id) REFERENCES members(id),"
        "FOREIGN KEY(book_id) REFERENCES books(id)"
        ");"
    );
    if (!ok)
        qDebug() << "createSavedBooksTable Error:" << query.lastError().text();
    return ok;
}

bool Database::createShelvesTable()
{
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS shelves ("
        "user_id         INTEGER NOT NULL,"
        "name            TEXT NOT NULL,"
        "PRIMARY KEY(user_id, name),"
        "FOREIGN KEY(user_id) REFERENCES members(id)"
        ");"
    );
    if (!ok)
        qDebug() << "createShelvesTable Error:" << query.lastError().text();
    return ok;
}

bool Database::createShelfBooksTable()
{
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS shelf_books ("
        "user_id                INTEGER NOT NULL,"
        "shelf_name             TEXT NOT NULL,"
        "book_id                INTEGER NOT NULL,"
        "PRIMARY KEY(user_id, shelf_name, book_id),"
        "FOREIGN KEY(user_id) REFERENCES members(id),"
        "FOREIGN KEY(book_id) REFERENCES books(id)"
        ");"
    );
    if (!ok)
        qDebug() << "createShelfBooksTable Error:" << query.lastError().text();
    return ok;
}

bool Database::createNotificationsTable()
{
    QSqlQuery query(db);
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS notifications ("
        "id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id    INTEGER NOT NULL,"
        "message    TEXT NOT NULL,"
        "seen    INTEGER DEFAULT 0,"
        "created_at TEXT NOT NULL,"
        "FOREIGN KEY(user_id) REFERENCES members(id)"
        ");"
    );
    if (!ok)
        qDebug() << "createNotificationsTable Error:" << query.lastError().text();
    return ok;
}

bool Database::ensureDefaultAdmin()
{
    QSqlQuery check(db);
    check.prepare("SELECT COUNT(*) FROM members WHERE role = 'Admin'");
    if (check.exec() && check.next() && check.value(0).toInt() > 0)
        return true;

    QString salt = AuthManager::instance().generateSalt();

    QSqlQuery insert(db);
    insert.prepare(
        "INSERT INTO members "
        "(username, password, salt, role, security_answer, is_blocked, is_first_login, favorite_genres, total_revenue, register_date) "
        "VALUES "
        "(:username, :password, :salt, 'Admin', :answer, 0, 0, '[]', 0.0, :date)"
    );
    insert.bindValue(":username", AuthManager::instance().encrypt("admin"));
    insert.bindValue(":password", AuthManager::instance().hashPassword("admin123", salt));
    insert.bindValue(":salt", salt);
    insert.bindValue(":answer", AuthManager::instance().encrypt("N/A"));
    insert.bindValue(":date", QDateTime::currentDateTime().toString(Qt::ISODate));

    if (!insert.exec()) {
        qDebug() << "ensureDefaultAdmin Error:" << insert.lastError().text();
        return false;
    }

    qDebug() << "Default admin account created (username: admin, password: admin123).";
    return true;
}