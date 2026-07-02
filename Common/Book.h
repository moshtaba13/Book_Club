#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QVector>

#include "Review.h"

enum class genre {
    Fiction,
    Non_fiction,
    Academic,
    Drama,
    Mystery,
    Crime,
    Romance,
    History,
    Science_fiction,
    Scary
};

class Book
{
private:
    const int Id;
    QString Title;
    QString Author;
    genre Genre;
    double Price;
    double Discount;
    bool isActive;
    static int counter;

    QVector<Review> Reviews;

public:
    Book(int Id,
         const QString& Title,
         const QString& Author,
         genre Genre,
         double Price,
         double Discount,
         bool isActive);

    ~Book() = default;

    int getId() const;
    QString getTitle() const;
    QString getAuthor() const;
    genre getGenre() const;
    double getPrice() const;
    double getDiscount() const;
    bool getisACTIVE() const;
    int geBookCount() const;

    void setTitle(const QString& newTitle);
    void setAuthor(const QString& newAuthor);
    void setGenre(genre newGenre);
    void setPrice(double newPrice);
    void setDiscount(double newDiscount);
    void setisACTIVE(bool newisActive);

    double getAverageRating() const;
    double getFinalPrice() const;

    void addReview(const Review& review);
};

#endif