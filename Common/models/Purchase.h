#ifndef PURCHASE_H
#define PURCHASE_H

#include <QDateTime>

#include "Book.h"

class Purchase
{
private:
    Book book;
    double pricePaid;
    QDateTime purchaseDate;
    int lastReadPage;
public:
    Purchase();
    Purchase(const Book &book, double pricePaid, const QDateTime &purchaseDate);

    Book getBook() const;
    double getPricePaid() const;
    QDateTime getPurchaseDate() const;
    int getLastReadPage() const;

    void setBook(const Book &b);
    void setPricePaid(double price);
    void setPurchaseDate(const QDateTime &date);
    void setLastReadPage(int page);
    
};

#endif