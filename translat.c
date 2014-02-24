/* 
 * translate.c - main guts of texi2ipf
 *
 * texi2roff history:
 *             Release 1.0a    August 1988
 *             Release 2.0     January 1990
 *
 * Copyright 1988, 1989, 1990  Beverly A.Erlebacher
 * erlebach@cs.toronto.edu    ...uunet!utai!erlebach
 *
 * texi2ipf history:
 *             Release 1.0     February 1993
 *
 * Modified by Marcus Gr”ber, Fido 2:2402/61.1
 *
 */

#include <stdio.h>
#include "texi2ipf.h"

#define FONTSTRSIZE 40

extern int what[MAXILEVEL];
int    displaylevel = 0;       /* nesting level of 'display' text */
int    inmacroarg = 0;         /* protect roff macro args flag */
int    ilevel = 0;             /* nesting level of itemized lists */
int    linecount;
int    newpara=-1,nopara=0,firstitem=0;
int    started = 0;            /* text has not yet started */
int    lastlevel = 1;          /* level of last heading */
int    vistext;                /* set if visible text encountered */

char   *filename;
char   *inp;                   /* pointer into input buffer */

char lastnode[MAXLINELEN]="\0";    /* last node id */
char macroarg[MAXARG][MAXLINELEN]; /* buffor for xref arguments */
int argmode=-1;

extern struct tablerecd * lookup();
extern char * itemize();
extern char * doitem();
extern char * value();
extern void setclear();

/* forward references */
extern char * gettoken();
extern char * eatwhitespace();
extern char * rtrim();
extern char * interpret();
extern int translate_line();

extern struct tablerecd tempframe, temp1stbrac, temp1stword;

/* 
 * errormsg - print error messages to stderr
 */

void
errormsg( message, other)
    char          *message;
    char          *other;
{
    (void) fprintf(stderr, "%s line %d : %s%s\n",
       filename, linecount, message, other);
}

/*
 * translate - translate one Texinfo file
 */

int
translate(in, inname)
    FILE          *in;
    char          *inname;
{
    char          input[MAXLINELEN];
    int           savlinct;
    char          *savfilnam;

    /*
     * save global variables linecount and filename in case this is a
     * recursive call to translate an @include file.  these variables
     * are used by errormsg() which is called from many places.
     */
    savfilnam = filename;
    savlinct = linecount;

    filename = inname;
    linecount = 0;

    /*
     * if fgets() truncates a line in the middle of a token, a blank will
     * appear in the word containing the MAXLINELENth char in the absurdly
     * long line. this is handled by gettoken()
     */

    while (fgets(input, MAXLINELEN, in) != NULL) {
       ++linecount;
       rtrim(input); 
       if (*input == '\0' && started)
           newpara = ilevel;            /* remeber paragraph */
       else
           if( translate_line (input) ) return ERROR;
    }
    /* restore the globals */
    filename = savfilnam;
    linecount = savlinct;
    return 0;
}

int
translate_line(input)
    char *input;
{
        char          output[MAXLINELEN * 2];
        char          token[MAXLINELEN];
        char          *c, *cprev;
        static char   lastchar='\n';
        int           redo;
 
 
        strcat(input,"\n");             /* Add end-of-line (has been removed) */

        do {
           inp = input;
           *output = '\0';
           vistext = NO;                /* No visibile text displayed yet */

           gettoken(NULL, NULL);        /* tell tab expander of newline */
           while (*inp != '\0') {
               inp = gettoken(inp, token);
               inp = interpret(token, output);
               if (inp == NULL)
                   return ERROR;
           }
           rtrim(output);               /* strip trailing whitespaces/returns */

           if(*lastnode && vistext && *output) {
                                        /* @node, but no title */
              sprintf(token,"@chapter %s",lastnode);
              translate_line(token);    /* Create artificial title */
              redo = 1;                 /* Line conversion might have changed... */
           }
           else
              redo = 0;
        } while (redo);
 
/*
 * output, stripping surplus newlines when possible.
 */
       if(newpara>=0 && *output && !nopara) {
                                        /* if not done, add paragraph tag */
           if(ilevel == newpara)
               (void) fputs(displaylevel?
                              "\n":
                            (newpara==0 || firstitem==0)?
                              cmds->dfltpara:
                            (what[ilevel]==TABLE || what[ilevel]==APPLY)?
                              cmds->dflttpara:
                            cmds->dfltipara,
                            stdout);
           newpara = -1;
           firstitem = 0;               /* more paragraphs may follow */
       }

       if(*output) {
         nopara = 0;                    /* maybe paragraph before next line */
         strcat(output,"\n");           /* one definitive return... */
       }

       cprev = &lastchar;   /* character at end of previous output buffer */
       for( c = output; *c != '\0'; cprev = c, ++c) {
           if (*c != '\n' || *cprev != '\n') {
               putc(*c, stdout);
           }
       }
       lastchar = *cprev;
       return 0;                        /* no error */
}

