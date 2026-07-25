#include "Book.h"

Book::Book() 
    : Id(0),
      Discount(0.0),
      isActive(true),
      salesCount(0),
      averageRating(0.0){}

Book::Book(const QString& title,
         const QString& author,
         genre Genre,
         double Price)
    : Id(0),
      Title(title),
      Author(author),
      Genre(Genre),
      Price(Price),
      Discount(0.0),
      isActive(true),
      salesCount(0),
      averageRating(0.0){}

int Book::getId() const{ return Id;}
QString Book::getTitle() const{return Title;}
QString Book::getAuthor() const{return Author;}
genre Book::getGenre() const{return Genre;}
QString Book::getDiscription() const{return Description;}
double Book::getPrice() const{return Price;}
double Book::getDiscount() const{return Discount;}
bool Book::getisActive() const{return isActive;}
int Book::getSalesCount() const{return salesCount;}
QString Book::getImagePath() const{return imagePath;}
QString Book::getPdfPath() const{return pdfPath;}
QDateTime Book::getPublishDate() const{return publishDate;}
QString Book::getPublisherUsername() const{return publisherUsername;}
double Book::getAverageRating() const{return averageRating;}
double Book::getFinalPrice() const{
    return Price * (1.0 - Discount / 100.0);
}
const QVector<Review>& Book::getReviews() const{return Reviews;}

void Book::setId(const int id) {Id= id;}
void Book::setTitle(const QString& newTitle) {Title = newTitle;}
void Book::setAuthor(const QString& newAuthor) {Author = newAuthor;}
void Book::setGenre(genre newGenre){Genre = newGenre;}
void Book::setPrice(double newPrice){ Price = newPrice;}
void Book::setDiscount(double newDiscount) {Discount = newDiscount;}
void Book::setDescription(const QString& description) {Description = description;}
void Book::setImagePath(const QString& newpath) {imagePath = newpath;}
void Book::setPdfPath(const QString& newpath) {pdfPath = newpath;}
void Book::setPublishDate(const QDateTime& date) {publishDate = date;}
void Book::setisActive(bool newisActive) {isActive = newisActive;}
void Book::setAverageRating(double ave) {averageRating = ave;}
void Book::setSalesCount(int count) {salesCount = count;}
void Book::setPublisherUsername(const QString& username) {publisherUsername = username;}
void Book::addReview(const Review& review){
    Reviews.append(review);
}