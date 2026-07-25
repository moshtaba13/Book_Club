#ifndef USER_H
#define USER_H

#include <QVector>

#include "../models/Book.h"
#include "Member.h"
#include "../models/Cart.h"
#include "../models/Shelf.h"
#include "../models/Purchase.h"

class User: public Member
{
private:
    QVector<genre> favoriteGenres;
    QVector<Purchase> purchasedBooks;
    QVector<Book> savedBooks;
    Cart cart;
    QVector<Shelf> shelves;
    int purchasesCount;
    bool firstLogin;
public:
    User();
    User(const QString& username, const QString& hashedPassword, const QString& securityAnswer);
    QString role() const override;

    const QVector<genre>& getfavoriteGenres() const;
    void setFavoriteGenres(const QVector<genre> &genres);
    void addFavoriteGenre(const genre &Genre);

    const QVector<Purchase>& getpurchasedBooks() const;
    void addPurchasedBook(const Purchase &book);
    bool hasPurchasedBook(int bookId) const;

    const QVector<Book>& getSavedBooks() const;
    void addSavedBook(const Book &book);
    void removeSavedBook(int bookId);
    bool hasSavedBook(int bookId) const;

    const Cart& getCart() const;

    const QVector<Shelf>& getShelves() const;
    void addShelf(const Shelf &shelf);
    void removeShelf(const QString &shelfName);
    void renameShelf(const QString &oldName, const QString &newName);
    Shelf* findShelf(const QString &name); 

    int lastReadPage(int bookId) const;
    void setLastReadPage(int bookId, int page);

    int getPurchasecount() const;
    void setPurchaseCount(int count);


    bool isFirstLogin() const;
    void setFirstLogin(bool first);

};

#endif