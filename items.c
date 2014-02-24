/*
 * items.c - handles itemized list commands (formerly part of translate.c)
 *           and @set / @clear tag lists
 *
 * texi2roff history:
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
#include <stdlib.h>
#include "texi2ipf.h"

int  icount[MAXILEVEL];
int  what[MAXILEVEL];
char item[MAXILEVEL][MAXLINELEN];

struct {
   char name[MAXTAGSIZE];
   char *value;
} tags[MAXTAG];                /* list of currently set tags */
int tagnum=0;                  /* number ob tags set */

extern int  ilevel;
extern char * gettoken();
extern char * eatwhitespace();
extern void errormsg();
extern struct tablerecd * lookup();

/*
 * itemize - handle the itemizing start commands @enumerate, @itemize
 *     and @table
 */

char * itemize(s, token)
char * s;
char * token;
{
    char *tag;

    tag = item[ilevel];
    if (STREQ(token,"@itemize")) {
       what[ilevel] = ITEMIZE;
       s = gettoken(eatwhitespace(s),tag);
       if (*tag == '\n') { /* this is an error in the input */
           --s;
           *tag = '-';
           errormsg("missing itemizing argument ","");
       } else {
           if (*tag =='@') {
               if ((lookup(tag)==NULL) && (lookup(strcat(tag,"{"))==NULL))
                    errormsg("unrecognized itemizing argument ",tag);
               else
                   if (*(tag + strlen(tag) - 1) == '{')
                       (void) strcat(tag,"}");
           }
       }
       (void) strcat(tag, " ");
    } else if (STREQ(token,"@enumerate")) {
       what[ilevel] = ENUMERATE;
       icount[ilevel] = 1;
    } else if (STREQ(token,"@table")) {
       what[ilevel] = TABLE;
       s = gettoken(eatwhitespace(s),tag);
       if (*tag == '\n') {
           *tag = '\0';  /* do nothing special */
           --s;
       } else {
           if (*tag =='@') {
               if ((lookup(tag)==NULL) && (lookup(strcat(tag,"{"))==NULL))
                   errormsg("unrecognized itemizing argument ",tag);
               else {
                   what[ilevel] = APPLY;
                   if (*(tag + strlen(tag) - 1) != '{')
                       (void) strcat(tag,"{");
               }
           }
       }
    }
    while (*s != '\n' && *s != '\0')
       ++s;  /* flush rest of line */
    return s;
}

/*
 * doitem - handle @item and @itemx
 */

char *
doitem(s, tag, itemx)
char * s;
char *tag;
int itemx;
{
        char *p;

    switch (what[ilevel]) {
    case ITEMIZE:
    case ENUMERATE:
       *tag=0;
       break;
    case TABLE:
       (void) strcpy(tag, itemx?"@br\n":"@_tag{pt}");
       tag+=strlen(tag);
       s = eatwhitespace(s);
       if (*s == '\n') {
           *tag++ = '-';
           errormsg("missing table item tag","");
       } else
           while(*s != '\n')
               *tag++ = *s++;
       *tag = '\0';
       break;
    case APPLY:
       (void) strcpy(p=tag, itemx?"@br\n":"@_tag{pt}");
       (void) strcat(tag,item[ilevel]);
       tag += strlen(tag);
       s = eatwhitespace(s);
       while(*s != '\n')
           *tag++ = *s++;
       *tag++ = '}';
       *tag = '\0';
       break;
    }
    return s;
}

/*
 * findtag - returns -1 if the tag has not been set,
 *           or the index of the tag in the tag list.
 */
int
findtag(str)
        char *str;
{
        int i;

        for(i=0;i<tagnum;i++)
           if(STREQ(str,tags[i].name))
             return i;
        return -1;
}

char *
value (str)
        char *str;
{
        int i;

        if ((i=findtag(str))==-1) {
           return NULL;
        } else {
           return tags[i].value;
        } /* endif */
}

/*
 * setclear - sets (value!="") or clears (value=="") a tag
 */
void
setclear(str,value)
        char *str;
        char *value;
{
        int i;

        if(*value) {                    /* SET tag */
           if ((i=findtag(str)) == -1 && tagnum<MAXTAG) 
              i=tagnum++;               /* Tag not found? Add tag */
           else
              free(tags[i].value);      /* Replace old tag value */
           tags[i].value=malloc(strlen(value)+1);
           strcpy(tags[i].name,str);
           strcpy(tags[i].value,value);
        }
        else {                          /* CLEAR tag */
           i=findtag(str);              /* tag set? */
           if(i!=-1) {                  /* yes, remove */
             free(tags[tagnum-1].value);
             memcpy(&tags[i],&tags[--tagnum],sizeof(*tags));
           }
        }
}
