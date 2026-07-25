#include "Book.h"
#include "Cart.h"

Cart::Cart() {};

bool Cart::isBookInCart(int bookId) {
    for (int i = 0; i < Items.size(); ++i) {
        if (Items[i].getId() == bookId) {
            return true;
        }
    }
    return false;
}

bool Cart::AddItem(const Book& newbook) {
    if (isBookInCart(newbook.getId())) {
        return false;
    }
    Items.append(newbook);
    return true;
}

bool Cart::RemoveItem(int bookId) {
    for (int i = 0; i < Items.size(); ++i) {
        if (Items[i].getId() == bookId) {
            Items.removeAt(i);
            return true;
        }
    }
    return false;
}

bool Cart::isEmpty() const{
    if (Items.isEmpty())
        return true;
    return false;
}

const QVector<Book>& Cart::getItems() const {
    return Items;
}

int Cart::getNumberofitems() const{
    return Items.size();
}

double Cart::getTotalPrice() const{
    double sum = 0.0;
    for (const Book& eachbook : Items) {
        sum = sum + eachbook.getPrice();
    }
    return sum;
};

double Cart::getTotalDiscount() const{
    double sum = 0.0;
    for (const Book& eachbook : Items) {
        sum = sum + (eachbook.getPrice() - eachbook.getFinalPrice());
    }
    return sum;
};  

double Cart::getFinalPrice() const{
    return getTotalPrice() - getTotalDiscount();
};

double Cart::getTotaldiscountPercentage() const{
    if (getTotalPrice() == 0.0) {
        return 0.0;
    }
    return (getTotalDiscount() / getTotalPrice()) * 100.0;
}

bool Cart::CheckOut() {
    if (Items.isEmpty()) {
        return false;
    }
    Items.clear();
    return true;
}
