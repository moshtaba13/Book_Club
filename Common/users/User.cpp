#include "User.h"

User::User():Member(), firstLogin(true){}

User::User(const QString& username, const QString& password, const QString& securityAnswer)
    : Member(username, password, securityAnswer), purchasesCount(0) ,firstLogin(true) {}

QString User::role() const{ return "User";}

const QVector<genre>& User::getfavoriteGenres() const {return favoriteGenres;}
void User::setFavoriteGenres(const QVector<genre> &genres) { favoriteGenres = genres; }

void User::addFavoriteGenre(const genre &Genre)
{
    if (!favoriteGenres.contains(Genre) && favoriteGenres.size() < 3)
        favoriteGenres.append(Genre);
}

const QVector<Purchase>& User::getpurchasedBooks() const {return purchasedBooks;}

void User::addPurchasedBook(const Purchase &book) {
    if (!hasPurchasedBook(book.getBook().getId())) {
        purchasedBooks.append(book);
    }
}

bool User::hasPurchasedBook(int bookId) const{
    for (const Purchase &b : purchasedBooks)
        if (b.getBook().getId() == bookId)
            return true;
    return false;
}

const QVector<Book>& User::getSavedBooks() const{ return savedBooks;}
void User::addSavedBook(const Book &book) {
    if (!hasSavedBook(book.getId()))
        savedBooks.append(book);
}

void User::removeSavedBook(int bookId) {
    for (int i = 0; i < savedBooks.size(); i++) {
        if (savedBooks[i].getId() == bookId) {
            savedBooks.removeAt(i);
            break;
        }
    }
}

bool User::hasSavedBook(int bookId) const{
    for (const Book &b : savedBooks)
        if (b.getId() == bookId) 
            return true;
    return false;
}

const Cart& User::getCart() const{return cart;}

const QVector<Shelf>& User::getShelves() const{ return shelves;}

void User::addShelf(const Shelf &shelf) {
    if (!findShelf(shelf.getName()))
        shelves.append(shelf);
}

void User::removeShelf(const QString &shelfName) {
    for (int i = 0; i < shelves.size(); ++i) {
        if (shelves[i].getName() == shelfName) {
            shelves.removeAt(i);
            break;
        }
    }
}

void User::renameShelf(const QString &oldName, const QString &newName) {
    Shelf *shelf = findShelf(oldName);
    if (shelf)
        shelf->setName(newName);
}

Shelf* User::findShelf(const QString &name) {
    for (Shelf &s : shelves)
        if (s.getName() == name)
            return &s;
    return nullptr;
}

int User::lastReadPage(int bookId) const{
    for (const Purchase &p : purchasedBooks)
        if (p.getBook().getId() == bookId)
            return p.getLastReadPage();
    return 0;
}

void User::setLastReadPage(int bookId, int page) {
    for (Purchase &p : purchasedBooks)
        if (p.getBook().getId() == bookId) {
            p.setLastReadPage(page);
            break;
        }
}

int User::getPurchasecount() const{return purchasesCount;}
void User::setPurchaseCount(int count) {purchasesCount = count;}

bool User::isFirstLogin() const { return firstLogin; }
void User::setFirstLogin(bool first) { firstLogin = first;}