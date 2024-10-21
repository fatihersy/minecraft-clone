REM Build script for testbed
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files.
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%

SET assembly=main
SET compilerFlags=-g 
REM -Wall -Werror
SET includeFlags=-Isrc -I./vendor/include/
SET linkerFlags=-L./vendor/lib/ -L./bin/ -lopengl32.lib -luser32.lib -lcglm.lib -lshell32.lib -lgdi32.lib -lglfw3dll.lib
SET defines=-D_DEBUG -DKIMPORT

ECHO "Building %assembly%%..."
clang %cFilenames% %compilerFlags% -o ./bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%