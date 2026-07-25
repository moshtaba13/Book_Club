#ifndef CART_H
#define CART_H
#include <QVector>
#include "Book.h"

class Cart {
private:
    QVector<Book> Items;

    bool isBookInCart(int bookId);

public:
    Cart();
    ~Cart() = default;

    bool AddItem(const Book& newbook);
    bool RemoveItem(int bookId);
    bool isEmpty() const;

    const QVector<Book>& getItems() const;
    int getNumberofitems() const;

    double getTotalPrice() const;            
    double getTotalDiscount() const;          
    double getFinalPrice() const;            
    double getTotaldiscountPercentage() const;

    bool CheckOut();
};
#endif