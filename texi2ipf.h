/*
 * texi2ipf.h - texi2ipf general header
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

/* troff macro packages supported */
#define NONE   0       /* dummy value for error detection */

/* useful confusion-reducing things */
#define STREQ(s,t) (*(s)==*(t) && strcmp(s, t)==0)
#define NO     0
#define YES    1
#define ERROR  (-1)

#define MAXLINELEN 1024
#define MAXILEVEL 10
#define MAXARG 5        /* Maximum number of arguments in xref etc */

/* tag table size */
#define MAXTAG 256
#define MAXTAGSIZE 16


/* miscellaneous troff command strings in macro header files. */
struct misccmds {
    char * init;       /* emit before the first input. this is the place to
                        * put troff commands controlling default point size,
                        * margin size, line length, etc.
                        */
    char * exit;       /* the opposite of init... */
    char * dfltpara;   /* emit when 2 consecutive newlines are detected */
                       /* in the input and the indentation level is <= 1. */
    char * dfltipara;  /* same but for indentation level > 1.  */
    char * dflttpara;  /* same but for two-column-table */
    char * indentstart; /* emit to increase indent level for itemized list */
    char * indentend;  /* emit to decrease indent level for itemized list */
    char * heading;    /* heading string, requires printf with arguments  
                        * for heading level (1..4) and modfiers (string)
                        */
};

extern struct misccmds * cmds;

struct tablerecd {
    char *  texstart;  /* starting token for a Texinfo command */
    char *  texend;    /* ending token for a Texinfo command */
    char *  trfstart;  /* troff commands to emit when texstart is found */
    char *  trfend;    /* troff commands to emit when texend is found */
    char *  font;      /* font in effect between trfstart & trfend */
    int  type;         /* kind of Texinfo command, as #defined below */
};

/* Texinfo command types */

#define ESCAPED    0  /* special character (special to Texinfo) */
#define INPARA     1  /* in-paragraph command */
#define PARAM      2  /* Parameter to structuring command follows */
#define DISCARD    3  /* not supported - discard following text */
#define PARAGRAPH  4  /* applies to following paragraph */
#define ITEMIZING  5  /* starts itemized list */
#define ITEM       6  /* item in list */
#define END        7  /* end construct */
#define CHAR       8  /* really special char: dagger, bullet - scary, eh? */
#define FOOTNOTE   9  /* footnote */
#define DISPLAY   10  /* text block of the kind called a 'display' */
#define INDEX     11  /* index entry */
#define INCLUDE   12  /* include file command */
#define SETCLEAR  13  /* SET or CLEAR command */
#define CONDITION 14  /* IFSET or IFCLEAR command */
#define NODE      15  /* NODE command */
#define XREF      16  /* cross reference */
#define COMMENT   17  /* comment: ignore rest of line */
#define HEADING1  18  /* chapter structuring commands */
#define HEADING2  19
#define HEADING3  20
#define HEADING4  21
#define TEMPLATE  22  /* template command */
#define TEMPLATE2 23  /* template command with cathegory */
#define VALUE     24  /* VALUE command */
                                                                                                                                                                                                                                             
/* itemizing types */
#define ITEMIZE    0
#define ENUMERATE  1
#define TABLE      2
#define APPLY      3

#include <string.h>
