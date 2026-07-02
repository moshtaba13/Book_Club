#ifndef REVIEW_H
#define REVIEW_H

#include <QString>
#include <QDateTime>

class Review
{
private:
    int UserId;
    int Stars;
    QString Comment;
    QDateTime TimeStamp;
    QDateTime lastEdited;

public:
    Review(int UserId,
           int Stars,
           const QString& Comment);

    ~Review() = default;

    int getUserId() const;
    int getStars() const;
    QString getComment() const;
    QDateTime getTimestamp() const;
    QDateTime getLastEdited() const;

    void setStars(int newStars);
    void setComment(const QString& newComment);

    void Edit(int newStars,
              const QString& newComment);
};

#endif // REVIEW_H