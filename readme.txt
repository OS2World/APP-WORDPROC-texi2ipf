texi2ipf - Texinfo to OS/2 IPFC source file converter
-----------------------------------------------------

        This program is a modified version of texi2roff.
        texi2roff is  Copyright  1988, 1989, 1990 by  Beverly A. Erlebacher

        texi2ipf was written by Marcus Groeber 1992/93

Very soon after the first GNU ports for OS/2 2.0 were out, some
people asked for a simple way to make use of the texinfo manuals,
especially those included with the various GCC ports.

Having seen some examples of OS/2's excellent Information
Presentation Facility (some would call it a help system) I had the
idea that porting the GNU manuals to a graphical viewer would be
much more aedequate for a graphical user interface than a text based
reader.

The texi2roff translator provided me with the basics of reading and
interpreting texinfo files. But the original texi2roff manual was 
right in saying that "texinfo remained a moving target" - since the
completion of the 1990 release of texi2roff (which was the lastet
I could find), a number of new additions the the texinfo package
had been made [such as conditional compiling].

Also, porting texinfo to a viewing system capable of "hypertext"
functions (i.e. cross references) meant having to add extended
support for those commands that were specific to the "info" version
of a texinfo file (as opposed to the "TeX" version).

Therefore, the source code to this program is a complete mixture of
original and modified routines down to expression level, which is
why I did not create a seperate diff file.


As the IPFC compiler from the Toolkit is absolutely required for
creating INF files, most end users won't be able to convert
texinfo files to INF themselves. Therefore, people doing GNU ports
for OS/2 should probably best do the conversion themselves and
include INF version manuals in their distribution.

It should be stressed that this program is *not* a general
TeX-to-something converter; even certain texinfo styles will not
be recognized properly, even though they might produce correct
TeX output. Especially questions like font changes inside other
statements (document titles, index entries etc.) are not even
handled consistently by texinfo authors.


What you need
-------------
To use this program, you should have the IPFC program from the
OS/2 2.0 development toolkit installed. As far as I know, this
program is not available separately, but it is included on the
public beta versions of the Toolkit as well.

I tested the included source files with the emx/gcc 0.8f port of the
Gnu C compiler. The other port ("GCC/2") should work also, but
you might have to change the make files. See below for further notes
on recompiling the program.


Usage
-----
There are no tricky options, just type 

	texi2ipf texinfo_file >IPF_file

to convert a given texinfo file to an IPF source file that can then
be compiled with the IPFC compiler. If the texinfo file includes
other files (as the GCC manual does), these should be in the current
directory. Output will always be in one big file. IPF filenames
should usually end in .IPF.

To compile the IPF file to a viewable INF database, type:

	ipfc /INF IPF_file

This creates a file with the same name as the IPF file, but with a
.INF extension.

Both commands can produce a number of warnings. With text based
formats that are as flexible as texinfo and IPF, complete
transformation is very hard to achieve. Anyway, most warnings
will only point at local problems in the data (such as unrecognized
commands) - the resulting database will still be usable in most
cases.

After compiling you can have a look at the database by typing

	view INF_file

If the database appears correct, you can delete the IPF source file.
If not, you can still try to patch out any problems reported.

To facilitate the translation process, a CMD file has been included
that automatically calls texi2ipf and ipfc. It is called texi2i
and should not be renamed. To use it, it must reside in the same
directory as texi2ipf, and ipfc must be on the PATH.

Typing

	texi2i texinfo_file

[texinfo_file must be specified without extension; names ending in
.tex .texi .texinfo are automatically found] converts and compiles
the texinfo file to an INF database and deletes the temporary
IPF source code. If you add a "!" as second parameter, the IPF
source file is preserved.


Texinfo support
---------------
The program has been tested with a number of texinfo files,
especially those that come with the gcc distribution, and most
compile with little or no errors and warnings [some of which
result from real problems in the original info files]. If you
come across a file which does not compile with texi2ipf, I'd
appreciate a short note to my Fidonet adress [see below].

The latest Texinfo version at the time of this release was 2.16
from October 1992.


Making changes to the source
----------------------------
The program was compiled using emx/gcc 0.8f with the Toolkit nmake
program [together with the toolkit headers, but these should not
make any difference as this is a commandline-only application that
uses no OS/2 specific features]. To compile the executable with the
included makefile, you'll have to build the omf libraries for
emx/gcc first, or you can omit the "-Zomf" parameter in the
compiler flags.

The source has not been completely "brushed over" to remove all
lines the have become obsolete with the changes from texi2roff,
and it may not be fully "bullet-proof" in extreme circumstances
(recursive include files, excessive nesting etc.) either, but as
this is not a program for everyday use, this seemed
appropriate to me.


Portability
-----------
None. :-) I haven't used any OS/2 specific functions, but as the
destination format is OS/2 specific, I did not take any precautions
to ensure portability to other environments.


Please adress comments, suggestions etc. to:
Marcus Groeber, Fido 2:2402/61.1
