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


#endif // DBOPERATION_H
