#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QVector>

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

class Book {
private:
    int Id;
    QString Title;
    QString Author;
    double Price;
    double Discount;
    genre Genre;
    bool isActive;
    QVector<QString> Comments;
    QVector<int> Ratings;

public:

    Book(int Id, QString Title, QString Author, double Price, double Discount, genre Genre, bool isActive);

    int getId() const;
    QString getTitle() const;
    QString getAuthor() const;
    double getPrice() const;
    double getDiscount() const;
    genre getGenre() const;
    bool getisACTIVE() const;

    void setTitle(QString newTitle);
    void setAuthor(QString newAuthor);
    void setPrice(double newPrice);
    void setDiscount(double newDiscount);
    void setGenre(genre newGenre);
    void setisACTIVE(bool newisActive);

    double getAverageRating() const;
    double getFinalPrice() const;
    void addReview(int Rating, const QString& Comment);
};

#endif 