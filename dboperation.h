#ifndef DBOPERATION_H
#define DBOPERATION_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>


#define DELETE_SUCCESS 0
#define DELETE_ERROR_NO_USER -1
#define DELETE_ERROR_ADMIN -2

#define ADD_SUCCESS 0
#define ADD_ERROR_SAME_USERNAME -1
#define ADD_ERROR_DIFF_PASSWORD -2
#define ADD_ERROR_PASSWORD_NULL -3

#define INFO_SUCCESS 0
#define INFO_ERROR_NO_USER -1


class User
{
private:
    QString name,pswd1,pswd2;
    int role;
public:
    User(QString _name,QString _pswd1,QString _pswd2,int _role);
    User(QString _name);
    int add();
    int del();
};

class Info
{
private:
    QString text;
    int senderid,recipientid;
public:
    Info(int _senderid,int _recipientid,QString _text);
    int add();
};


#endif // DBOPERATION_H
