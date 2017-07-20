#include "dboperation.h"
#include <QDateTime>
#include "currentuser.h"

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
    if(this->name==NULL)
        return ADD_ERROR_USERNAME_NULL;
    if(this->pswd1!=this->pswd2)
        return ADD_ERROR_DIFF_PASSWORD;
    if(this->pswd1==NULL)
        return ADD_ERROR_PASSWORD_NULL;
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from UserList where Name='%1'").arg(name));
    if(query.first())
        return ADD_ERROR_SAME_USERNAME;
    query.exec("select * from UserList");
    query.last();
    int newid = query.value(0).toInt()+1;
    query.exec(QString("insert into UserList values( %1, '%2', '%3', %4 )")
               .arg(newid).arg(name).arg(pswd1).arg(role));
    return ADD_SUCCESS;
}

int User::del()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec("select * from UserList where Name='"+this->name+"'");
    if(!query.first())
        return DELETE_ERROR_NO_USER;
    if(query.value(3).toInt()==1)
        return DELETE_ERROR_ADMIN;
    query.exec("delete from UserList where Name='"+this->name+"'");
    return DELETE_SUCCESS;
}

int User::query()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec("select * from UserList where Name='"+this->name+"'");
    if(!query.first())
        return QUERY_NO_USER;
    else return query.value(0).toInt();
}

Info::Info(int _senderid, int _recipientid, QString _text)
{
    senderid=_senderid;
    recipientid=_recipientid;
    text=_text;
}
Info::Info(int _id)
{
    id = _id;
}

int Info::add()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from UserList where Id=%1").arg(recipientid));
    if(recipientid!=-1&&!query.first())
        return INFO_ERROR_NO_USER;
    query.exec("select * from InfoList");
    query.last();
    int newid = query.value(0).toInt()+1;
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    query.exec(QString("insert into InfoList values( %1, %2, %3, '%4', %5, '%6')")
               .arg(newid).arg(senderid).arg(recipientid).arg(str).arg(senderid).arg(text));
    if(recipientid!=-1&&senderid!=recipientid)
    query.exec(QString("insert into InfoList values( %1, %2, %3, '%4', %5, '%6')")
               .arg(newid+1).arg(senderid).arg(recipientid).arg(str).arg(recipientid).arg(text));
    return INFO_SUCCESS;
}

int Info::del(INFODELTYPE _INFODELTYPE)
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    if(_INFODELTYPE==RECIDEL)
    {
        QSqlQuery tmpquery(db);
        tmpquery.exec(QString("select * from InfoList where Id=%1").arg(id));
        tmpquery.first();
        if(tmpquery.value(2)==-1)
            return INFO_DEL_FAILED_NO_PERMISSION;
    }
    query.exec(QString("delete from InfoList where Id=%1").arg(id));
    return INFO_DEL_SUCCESS;
}

void Current_user_send_Info_clear()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("delete from InfoList where OwnerId=%1 and SenderId=%2")
               .arg(get_currentuser()).arg(get_currentuser()));
}

void Current_user_reci_Info_clear()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("delete from InfoList where OwnerId=%1 and RecipientId=%2")
               .arg(get_currentuser()).arg(get_currentuser()));
}

Insurance::Insurance(QString _name, int _fixedcost, double _floatcost)
{
    name = _name;
    fixedcost = _fixedcost;
    floatcost = _floatcost;
}

int Insurance::add()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec("select * from InsuranceList");
    query.last();
    int newid = query.value(0).toInt()+1;
    query.exec(QString("insert into InsuranceList values( %1, '%2', %3, %4 ,1)")
               .arg(newid).arg(name).arg(fixedcost).arg(floatcost));
    return INSURANCE_ADD_SUCCESS;
}

Policy::Policy(QString _policyid, QString _clientname, QString _clientcard,
               QString _carnumber, QString _carstyle,
               int _carvalue, double _discount, QDate _startdate)
{
    policyid = _policyid;
    clientname = _clientname;
    clientcard = _clientcard;
    carnumber = _carnumber;
    carstyle = _carstyle;
    carvalue = _carvalue;
    discount = _discount;
    startdate = _startdate;
}

Policy::Policy(int _id)
{
    id = _id;
}

int Policy::add(int& Policyid)
{
    if(policyid==NULL||clientname==NULL||clientcard==NULL||carnumber==NULL||carstyle==NULL)
        return POLICY_ADD_ERROR_IMPORTANT_INFO_EMPTY;
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("select * from PolicyList where PolicyId='%1'").arg(policyid));
    if(query.first())
       return POLICY_ADD_ERROR_SAME_POLICYID;
    query.exec("select * from PolicyList");
    query.last();
    int newid = query.value(0).toInt()+1;
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd"); //设置显示格式
    QString str2 = startdate.toString("yyyy-MM-dd"); //设置显示格式
    query.exec(QString("insert into PolicyList values"
                       "( %1, '%2', '%3', '%4', '%5', '%6', %7, '%8', '%9', %10, %11)")
               .arg(newid).arg(policyid).arg(clientname).arg(clientcard).arg(carnumber).arg(carstyle)
               .arg(carvalue).arg(str).arg(str2).arg(discount).arg(get_currentuser()));
    Policyid = newid;
    return POLICY_ADD_SUCCESS;
}

int Policy::del()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec(QString("delete from PolicyList where Id=%1").arg(id));
    return POLICY_DEL_SUCCESS;
}

PolicyInsurance::PolicyInsurance(int _policyid, int _insuranceid)
{
    policyid = _policyid;
    insuranceid = _insuranceid;
}

int PolicyInsurance::add()
{
    QSqlDatabase db = QSqlDatabase::database("connection");
    QSqlQuery query(db);
    query.exec("select * from PolicyInsuranceRelationList");
    query.last();
    int newid = query.value(0).toInt()+1;
    query.exec(QString("insert into PolicyInsuranceRelationList values( %1, %2, %3 )")
               .arg(newid).arg(policyid).arg(insuranceid));
    return POLICY_INSURANCE_ADD_SUCCESS;
}
