#ifndef MEMBER_H
#define MEMBER_H

#include <Qstring>

class Member
{
private:
    const int ID;
    QString username;
    QString password;
    bool blocked;
    const QString securityAnswer;
public:
    Member(QString username, QString password, QString securityAnswer);
    virtual ~Member() = default;

    int getId() const;
    QString getUsername() const;
    void setUsername(const QString name);
    QString getPassword() const;
    void setPassword(const QString pass);
    QString getSecurityAnswer() const;
    bool isBlocked() const;
    void setBlocked(bool block);

    virtual QString role() const = 0;
};

#endif