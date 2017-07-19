#ifndef DBOPERATION_H
#define DBOPERATION_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDate>

#define DELETE_SUCCESS 0
#define DELETE_ERROR_NO_USER -1
#define DELETE_ERROR_ADMIN -2

#define ADD_SUCCESS 0
#define ADD_ERROR_SAME_USERNAME -1
#define ADD_ERROR_DIFF_PASSWORD -2
#define ADD_ERROR_PASSWORD_NULL -3
#define ADD_ERROR_USERNAME_NULL -4

#define QUERY_NO_USER -1

#define INFO_SUCCESS 0
#define INFO_ERROR_NO_USER -1
#define INFO_DEL_SUCCESS 0
#define INFO_DEL_FAILED_NO_PERMISSION -1

#define INSURANCE_ADD_SUCCESS 0

#define POLICY_ADD_SUCCESS 0
#define POLICY_ADD_ERROR_SAME_POLICYID -1
#define POLICY_ADD_ERROR_IMPORTANT_INFO_EMPTY -2
#define POLICY_DEL_SUCCESS 0

#define RECIDEL 0
#define SENDDEL 1

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
    int query();
};

typedef int INFODELTYPE;

class Info
{
private:
    int id;
    QString text;
    int senderid,recipientid;
public:
    Info(int _senderid,int _recipientid,QString _text);
    Info(int _id);
    int add();
    int del(INFODELTYPE _INFODELTYPE);
};

void Current_user_send_Info_clear();
void Current_user_reci_Info_clear();

class Insurance
{
private:
    QString name;
    int fixedcost;
    double floatcost;
public:
    Insurance(QString _name,int _fixedcost,double _floatcost);
    int add();
};

class Policy
{
private:
    int id;
    QString policyid, clientname, clientcard, carnumber, carstyle;
    int carvalue;
    double discount;
    QDate startdate;
public:
    Policy(QString _policyid,QString _clientname,QString _clientcard,
           QString _carnumber,QString _carstyle,
           int _carvalue,double _discount,
           QDate _startdate);
    Policy(int _id);
    int add(int& Policyid);
    int del();
};

#endif // DBOPERATION_H
