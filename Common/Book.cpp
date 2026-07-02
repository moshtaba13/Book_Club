#include "Book.h"

Book::Book(int Id,
           const QString& Title,
           const QString& Author,
           genre Genre,
           double Price,
           double Discount,
           bool isActive)
    : Id(Id),
      Title(Title),
      Author(Author),
      Genre(Genre),
      Price(Price),
      Discount(Discount),
      isActive(isActive)
{
}

int Book::getId() const
{
    return Id;
}

QString Book::getTitle() const
{
    return Title;
}

QString Book::getAuthor() const
{
    return Author;
}

genre Book::getGenre() const
{
    return Genre;
}

double Book::getPrice() const
{
    return Price;
}

double Book::getDiscount() const
{
    return Discount;
}

bool Book::getisACTIVE() const
{
    return isActive;
}

void Book::setTitle(const QString& newTitle)
{
    Title = newTitle;
}

void Book::setAuthor(const QString& newAuthor)
{
    Author = newAuthor;
}

void Book::setGenre(genre newGenre)
{
    Genre = newGenre;
}

void Book::setPrice(double newPrice)
{
    if (newPrice >= 0.0)
        Price = newPrice;
}

void Book::setDiscount(double newDiscount)
{
    if (newDiscount >= 0.0 && newDiscount <= 100.0)
        Discount = newDiscount;
}

void Book::setisACTIVE(bool newisActive)
{
    isActive = newisActive;
}

double Book::getAverageRating() const
{
    if (Reviews.isEmpty())
        return 0.0;

    double sum = 0.0;

    for (const Review& review : Reviews)
        sum += review.getStars();

    return sum / Reviews.size();
}

double Book::getFinalPrice() const
{
    return Price * (1.0 - Discount / 100.0);
}

void Book::addReview(const Review& review)
{
    Reviews.append(review);
}