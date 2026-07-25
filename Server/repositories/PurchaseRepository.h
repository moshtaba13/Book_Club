#ifndef PURCHASEREPOSITORY_H
#define PURCHASEREPOSITORY_H

#include "../Database.h"
#include "../../Common/models/Purchase.h"
#include <QVector>

class PurchaseRepository
{
private:
    PurchaseRepository() = default;
    PurchaseRepository(const PurchaseRepository&) = delete;
    PurchaseRepository& operator=(const PurchaseRepository&) = delete;

    Purchase mapToPurchase(QSqlQuery &query);
public:
    static PurchaseRepository& instance();

    bool save(int userId, const Purchase &purchase);

    QVector<Purchase> findByUser(int userId);
    bool hasPurchased(int userId, int bookId);

    bool updateLastReadPage(int userId, int bookId, int page);
    int getLastReadPage(int userId, int bookId);


};

#endif