@ECHO OFF
REM Build Everything

ECHO "Building everything..."

CALL build.bat

IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies built successfully."