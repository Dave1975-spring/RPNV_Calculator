# Compile and link with Open WATCOM C Compiler 1.9 with the command below, from the directoy 
# where *.c and *.h files are stored:

wcl *.c 

# the resulting .exe file is the file to run (not necessarely is named rpnv.exe).
# In FreeDOS, after installing OpenWATCOM C Compiler 1.9, prior to compiling command, 
# the environemnt has to be setup by running the file "owsetenv.bat" in the OpenWatcom directory,
#
# The command includes the following lines:

   @echo off

   if "%WATCOM%" == "" goto SetEnv
   if not exist %DOSDIR%\BIN\VFDUTIL.COM goto SetEnv
   VFDUTIL /T WFC.EXE /T WFL.EXE
   if errorlevel 1 goto SetEnv
   goto End

   :SetEnv
   echo Open Watcom C/C++ Build Environment
   SET PATH=C:\DEVEL\WATCOMC\BINW;%PATH%
   SET INCLUDE=C:\DEVEL\WATCOMC\H;%INCLUDE%
   SET WATCOM=C:\DEVEL\WATCOMC
   SET EDPATH=C:\DEVEL\WATCOMC\EDDAT
   SET WIPFC=C:\DEVEL\WATCOMC\WIPFC

   :End
