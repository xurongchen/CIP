#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#define USERLOGOUT -1

class CurrentUser
{
    static int currentuserid;
 public:
    static void set_currentuser(int userid);
    static void reset_currentuser();
    static int get_currentuser();
};


static void CurrentUser::set_currentuser(int userid)
{
    CurrentUser.currentuserid = userid;
}

static void CurrentUser::reset_currentuser()
{
    CurrentUser.currentuserid = USERLOGOUT;
}

static int CurrentUser::get_currentuser()
{
    return CurrentUser.currentuserid;
}

#endif // CURRENTUSER_H
