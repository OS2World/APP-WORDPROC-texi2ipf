/*
 * texi2ipf.c - texi2ipf mainline
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
#ifndef __TURBOC__
#include <sys/types.h>
#endif
#include <sys/stat.h>
#include "texi2ipf.h"

char *progname;

/*
 * main - parse arguments, handle options
 *     - initialize tables and other strings
 *     - open files and pass them to process().
 */
int main(argc, argv)
int argc;
char *argv[];
{
    int errflg = 0;
    FILE *in;
    char *inname;
    int optind = 1;	/* to keep structure without using getopt() */

    extern int process();
    extern void initialize();

    progname = argv[0];

    if (errflg || argc < 2) {
       (void) fprintf(stderr,
           "\ntexi2ipf -=- Convert GNU texinfo files to OS/2 IPFC source code\n"
           "Based on texi2roff by Beverly A.Erlebacher (1990)\n"
           "Modified by Marcus Groeber 1993\n\n"
           "Usage: %s texi_file(s) [>ipfc_file]\n", progname);
       exit(1);
    }

    (void) initialize();

    if (optind >= argc) {
       errflg += process(stdin, "stdin");
       }
    else
       for (; optind < argc; optind++) {
           if (STREQ(argv[optind], "-")) {
               inname = "stdin";
               in = stdin;
               }
           else {
               if (( in = fopen(argv[optind], "rt")) == NULL) {
                   (void) fprintf(stderr,"%s : can't open file %s\n",
                           progname, argv[optind]);
                   continue;
               }
               inname = argv[optind];
           }
           errflg += process(in, inname);
           if (in != stdin)
               (void) fclose(in);
       }
    if(!errflg)
      puts(cmds->exit);
    exit(errflg);
}

/*
 * process -  check opened files and pass them to translate().
 *        -  report on disastrous translation failures
 */
int
process(fp, filename)
    FILE *fp;
    char *filename;
{
    struct stat statbuf;
    extern int translate(/* FILE *, char * */);

    if (fstat(fileno(fp), &statbuf) != 0){
       (void) fprintf(stderr,"%s : can't fstat file %s\n", progname,
                                                               filename);
       return 1;
    }
    if ((statbuf.st_mode & S_IFMT)==S_IFDIR) {
       (void) fprintf(stderr, "%s : %s is a directory\n", progname,
                                                               filename);
       return 1;
    }
    /* translate returns 0 (ok) or -1 (disaster). it isn't worthwhile
     * to try to recover from a disaster.
     */
    if (translate(fp, filename) < 0) {
       (void) fprintf(stderr,
               "%s: error while processing file %s, translation aborted\n",
               progname, filename);
       exit(1);
    }
    return 0;
}
