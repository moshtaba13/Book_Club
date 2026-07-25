#ifndef REVIEW_H
#define REVIEW_H

#include <QString>
#include <QDateTime>

class Review
{
private:
    int UserId;
    QString Username;
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

    QString getUsername() const;
    void setUsername(const QString &username);

};

#endif 