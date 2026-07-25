#include "Publisher.h"

Publisher::Publisher()
    : Member(), totalRevenue(0.0) {};

Publisher::Publisher(const QString &username, const QString &password, const QString& securityAnswer)
    : Member(username, password, securityAnswer),
      totalRevenue(0.0) {};

QString Publisher::role() const{return "Publisher";}

const QVector<Book>& Publisher::getPublishedBooks() const{ return publishedBooks;}

void Publisher::addPublishedBook(const Book &book){
    publishedBooks.append(book);
}

void Publisher::removePublishedBook(int bookId) {
    for (int i = 0; i < publishedBooks.size(); ++i) {
        if (publishedBooks[i].getId() == bookId) {
            publishedBooks.removeAt(i);
            break;
        }
    }
}

void Publisher::updatePublishedBook(const Book &updated) {
    for (Book &b : publishedBooks) {
        if (b.getId() == updated.getId()) {
            b = updated;
            break;
        }
    }
}

double Publisher::getTotalRevenue() const{ return totalRevenue;}
void Publisher::setTotalRevenue(double revenue) { totalRevenue = revenue;}

int Publisher::numberOfPublishedBook() const{return publishedBooks.size();}