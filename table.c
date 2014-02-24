/*
 * table.c - set up translation tables for texi2roff
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
 *
 * When adding more commands:
 *
 * - be sure that gettoken() can recognize not just the ending token
 *   (texend) but also the end of the starting token (texstart) for
 *   the command, if it doesnt already occur in a table.
 *
 * - keep the tables sorted
 *
 * - try to keep all troff strings here and in the table header files
 *
 * - strive diligently to keep the program table-driven
 */

#include <stdio.h>     /* just to get NULL */
#include "texi2ipf.h"
#include "table.h"

char indexmacro[] = "";
char trquotes[] = "";

struct misccmds * cmds;
struct tablerecd * table, * endoftable;

void
initialize()
{
    long tablesize;

    table = mstable;
    tablesize = sizeof mstable;
    cmds = &mscmds;
    endoftable = table + tablesize/sizeof table[0];
    puts(cmds->init);
    puts(trquotes);
}


/*
 * lookup - linear search for texinfo command in table
 * i used bsearch() for a while but it isnt portable and makes lint squawk.
 */

struct tablerecd *
lookup(token)
char *token;
{
    struct tablerecd *tptr;

    for (tptr = table; tptr < endoftable; ++tptr) {
       if (STREQ(tptr->texstart, token))
           return tptr;
    }
    return NULL;
}
