/*
 * table.h -  table and other explicit ipfc command strings
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
 * Modified by Marcus Gr�ber, Fido 2:2402/61.1
 *
 */

#define FONT_MONO ":font facename=Courier size=16x8."
#define EFNT_MONO ":font facename=default size=0x0."

/*
 * miscellaneous commands, described in texi2roff.h
 */
struct misccmds mscmds = { 
    ":userdoc.\n:docprof toc=1234.",    /* init string */
    ":euserdoc.",                       /* end string */
    ":p.",                              /* text paragraph */
    ":li.",                             /* list item */
    ":pd.",                             /* paragraph inside list item */
    "",
    "",
    "\n:h%d%s."                         /* heading */
};

struct tablerecd
  tempframe =   { "","\n","",EFNT_MONO"\n.br\n",FONT_MONO,INPARA },
  temp1stbrac = { "","}",":hp2.","&colon.:ehp2."FONT_MONO,"",INPARA },
  temp1stword = { ""," ",":hp2.","&colon.:ehp2. "FONT_MONO,"",INPARA };

struct tablerecd mstable[] = {
    "@",               "",     "@",            "",     "",     ESCAPED,
    "@&colon.",        "",     "",             "",     "",     ESCAPED,
    "@&per.",          "",     "&per.",        "",     "",     ESCAPED,
    "@*",              "",     "\n.br\n",      "",     "",     ESCAPED,
    "@@",              "",     "@",            "",     "",     ESCAPED,
    "@Arrow",          "\n",   "->",           "",     "",     CHAR,
    "@TeX{",           "}",    "TeX",          "",     "",     INPARA,
    "@appendix",       "\n",   "",             "\n",   "",     HEADING1,
    "@appendixsec",    "\n",   "",             "\n",   "",     HEADING2,
    "@appendixsection","\n",   "",             "\n",   "",     HEADING2,
    "@appendixsubsec", "\n",   "",             "\n",   "",     HEADING3,
    "@appendixsubsubsec","\n", "",             "\n",   "",     HEADING4,
    "@arrow",          "\n",   "->",           "\n",   "",     CHAR,
    "@asis",           "\n",   "",             "\n",   "",     PARAGRAPH,
    "@asis{",          "}",    "",             "",     "",     INPARA,
    "@author",         "\n",   "\n:lines align=center.\n",
                               "\n:elines.\n", "",             DISPLAY,
    "@author{",        "}",    "\n:lines align=center.\n",
                               "\n:elines.\n", "",             DISPLAY,
    "@br",             "\n",   "\n.br\n",      "",     "",     PARAGRAPH,
    "@bullet{",        "}",    "&sqbul.",      "",     "",     CHAR,
    "@bye",            "\n",   "",             "",     "",     DISCARD,
    "@b{",             "}",    ":hp2.",        ":ehp2.","",    INPARA,
    "@c",              "\n",   "",             "",     "",     COMMENT,
    "@cartouche",      "@end", "",             "",     "",     PARAGRAPH,
    "@center",         "\n",   "\n:lines align=center.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@center{",        "}",    "\n:lines align=center.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@chapter",        "\n",   "",             "\n",   "",     HEADING1,
    "@cindex",         "\n",   "\n:i1.",       "\n",   "",     INDEX,
    "@cite{",          "}",    ":hp1.",        ":ehp1.","",    INPARA,
    "@clear",          "\n",   "",             "",     "",     SETCLEAR,
    "@code",           "\n",   FONT_MONO,      "\n",   FONT_MONO,   
                                                               INPARA,
    "@code{",          "}",    FONT_MONO,      "",     FONT_MONO,   
                                                               INPARA,
    "@comment",        "\n",   "",             "",     "",     COMMENT,
    "@contents",       "\n",   "",             "",     "",     INPARA,
    "@copyright{",     "}",    "(C)",          "",     "",     CHAR,
    "@ctrl{",          "}",    FONT_MONO"^",   "",     FONT_MONO,INPARA,
    "@dag",            "}{",   "+",            "",     "",     CHAR,
    "@dag{",           "}",    "+",            "",     "",     CHAR,
    "@ddag",           "}{",   "++",           "",     "",     CHAR,
    "@ddag{",          "}",    "++",           "",     "",     CHAR,
    "@defcmd",         "@end", "-- Command",   "",     "",     TEMPLATE2,
    "@defcmdx",        "\n",   "-- Command",   "",     "",     TEMPLATE2,
    "@defconst",       "@end", "-- Constant",  "",     "",     TEMPLATE2,
    "@defconstx",      "\n",   "-- Constant",  "",     "",     TEMPLATE2,
    "@defcv",          "@end", "-- ",          "",     "",     TEMPLATE,
    "@defcvx",         "\n",   "-- ",          "",     "",     TEMPLATE,
    "@deffn",          "@end", "-- ",          "",     "",     TEMPLATE,
    "@deffnx",         "\n",   "-- ",          "",     "",     TEMPLATE,
    "@defmac",         "@end", "-- Macro",     "",     "",     TEMPLATE2,
    "@defmacx",        "\n",   "-- Macro",     "",     "",     TEMPLATE2,
    "@defmethod",      "@end", "-- Method",    "",     "",     TEMPLATE2,
    "@defmethodx",     "\n",   "-- Method",    "",     "",     TEMPLATE2,
    "@defopt",         "@end", "-- User Option","",    "",     TEMPLATE2,
    "@defoptx",        "\n",   "-- User Option","",    "",     TEMPLATE2,
    "@defspec",        "@end", "-- Special Form","",   "",     TEMPLATE2,
    "@defspecx",       "\n",   "-- Special Form","",   "",     TEMPLATE2,
    "@deftypefn",      "@end", "-- ",          "",     "",     TEMPLATE,
    "@deftypefnx",     "\n",   "-- ",          "",     "",     TEMPLATE,
    "@deftypefun",     "@end", "-- Function",  "",     "",     TEMPLATE2,
    "@deftypefunx",    "\n",   "-- Function",  "",     "",     TEMPLATE2,
    "@deftypevar",     "@end", "-- Variable",  "",     "",     TEMPLATE2,
    "@deftypevarx",    "\n",   "-- Variable",  "",     "",     TEMPLATE2,
    "@deftypevr",      "@end", "-- ",          "",     "",     TEMPLATE,
    "@deftypevrx",     "\n",   "-- ",          "",     "",     TEMPLATE,
    "@defun",          "@end", "-- Function",  "",     "",     TEMPLATE2,
    "@defunx",         "\n",   "-- Function",  "",     "",     TEMPLATE2,
    "@defvr",          "@end", "-- ",          "",     "",     TEMPLATE,
    "@defvrx",         "\n",   "-- ",          "",     "",     TEMPLATE,
    "@defvar",         "@end", "-- Variable",  "",     "",     TEMPLATE2,
    "@defvarx",        "\n",   "-- Variable",  "",     "",     TEMPLATE2,
    "@dfn{",           "}",    ":hp1.",        ":ehp1.","",    INPARA,
    "@display",        "@end", "\n:lines.\n",  "\n:elines.\n",
                                                       "",     PARAGRAPH,
    "@dmn{",           "}",    "",             "",     "",     INPARA,
    "@dots{",          "}",    "&per.&per.&per.","",   "",     INPARA,
    "@eject",          "\n",   "",             "",     "",     DISCARD,
    "@emph{",          "}",    ":hp1.",        ":ehp1.","",    INPARA,
    "@end",            "\n",   "",             "",   "",       END,
    "@enumerate",      "@end", "\n:ol.\n",     "\n:eol.\n","", ITEMIZING,
    "@equiv{",         "}",    "==",           "",     "",     CHAR,
    "@error{",         "}",    "error-->",     "",     "",     CHAR,
    "@example",        "@end", "\n:xmp.\n",    "\n:exmp.\n",
                                                       FONT_MONO,DISPLAY,    
    "@example{",       "}",    FONT_MONO,      "",     FONT_MONO,INPARA,
    "@exdent",         "\n",   "",             "\n",   "",     PARAGRAPH,
    "@expansion{",     "}",    "->",           "",     "",     CHAR,
    "@file{",          "}",    FONT_MONO"`",   "'",    FONT_MONO,INPARA,
    "@finalout",       "\n",   "",             "\n",   "",     DISCARD,
    "@findex",         "\n",   "\n:i1.",       "\n",   "",     INDEX,
    "@flushleft",      "@end", "\n:lines align=left.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@flushright",     "@end", "\n:lines align=right.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@format",         "@end", "",             "",     "",     PARAGRAPH,
    "@footnote{",      "}",    " (",           ") ",   "",     FOOTNOTE,
    "@ftable",         "@end", "\n:parml.\n",  "\n:eparml.\n","",
                                                               ITEMIZING,
    "@group",          "@end", "\n",           "\n",   "",     PARAGRAPH,
    "@heading",        "\n",   "",     "\n",   "",     HEADING2,
    "@headings",       "\n",   "",             "",     "",     DISCARD,
    "@iappendix",      "\n",   "",     "\n",   "",     HEADING1,
    "@iappendixsec",   "\n",   "",     "\n",   "",     HEADING2,
    "@iappendixsubsec","\n",   "",     "\n",   "",     HEADING3,
    "@ichapter",       "\n",   "",     "\n",   "",     HEADING1,
    "@ifclear",        "@end", "",             "",     "",     CONDITION,
    "@ifinfo",         "@end", "\n",           "\n",   "",     PARAGRAPH,
    "@ifset",          "@end", "",             "",     "",     CONDITION,
    "@iftex",          "@end", "",             "",     "",     DISCARD,
    "@ignore",         "@end", "",             "",     "",     DISCARD,
    "@include",        "\n",   "",             "\n",   "",     INCLUDE,
    "@inforef{",       "}",    "See Info file ","",    "",     PARAGRAPH,
    "@isection",       "\n",   "",     "\n",   "",     HEADING2,
    "@isubsection",    "\n",   "",     "\n",   "",     HEADING3,
    "@isubsubsection", "\n",   "",     "\n",   "",     HEADING4,
    "@item",           "\n",   "",             "",   "",       ITEM,
    "@itemize",        "@end", "\n:ul.\n",     "\n:eul.\n","", ITEMIZING,
    "@itemx",          "\n",   "",             "",   "",       ITEM,
    "@iunnumbered",    "\n",   "",     "\n","",        HEADING1,
    "@iunnumberedsec", "\n",   "",     "\n","",        HEADING2,
    "@iunnumberedsubsec","\n", "",             "\n",   "",     HEADING3,
    "@i{",             "}",    ":hp1.",        ":ehp1.","",    INPARA,
    "@kbd{",           "}",    FONT_MONO,      "",     FONT_MONO,INPARA,
    "@key{",           "}",    FONT_MONO,      "",     FONT_MONO,INPARA,
    "@kindex",         "\n",   "\n:i1.",       "\n",   "",     INDEX,
    "@lisp",           "@end", "\n:xmp.\n",    "\n:exmp.\n",FONT_MONO,DISPLAY,
    "@majorheading",   "\n",   "",     "\n",   "",     HEADING1,
    "@menu",           "@end", "",             "",     "",     DISCARD,
    "@minus{",         "}",    "-",            "",     "",     CHAR,
    "@need",           "\n",   "",             "",     "",     DISCARD,
    "@node",           "\n",   "",             "",     "",     NODE,
    "@noindent",       "\n",   "\n",           "",     "",     PARAGRAPH,
    "@page",           "\n",   ":p.:p.\n",     "",     "",     PARAGRAPH,
    "@pindex",         "\n",   "\n:i1.",       "\n",   "",     INDEX,
    "@point{",         "}",    "-!-",          "",     "",     CHAR,
    "@printindex",     "\n",   "Sorry, no ",   " index\n","",  PARAGRAPH,
    "@prints{",        "}",    "-|",           "",     "",     CHAR,
    "@print{",         "}",    "-|",           "",     "",     CHAR,
    "@pxref{",         "}",    "see :link reftype=hd %s refid='%s'.",
                                               ":elink.","",   XREF,
    "@quotation",      "@end", "",             "",     "",     PARAGRAPH,
    "@refill",         "\n",   "",             "\n",   "",     DISCARD,
    "@ref{",           "}",    ":link reftype=hd %s refid='%s'.",
                                               ":elink.","",   XREF,
    "@result",         "\n",   "=>",           "\n",   "",     CHAR,
    "@result{",        "}",    "=>",           "",     "",     CHAR,
    "@r{",             "}",    EFNT_MONO,      "",     EFNT_MONO,     
                                                               INPARA,
    "@samp{",          "}",    FONT_MONO"`",   "'",    FONT_MONO,INPARA,
    "@sc{",            "}",    ":hp2.",        ":ehp2.","",    INPARA,
    "@section",        "\n",   "",     "\n",   "",     HEADING2,
    "@set",            "\n",   "",             "",     "",     SETCLEAR,
    "@setchapternewpage","\n", "",             "\n",   "",     DISCARD,
    "@setfilename",    "\n",   "",             "",     "",     DISCARD,
    "@settitle",       "\n",   ":title.",      "\n",   "",     PARAM,
    "@smallbook",      "\n",   "",             "\n",   "",     DISCARD,
    "@smallexample",   "@end", "\n:xmp.\n",    "\n:exmp.\n",FONT_MONO,
                                                               DISPLAY,
    "@sp",             "\n",   "\n:p.",        "",     "",     DISCARD,
    "@sp2",            "\n",   "\n:p.",        "",     "",     DISCARD,
    "@strong{",        "}",    ":hp2.*",       "*:ehp2.","",   INPARA,
    "@subheading",     "\n",   "",     "\n",   "",     HEADING3,
    "@subsection",     "\n",   "",     "\n",   "",     HEADING3,
    "@subsubheading",  "\n",   "",     "\n",   "",     HEADING4,
    "@subsubsection",  "\n",   "",     "\n",   "",     HEADING4,
    "@subtext",        "\n",   "\n:lines align=center.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@subtext{",       "}",    "\n:lines align=center.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@subtitle",       "\n",   "\n:lines align=center.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@subtitle{",      "}",   "\n:lines align=center.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@summarycontents","\n",   "",             "",     "",     INPARA,
    "@syncodeindex",   "\n",   "",             "\n",   "",     DISCARD,
    "@synindex",       "\n",   "",             "\n",   "",     DISCARD,
    "@t",              "\n",   "\t",           "\n",   "",     CHAR,
    "@table",          "@end", "\n:parml.\n",  "\n:eparml.\n","", 
                                                               ITEMIZING,
    "@tex",            "@end", "",             "",     "",     DISCARD,
    "@tindex",         "\n",   "\n:i1.",       "\n",   "",     INDEX,
    "@title",          "\n",   "\n:lines align=center.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@title{",         "}",    "\n:lines align=center.\n",
                               "\n:elines.\n", "",             PARAGRAPH,
    "@titlefont{",     "}",    "",             "",     "",     INPARA,
    "@titlepage",      "@end", "Title page",   "",     "",     HEADING1,
    "@titlespec",      "@end", "",             "",     "",     PARAGRAPH,
    "@top",            "\n",   "Top node: \"", "\"\n", "",     HEADING1,
    "@t{",             "}",    FONT_MONO,      "",     FONT_MONO,INPARA,
    "@tt{",            "}",    FONT_MONO,      "",     FONT_MONO,INPARA,
    "@unnumbered",      "\n",  "",             "\n",   "",     HEADING1,
    "@unnumberedsec",   "\n",  "",             "\n",   "",     HEADING2,
    "@unnumberedsubsec","\n",  "",             "\n",   "",     HEADING3,
    "@unnumberedsubsubsec","\n","",            "\n",   "",     HEADING4,
    "@value{",         "}",    "",             "",     "",     VALUE,
    "@var{",           "}",    ":hp1.",        ":ehp1.","",    INPARA,
    "@vfil",           "\n",   "",             "\n",   "",     DISCARD,
    "@vindex",         "\n",   "\n:i1.",       "\n",   "",     INDEX,
    "@vskip",          "\n",   "",             "\n",   "",     DISCARD,
    "@vtable",         "@end", "\n:parml.\n",  "\n:eparml.\n","",
                                                               ITEMIZING,
    "@w{",             "}",    "",             "",     "",     INPARA,
    "@xref{",          "}",    "See :link reftype=hd %s refid='%s'.",
                                               ":elink.","",   XREF,
/* the following macro is for internal communication between modules only */
    "@_tag{",          "}",    ":",            ".",    "",     CHAR,

    "@{",              "",     "{",            "",     "",     ESCAPED,
    "@}",              "",     "}",            "",     "",     ESCAPED,
};