/*
 * PUSH - macro to push pointer to table entry onto command stack
 *       and current font onto font stack
 */

#define MAXDEPTH    20

#define PUSH(tptr)                                                     \
{   if (++stackptr == MAXDEPTH) {                                      \
       errormsg("stack overflow - commands nested too deeply", "");    \
       return NULL;                                                    \
    }                                                                  \
    stack[stackptr] = (tptr);                                          \
    (void) strcpy(fontstack[++fontptr], curfont);                      \
    if (*(tptr)->font != '\0' && !discarding)                          \
       (void) strcpy(curfont, (tptr)->font);                           \
}

#define OUTPUT(tptr)\
{   (void) strcat((argmode>=0 && argmode<MAXARG)?\
                     macroarg[argmode]:          \
                     outstring,                  \
                  (tptr));                       \
}

struct tablerecd brackets = {"{","}","","","",INPARA};


/*
 * interpret - interprets and concatenates interpreted token onto outstring
 */

char *
interpret(token, outstring)
char   *token;
char   *outstring;
{
    static struct tablerecd *stack[MAXDEPTH];
    static int    stackptr = 0; /* zeroth element is not used */
    static int    discarding = NO;
    static int    discardlevel = MAXDEPTH;
    static int    fontptr;
    static char   fontstack[MAXDEPTH][FONTSTRSIZE];
    static char   curfont[FONTSTRSIZE];
    static char   defaultfont[] = ":font facename=default size=0x0.";
    static int    init = NO;
    struct tablerecd *tptr,*tptr2;
    char           *s, *cp, tempstr[MAXLINELEN],itemtag[MAXLINELEN];
    int            level,i,retry;
    FILE           *fp;        /* for @include files */
    extern int     process();  /* for @include files */

    if (init == NO) {
       (void) strcpy(fontstack[0], defaultfont);
       (void) strcpy(curfont, defaultfont);
       fontptr = 0;
       init = YES;
    }

    if (*token == '@') {                /* attempt to look up texinfo cmds */
       if (!(tptr = lookup(token)) ) {  /* not found in present form? */
          cp = token + strlen(token) - 1;
          if (*cp == '{') {
             *cp = '\0';                /* try removing the trailing "{" */
             if ((tptr = lookup(token))) {
                                        /* valid without the "{"? */
                inp--;                  /* unget "{" char */
             } else {
                *cp = '{';              /* no use, replace for error message */
             } /* endif */
          }
       }
    }
    else 
       tptr = (*token == '{')? &brackets : NULL;

    s = inp;
    if (stackptr > 0 && STREQ(token, stack[stackptr]->texend)) {
    /* have fetched closing token of current Texinfo command */
     if (STREQ(token, "@end"))          /* get second half of end command */
         s = gettoken(eatwhitespace(s),tempstr);
     do {
       if (discarding && stackptr <= discardlevel) {
           discarding = NO;
           discardlevel = MAXDEPTH;
       }
       if (inmacroarg && stackptr <= inmacroarg) 
           inmacroarg = NO;
       retry = NO;                      /* no need roll back multiple @ends */
       if (STREQ(token, "@end")) {
           if(! STREQ(&(stack[stackptr]->texstart[1]), tempstr)) {
               errormsg("found unexpected @end ", tempstr);
               errormsg("skipped missing @end ",stack[stackptr]->texstart+1);
               retry = YES;             /* Skip back end */
           }
           if (stack[stackptr]->type == HEADING1 ) {
               started = NO;            /* end of titlepage - skip again */
           }
       }

/* End of something with arguments */ 
       if(stack[stackptr]->type == XREF || stack[stackptr]->type == NODE
          || stack[stackptr]->type == SETCLEAR) {
          if( argmode < MAXARG )
             rtrim(macroarg[argmode]);
          i = argmode;
          argmode = -1;
          switch(stack[stackptr]->type) {
          case XREF:
             if(!discarding && *macroarg[0]) {
                if( i >= 3 && *macroarg[3] ) {
                                        /* reference to another file */
                   if ((cp = strrchr(macroarg[3],'.')))
                     *cp = '\0';        /* remove extension ".info" */
                   sprintf(macroarg[1],"database='%s.INF'",macroarg[3]);
                                        /* create "database" option for link */
                }
                else
                   *macroarg[1]='\0';
                sprintf(outstring+strlen(outstring),
                        stack[stackptr]->trfstart,macroarg[1],macroarg[0]);
                OUTPUT(macroarg[(i>=2 && *macroarg[2])?2:0 ]);
                OUTPUT(stack[stackptr]->trfend);
             }
             break;

          case NODE:
             if(!discarding) strcpy(lastnode,macroarg[0]);
             break;

          case SETCLEAR:
             if (*macroarg[1]=='\0') {
                if (STREQ(stack[stackptr]->texstart,"@set")) {
                   strcpy(macroarg[1],"*");
                }
             }
             setclear(macroarg[0],macroarg[1]);
             break;
          }
       }             
       else if(!discarding 
               && (!inmacroarg || stack[stackptr]->type != INPARA)
               && (started || stack[stackptr]->type == PARAM))
           OUTPUT(stack[stackptr]->trfend);

       switch(stack[stackptr]->type) {
       case DISPLAY:
           --displaylevel;
           newpara = ilevel;
           nopara = 1;
           break;
       case ITEMIZING:
           --ilevel;
           newpara = ilevel;
           nopara = 1;
           if (!discarding && ilevel > 0)
               OUTPUT(cmds->indentend);
           break;
       case HEADING1:
       case HEADING2:
       case HEADING3:
       case HEADING4:
           newpara=ilevel;
           nopara=1;
           break;
       }

       if (--stackptr < 0) {
           errormsg("stack underflow", "");
           return NULL;
       }
       /* restore previous active font */
       if (STREQ(curfont, fontstack[fontptr]) == NO) {
               (void) strcpy(curfont, fontstack[fontptr]);
               if(!inmacroarg && started)
                    OUTPUT(curfont);    /* don't exec font changes in titles */
       }
       if (fontptr > 0)
               --fontptr;
     } while (stackptr 
              && ((*token == '\n' && *(stack[stackptr]->texend)=='\n')
                  || (retry && STREQ(stack[stackptr]->texend,"@end")))
             );
     if (retry) {                       /* cannot rollback @end's */
        errormsg("nesting level rollback failed","");
        return NULL;                    /* fatal error */
     } /* endif */
     if (STREQ(token, "@end"))
        return "";                      /* flush rest of line if any */
    } else if (*token == ',' && argmode>=0) {
       if (argmode < MAXARG)
          rtrim(macroarg[argmode]);
       if (++argmode < MAXARG)
          *macroarg[argmode]='\0';
    } else if (tptr == NULL) {          /* ordinary piece of text */
       if (*token == '@' && !discarding) {
          errormsg("unrecognized Texinfo command ", token);
          return "";
       }
       if (argmode >= 0 && argmode < MAXARG) {
           if (*token == '\n') {
              if(*macroarg[argmode])
                 strcat(macroarg[argmode], " ");
           }
           else if (*token != ' ' || *macroarg[argmode])
              strcat(macroarg[argmode], token);
       }
       else if (!discarding
                && argmode < 0
                && (started || (stackptr && stack[stackptr]->type == PARAM) || *lastnode)) {
           OUTPUT(token);
           if(!inmacroarg) vistext = YES;
       }
       if (*token == '\n')
           return "";
    } else {                           /* start of Texinfo command */
           switch (tptr->type) {
           case ESCAPED:
               if (!discarding && started)
                   OUTPUT(tptr->trfstart);
               break;

           case DISPLAY:
               ++displaylevel;
               PUSH(tptr);
               if (!discarding && displaylevel<2)
                   OUTPUT(tptr->trfstart);
               newpara = -1;
               break;

           case INDEX:
               nopara = 1;              /* move para tag after index entry */
           case PARAM:
               inmacroarg = stackptr+1; /* no font changes in parameters */
                       /* fall through */
           case PARAGRAPH:
           case CHAR:  /* may be some need to distinguish these in future */
           case INPARA:
           case FOOTNOTE:
           case TEMPLATE:
           case TEMPLATE2:
               s = eatwhitespace(s);
               PUSH(tptr);
               if (!discarding && !(tptr->type==INPARA && inmacroarg)) {
                 if (started || tptr->type == PARAM) {
                   if (tptr->type == TEMPLATE || tptr->type == TEMPLATE2) {
                     PUSH(&tempframe);
                     OUTPUT(temp1stword.trfstart);
                   }
                   OUTPUT(tptr->trfstart);
                   if (tptr->type == TEMPLATE) {
                     if (*s == '{') {
                        tptr2 = &temp1stbrac;
                        s++;
                     }
                     else
                        tptr2 = &temp1stword;
                     PUSH(tptr2);
                   }
                   else if (tptr->type == TEMPLATE2)
                     OUTPUT(temp1stword.trfend);
                 }
               }
               break;

           case HEADING1:
           case HEADING2:
           case HEADING3:
           case HEADING4:
               inmacroarg = stackptr+1; /* no font changes in headlines */
               s = eatwhitespace(s);
               PUSH(tptr);
               if (!discarding) {
                 level = tptr->type - HEADING1 + 1;
                 if(level - lastlevel > 1)
                   while (level-lastlevel>1) level--;
                                        /* Correct missing heading levels */
                 else
                   lastlevel = level;
                 if(*lastnode)
                    sprintf(tempstr," id=\'%s\'",lastnode);
                 else
                   *tempstr = '\0';
                 *lastnode = '\0';      /* @node command has been processed */
                 if (!started)
                     started = YES;     /* stop skipping over header */
                 else
                     OUTPUT(cmds->dfltpara);
                                        /* para, so panel is never empty */
                 newpara = -1;          /* swallow newlines at end of panel */
                 sprintf(outstring+strlen(outstring), cmds->heading, level, tempstr);
                 OUTPUT(tptr->trfstart);
               }
               break;

           case DISCARD:
               PUSH(tptr);
               if (!discarding) {
                   OUTPUT(tptr->trfstart);
                   discarding = YES;
                   discardlevel = stackptr;
               }
               break;

           case ITEMIZING:
               if (!discarding) {
                   OUTPUT(tptr->trfstart);
                   if (ilevel > 0)
                       OUTPUT(cmds->indentstart);
               }
               PUSH(tptr);
               ++ilevel;
               s = itemize(s, token);
               newpara=-1;
               break;

           case ITEM:
               PUSH(tptr);
               if (!discarding) {
                   OUTPUT(tptr->trfstart);
                   /* set up, parse and interpret item tag */
                   s = doitem(eatwhitespace(s),itemtag,STREQ(token,"@itemx"));
                   cp = itemtag;
                   while (*cp != '\0') {
                       cp = gettoken(cp, tempstr);
                       (void) interpret(tempstr, outstring);
                   }
                   if(what[ilevel]!=ITEMIZE && what[ilevel]!=ENUMERATE)
                     nopara=1;          /* delay until all item's are through */
                   newpara=ilevel;      /* add paragraph before description */
                   firstitem=1;         /* first paragraph in item follows */
               }
               break;

           case END:
               s = gettoken(eatwhitespace(s),tempstr);
               if (!discarding)
                  errormsg("unexpected @end found for Texinfo cmd @", tempstr);
               break;

           case INCLUDE:
               s = eatwhitespace(s);
               for (cp = tempstr; strchr(" \t\n",*s) == NULL; *cp++ = *s++)
                       ;
               *cp = '\0';
               if (!discarding) {
                 cp=strrchr(tempstr,'.');
                                        /* try to find "extension" dot */
/* The following statement (relying strongly on short-circuit evaluation)
   attempts to open first the specified filename, and then, all three
   .tex, .texi and .texinfo filenames, if one of these extensions was
   specified. This allows some files with original unix names to be
   compiled on FAT partitions with no modification. */
                 if ( (fp = fopen(tempstr, "r")) == NULL
                                        /* try original file name/not found: */
                      && (!cp           /* if extension is ".tex[i[nfo]]... */
                          || (!STREQ(cp,".tex") && !STREQ(cp,".texi")
                               && !STREQ(cp,".texinfo"))
                          || (!(strcpy(cp,".tex"),fp = fopen(tempstr,"r"))
                               && !(fp = fopen(strcat(tempstr,"i"),"r"))
                               && !(fp = fopen(strcat(tempstr,"nfo"),"r")) )))
                                        /* ...try all three extensions */
                     errormsg("can't open included file ", tempstr);
                 else {
                     (void) process(fp, tempstr);
                     (void) fclose(fp);
                 }
               }
               break;

           case SETCLEAR:
               PUSH(tptr);
               if(!discarding) {
                  s = eatwhitespace(gettoken(eatwhitespace(s),macroarg[0]));
                                        /* get tag name */
                  argmode = 1;          /* prepare for reading value */
                  *macroarg[argmode]='\0';
               }
               break;

           case VALUE:
               PUSH(tptr);
               if(!discarding) {
                  s = gettoken(eatwhitespace(s),tempstr);
                  cp = value(tempstr);
                  if(cp!=NULL) OUTPUT(cp);
               }
               break;

           case CONDITION:
               PUSH(tptr)
               if(!discarding)
                   s = gettoken(eatwhitespace(s),tempstr);
                   tempstr[MAXTAGSIZE-1]='\0';
                   cp = value(tempstr);
                   if((cp != NULL && *cp) ^ STREQ(token,"@ifclear")) {
                     OUTPUT(tptr->trfstart);
                   }
                   else {
                     discarding = YES;
                     discardlevel = stackptr;
                   }
               break;

           case NODE:
           case XREF:
               PUSH(tptr);
               if(!discarding) {
                   s = eatwhitespace(s);
                   argmode=0;
                   *macroarg[argmode]='\0';
               }
               break;

           case COMMENT:                /* avoid any mis-interpretation */
               return "";               /* leave immediately */

           default:
               /* can't happen */
               errormsg("ack ptui, what was that thing? ", token);
           }
    }
    return s;
}

