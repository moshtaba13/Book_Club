#include "NotificationManager.h"

NotificationManager& NotificationManager::instance() {
    static NotificationManager instance;
    return instance;
}

void NotificationManager::notifNewBookInFavoriteGenre(const Book &book, const QVector<int> &userIds)
{
    QString message = QString("A new book you might like just arrived: '%1' ").arg(book.getTitle());
    
    Notification notification(message);
    for (int userId : userIds)
        NotificationRepository::instance().save(userId, notification);
}

void NotificationManager::notifDiscountOnSavedBook(const Book &book, const QVector<int> &userIds)
{
    QString message = QString("Good news! '%1' is now on sale.").arg(book.getTitle());
    Notification notification(message);

    for (int userId : userIds)
        NotificationRepository::instance().save(userId, notification);
}

void NotificationManager::notifNewSale(int publisherId, const Book &book) {
    QString message = QString("You just sold a copy of '%1'").arg(book.getTitle());
    Notification notification(message);
    NotificationRepository::instance().save(publisherId, notification);
}

void NotificationManager::notifNewReview(int publisherId, const Book &book){
    QString message = QString("Someone left a review on '%1'").arg(book.getTitle()); 
    Notification notification(message);
    NotificationRepository::instance().save(publisherId, notification);
}