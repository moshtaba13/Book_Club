#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "Member.h"
#include "../models/Book.h"
#include <QVector>

class Publisher : public Member
{
private:
    QVector<Book> publishedBooks;
    double totalRevenue;
public:
    Publisher();
    Publisher(const QString& username, const QString& hashedPassword, const QString& securityAnswer);

    QString role() const override;

    const QVector<Book>& getPublishedBooks() const;
    void addPublishedBook(const Book &book);
    void removePublishedBook(int bookId);
    void updatePublishedBook(const Book &book);

    double getTotalRevenue() const;
    void setTotalRevenue(double revenue);

    int numberOfPublishedBook() const;
};

#endif