/*
 * eatwhitespace - move input pointer to first char that isnt a blank or tab
 *     (note that newlines are *not* whitespace)
 */

char *
eatwhitespace(s)
    register char *s;
{
    while(*s == ' ' || *s == '\t')
       ++s ;
    return s;
}

/*  
 * rtrim - as in dbase - remove trailing whitespaces and returns
 */
char *
rtrim(s)
    char *s;
{
        int i;

        for(i=strlen(s)-1;i>=0 && (s[i]==' ' || s[i]=='\t' || s[i]=='\n');i--)
          s[i]='\0';
        return s;
}

/*
 * strpbrk_like - returns pointer to the leftmost occurrence in str of any
 *     character in set, else pointer to terminating null.
*/

char *
strpbrk_like(str, set)
    register char *str;
    char *set;
{
    static int inited_set = 0;
    static char set_vec[256] = { 0 };

    if (!inited_set) { /* we *know* it'll be the same every time... */
       while (*set)
           set_vec[(unsigned char)*set++] = 1;
       set_vec[0] = 1;
       inited_set = 1;
       }
    while (set_vec[(int)*str] == 0)
       ++str;
    return str;
}


/*
 * gettoken - fetch next token from input buffer. leave the input pointer
 *     pointing to char after token.    may need to be modified when
 *     new Texinfo commands are added which use different token boundaries.
 *
 *     will handle case where fgets() has split a long line in the middle
 *     of a token, but the token will appear to have been divided by a blank
 *
 *     will expand tab characters to "stupid" 8-character-tabs, as they
 *     would be created by an editor automatically "tabbing" lines.
 */

