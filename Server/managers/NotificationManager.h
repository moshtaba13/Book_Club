#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QVector>

#include "../repositories/NotificationRepository.h"
#include "../../Common/models/Book.h"
#include "../../Common/models/Notification.h"

class NotificationManager
{
private:
    NotificationManager() = default;
    NotificationManager(const NotificationManager&) = delete;
    NotificationManager& operator=(const NotificationManager&) = delete;

public:
    static NotificationManager& instance();

    void notifNewBookInFavoriteGenre(const Book &book, const QVector<int> &userIds);
    void notifDiscountOnSavedBook(const Book &book,const QVector<int> &userIds);

    void notifNewSale(int publisherId, const Book &book);
    void notifNewReview(int publisherId, const Book &book);


};

#endif