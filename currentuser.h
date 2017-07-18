#ifndef CURRENTUSER_H
#define CURRENTUSER_H

#define USERLOGOUT -1
class CurrentUser
{
    static int currentuserid;
 public:
    static void set_currentuser(int userid)
    {
        currentuserid = userid;
    }
    static void reset_currentuser()
    {
        currentuserid = USERLOGOUT;
    }
    static int get_currentuser()
    {
        return currentuserid;
    }
};

void set_currentuser(int userid);
void reset_currentuser();
int get_currentuser();

#endif // CURRENTUSER_H