char *
gettoken(s, token)
    char          *s;
    char          *token;
{
    static char    endchars[] = " \n\t@{}:.*,";
    char           *q, *t;
    static int     xpos;

    if(s==NULL) {                       /* Init new line */
        xpos = 0;
        return NULL;
    }
    q = s;
    s = strpbrk_like(q, endchars);
    if (s != q) {
       switch (*s) {
       case ' ':
       case '\n':
       case '\t':
       case '@':
       case '}':
       case ':':
       case '.':
       case '*':
       case '\0':
       case ',':
           --s;
           break;
       case '{':
           break;
       }
    } else {   /* *s == *q */
       switch (*s) {
       case ' ':
       case '\n':
       case '\t':
       case '{':
       case ':':
       case '.':
       case '*':
       case '\0':
           break;
       case '}':
           if (*(s+1) == '{') /* footnotes with daggers and dbl daggers!! */
               ++s;
           break;
       case '@':
           s = strpbrk_like(q + 1, endchars );
           /* handles 2 char @ tokens: @{ @} @@ @: @. @* */
           if ( strchr("{}@:.*", *s) == NULL
                       || (s > q+1 && (*s =='}' || *s == '@')))
               --s;
           break;
       }
    }
    for (t = token; q <= s; ++q, ++t, ++xpos) {
       switch (*q) {
           case ':':
               strcpy(t,"&colon.");
               t+=6;
               break;
           case '&':
               strcpy(t,"&amp.");
               t+=4;
               break;
           case '.':
               strcpy(t,"&per.");
               t+=4;
               break;
           case '\0':
               *t = ' ';
               break;
           case '\t':
               do {
                  *(t++)=' ';           
               } while ((++xpos)%8);    /* file with blanks up to next 8-tab */
               t--; xpos--;             /* last inc will be done by loop */
               break;
           case '\'':
               if(argmode >= 0) {
                  *t = '_';             /* Change ' to _ in xref's */
                  break;
               }
           default   :
               *t = *q;
               break;
       }
    }
    *t = 0;
    return ++s;
}
