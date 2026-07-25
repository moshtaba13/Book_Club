#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QVector>
#include <QDateTime>

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
    int Id;
    static int idCounter;
    QString Title;
    QString Author;
    QString publisherUsername;
    genre Genre;
    QString Description;
    double Price;
    double Discount;
    QString imagePath;
    QString pdfPath;
    bool isActive;
    QDateTime publishDate;
    int salesCount;
    double averageRating;
    QVector<Review> Reviews;
    void recalculateAverageRating();

public:
    Book();
    Book(const QString& title,
         const QString& author,
         genre Genre,
         double Price
        );

    ~Book() = default;

    int getId() const;
    QString getTitle() const;
    QString getAuthor() const;
    QString getDiscription() const;
    genre getGenre() const;
    double getPrice() const;
    double getDiscount() const;
    bool getisActive() const;
    QString getImagePath() const;
    QString getPdfPath() const;
    QDateTime getPublishDate() const;
    int getSalesCount() const;
    double getAverageRating() const;
    double getFinalPrice() const;
    QString getPublisherUsername() const;

    void setId(const int id);
    void setTitle(const QString& newTitle);
    void setAuthor(const QString& newAuthor);
    void setGenre(genre newGenre);
    void setPrice(double newPrice);
    void setDiscount(double newDiscount);
    void setDescription(const QString &description);
    void setImagePath(const QString &newpath);
    void setPdfPath(const QString &newpath);
    void setisActive(bool newisActive);
    void setPublishDate(const QDateTime &date);
    void setSalesCount(int count);
    void setAverageRating(double ave);
    void setPublisherUsername(const QString& username);

    void addReview(const Review& review);
    const QVector<Review>& getReviews() const;
};

#endif