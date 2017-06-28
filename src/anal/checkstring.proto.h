
/* checkstring.c */
int setupgkword(gk_word *, char *string, PrntFlags prntflags);
int checkstring(char *, PrntFlags, FILE *);
int check_gkword(gk_word *, char *);
int checkstring1(gk_word *);
int checkstring2(gk_word *);
int checkstring3(gk_word *);
int has_cun(char *);
int checkapostr(gk_word *);
int has_tt(char *);
int SetWantDialect(Dialect);
int AddWantDialect(Dialect);
int ZapWantDialect(Dialect);
Dialect GetWantDialect(void);
int updateDialect(Dialect);
