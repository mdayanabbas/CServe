
$dto(struct s_credentials, "cred")
struct s_credentials
{
    char username[16];
    char password[16];
};



$post(login,"/users/login")
bool login(Request *req , Response *res)
{
    char *name,*pass;

    name = req->cred->username;
    pass = req->cred->password;

    if(!strcmp(user,"ayan")
        && (!strcmp(pass,"abbas")))
        {
            req.session.loggedin = true;

            return json("{  'loggedin' : true}");
        }else{
            res.errorcode = 301;
            return httperrir(res,"Unauthorized");
        }
}