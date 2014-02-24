@echo off
if not "%IPFC%"=="" goto IPFCSET
echo.
echo *Warning*: The IPFC environment variable should be set to the IPFC
echo subdirectory (normally in TOOLKT20) for IPFC to work correctly.
echo.
:IPFCset
if not "%1"=="" goto withArgs
echo Usage: %0 texinfo_file [!]
echo.
echo   Do *not* specify extension in texinfo filename.
echo   "!" option keeps the IPF source file.
goto End
:withArgs
if not exist %1.tex goto no_tex
%0pf %1.tex >%1.IPF
goto compile
:no_tex
if not exist %1.texi goto no_texi
%0pf %1.texi >%1.IPF
goto compile
:no_texi
if not exist %1.texinfo goto no_thing
%0pf %1.texinfo >%1.IPF
goto compile
:no_thing
echo %1.tex[i[nfo]] not found. Please do not specify an extension.
goto end
:compile
ipfc %1 /INF /S
if not "%2"=="!" del %1.IPF
:end