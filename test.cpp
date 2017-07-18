#include "test.h"

void test()
{
    db_user_add_test();
    //db_user_delete_test();
    //db_info_add_test();
    //db_insurance_add_test();
}


void db_user_add_test()
{
    QString str=QString::fromLocal8Bit("王心亮");
    User u1(str,"123","123",2);
    u1.add();
    User u2("ff","3321","3321",3);
    u2.add();
}
void db_user_delete_test()
{
    User u2("王心亮");
    u2.del();
}

void db_info_add_test()
{
    Info i1(0,1,"dsafalksdjlkjf");
    i1.add();
}

void db_insurance_add_test()
{
    Insurance i1("First insurance",199,13);
    i1.add();
}
