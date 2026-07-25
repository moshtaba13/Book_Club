#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QString>
#include <QDebug>

class Database
{
private:
    QSqlDatabase db;
    Database() = default;
    Database(const Database& ) = delete;
    Database& operator=(const Database& ) = delete;

    bool createMembersTable();
    bool createBooksTable();
    bool createPurchasesTable();
    bool createCartsTable();
    bool createReviewsTable();
    bool createSavedBooksTable();
    bool createShelvesTable();
    bool createShelfBooksTable();
    bool createNotificationsTable();

public:
    static Database& instance();

    bool connect(const QString& dbPath = "data/bookclub.db");
    bool isConnected() const;
    void close();

    QSqlQuery createQuery() const;

};

#endif