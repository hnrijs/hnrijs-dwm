static const char *user  = "nobody";
static const char *group = "nobody";

static const char *colorname[NUMCOLS] = {
    [INIT] =   "#000000",   /* after initialization */
    [INPUT] =  "#000000",   /* during input */
    [FAILED] = "#FFFFFF",   /* wrong password */
};
/* treat a cleared input like a wrong password */
static const int failonclear = 1;
