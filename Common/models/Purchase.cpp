#include "Purchase.h"

Purchase::Purchase(): pricePaid(0.0), lastReadPage(0){}

Purchase::Purchase(const Book &book, double pricePaid, const QDateTime &purchaseDate)
    : book(book)
    , pricePaid(pricePaid)
    , purchaseDate(purchaseDate)
    , lastReadPage(0)
{}

Book Purchase::getBook() const{ return book;}
void Purchase::setBook(const Book &b) { book = b;}

double Purchase::getPricePaid() const{ return pricePaid; }
void Purchase::setPricePaid(double price) { pricePaid = price; }

QDateTime Purchase::getPurchaseDate() const { return purchaseDate; }
void Purchase::setPurchaseDate(const QDateTime &date) { purchaseDate = date; }

int Purchase::getLastReadPage() const { return lastReadPage; }
void Purchase::setLastReadPage(int page) { lastReadPage = page; }