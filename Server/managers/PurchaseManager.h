#ifndef PURCHASEMANAGER_H
#define PURCHASEMANAGER_H

#include "../repositories/PurchaseRepository.h"
#include "../repositories/CartRepository.h"
#include "../repositories/BookRepository.h"
#include "../repositories/MemberRepository.h"
#include "../repositories/SavedBookRepository.h"
#include "../../Common/models/Cart.h"
#include "../../Common/models/Purchase.h"
#include "../../Common/users/User.h"
#include "../../Common/users/Publisher.h"
#include <QVector>

class PurchaseManager
{
private:
    PurchaseManager() = default;
    PurchaseManager(const PurchaseManager&) = delete;
    PurchaseManager& operator=(const PurchaseManager&) = delete;
public:
    static PurchaseManager& instance();

    bool addToCart(int userId, int bookId);
    bool checkout(int userId);


};

#endif