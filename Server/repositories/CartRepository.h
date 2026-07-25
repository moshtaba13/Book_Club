#ifndef CARTREPOSITORY_H
#define CARTREPOSITORY_H

#include "../Database.h"
#include "../../Common/models/Cart.h"
#include <QVector>

class CartRepository
{
private:
    CartRepository() = default;
    CartRepository(const CartRepository&) = delete;
    CartRepository& operator=(const CartRepository&) = delete;
public:
    static CartRepository& instance();

    bool addBook(int userId, int bookId);
    bool removeBook(int userId, int bookId);
    bool clear(int userId);

    Cart loadCart(int userId);
    bool hasBook(int userId, int bookId);

};

#endif