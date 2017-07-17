#include "dboperation.h"

User::User(QString _name, QString _pswd1, QString _pswd2, int _role)
{
    name = _name;
    pswd1 = _pswd1;
    pswd2 = _pswd2;
    role = _role;
}

User::User(QString _name)
{
    name=_name;
}

int User::add()
{
    if(this->pswd1!=this->pswd2)
        return ADD_ERROR_DIFF_PASSWORD;
    if(this->pswd1==NULL)
        return ADD_ERROR_PASSWORD_NULL;
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec("select * from UserList where Name='"+this->name+"'");
    if(query.first())
        return ADD_ERROR_SAME_USERNAME;
    query.exec("select * from UserList");
    query.last();
    int newid = query.value(0).toInt()+1;
    query.exec(QString("insert into UserList values( %1, '%2', '%3', %4 )").arg(newid).arg(name).arg(pswd1).arg(role));
    return ADD_SUCCESS;
}

int User::del()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec("select * from UserList where Name='"+this->name+"'");
    if(!query.first())
        return DELETE_ERROR_NO_USER;
    if(query.value(2).toInt()==1)
        return DELETE_ERROR_ADMIN;
    query.exec("delete from UserList where Name='"+this->name+"'");
    return DELETE_SUCCESS;
}

Info::Info(int _senderid, int _recipientid, QString _text)
{
    senderid=_senderid;
    recipientid=_recipientid;
    text=_text;
}

int Info::add()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from UserList where Id=%1").arg(recipientid));
    if(!query.first())
        return INFO_ERROR_NO_USER;
    query.exec("select * from InfoList");
    query.last();
    int newid = query.value(0).toInt()+1;
    query.exec(QString("insert into InfoList values( %1, %2, %3, '%4' )").arg(newid).arg(senderid).arg(recipientid).arg(text));
    return INFO_SUCCESS;
}
