#include "Book.h"


Book::Book(int Id, QString Title, QString Author, double Price, double Discount, genre Genre, bool isActive)
    : Id(Id), Title(Title), Author(Author), Price(Price), Discount(Discount), Genre(Genre), isActive(isActive) {}

int Book::getId() const { return Id; }
QString Book::getTitle() const { return Title; }
QString Book::getAuthor() const { return Author; }
double Book::getPrice() const { return Price; }
double Book::getDiscount() const { return Discount; }
genre Book::getGenre() const { return Genre; }
bool Book::getisACTIVE() const { return isActive; }

void Book::setTitle(QString newTitle) { Title = newTitle; }
void Book::setAuthor(QString newAuthor) { Author = newAuthor; }

void Book::setPrice(double newPrice) {
    if (newPrice >= 0.0) {
        Price = newPrice;
    }
}

void Book::setDiscount(double newDiscount) {
    if (newDiscount >= 0.0 && newDiscount <= 100.0) {
        Discount = newDiscount;
    }
}

void Book::setGenre(genre newGenre) { Genre = newGenre; }
void Book::setisACTIVE(bool newisActive) { isActive = newisActive; }


double Book::getAverageRating() const {
    if (Ratings.isEmpty()) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (int r : Ratings) { 
        sum += r;
    }
    
    return sum / Ratings.size(); 
}

double Book::getFinalPrice() const {
    return Price * (1.0 - (Discount / 100.0));
}


void Book::addReview(int rating, const QString& Comment) {

    if (rating >= 1 && rating <= 5) {
        Ratings.append(rating);
    
        if (!Comment.isEmpty()) {
            Comments.append(Comment);
        } else {
            Comments.append("");
        }
    }
}