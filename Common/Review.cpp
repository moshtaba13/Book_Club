#include "Review.h"

Review::Review(int UserId, int Stars, const QString& Comment)
    : UserId(UserId),
    Stars(Stars),
    Comment(Comment),
    TimeStamp(QDateTime::currentDateTime()),
    lastEdited()
{
    if (this->Stars < 1)
        this->Stars = 1;

    if (this->Stars > 5)
        this->Stars = 5;
}

int Review::getUserId() const
{
    return UserId;
}

int Review::getStars() const
{
    return Stars;
}

QString Review::getComment() const
{
    return Comment;
}

QDateTime Review::getTimestamp() const
{
    return TimeStamp;
}

QDateTime Review::getLastEdited() const
{
    return lastEdited;
}

void Review::setStars(int newStars)
{
    if (newStars >= 1 && newStars <= 5)
        Stars = newStars;
}

void Review::setComment(const QString& newComment)
{
    Comment = newComment;
}

void Review::Edit(int newStars, const QString& newComment)
{
    setStars(newStars);
    setComment(newComment);

    lastEdited = QDateTime::currentDateTime();
}