#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

static bool createConnection()
{
    // 创建一个数据库连接，使用“connection1”为连接名
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connection");
    db.setDatabaseName("Insurance.db");
    if (!db.open()) {
        QMessageBox::critical(0, "Cannot open database",
            "Unable to establish a database connection.", QMessageBox::Cancel);
        return false;
    }
    // 这里要指定连接
    QSqlQuery query(db);

    query.exec("create table UserList (Id int primary key, "
               "Name varchar(32),"
               "Password varchar(32),"
               "Role int)");
     /*
       role:1 admin
            2 manager
            3 seller
    */
    query.exec("create table PolicyList (Id int primary key, "
               "PolicyId varchar(32),"
               "Insured varchar(32),"
               "InsuredDocument varchar(32),"
               "CarNumber varchar(32),"
               "CarStyle varchar(32),"
               "CarValue int,"
               "PolicyTime date,"
               "StartTime date,"
               "Discount double"
               "UserId int)");

    query.exec("create table InsuranceList (Id int primary key, "
               "Name varchar(32),"
               "FixedCost double,"
               "FloatCost double)");

    query.exec("create table PolicyInsuranceRelationList (Id int primary key, "
               "PolicyId int,"
               "InsuranceId int)");

    query.exec("create table InfoList (Id int primary key, "
               "SenderId int,"
               "RecipientId int,"
               "Content varchar(256))");

    query.exec("insert into UserList values(0,'admin','hardpswd',1)");

    /*
    query.exec("insert into student values(0, 'LiMing')");
    query.exec("insert into student values(1, 'LiuTao')");
    query.exec("insert into student values(2, 'WangHong')");
    */
    return true;
}


#endif // DBCONNECT_H
