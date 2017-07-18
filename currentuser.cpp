#include "currentuser.h"

int CurrentUser::currentuserid = -1;
void set_currentuser(int userid)
{
    CurrentUser::set_currentuser(userid);
}

void reset_currentuser()
{
    CurrentUser::reset_currentuser();
}
int get_currentuser()
{
    return CurrentUser::get_currentuser();
}
