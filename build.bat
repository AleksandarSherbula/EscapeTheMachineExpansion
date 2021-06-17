REM make sure the emscripten environment is set properly
call "D:\Projects\Third party Libraries\emsdk-2.0.23\emsdk_env.bat"

REM ensure folder exists for binary
md build\bin\Release\wasm

REM compile the individual files
call em++ -c src/Ending.cpp -o src/Ending.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17
call em++ -c src/Game.cpp -o src/Game.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17
call em++ -c src/Level.cpp -o src/Level.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17
call em++ -c src/main.cpp -o src/main.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17
call em++ -c src/Menu.cpp -o src/Menu.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17
call em++ -c src/Object.cpp -o src/Object.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17
call em++ -c src/olcPixelGameEngine.cpp -o src/olcPixelGameEngine.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17 -s USE_LIBPNG=1
call em++ -c src/Scoreboard.cpp -o src/Scoreboard.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17 -s ASYNCIFY -s ASYNCIFY_IMPORTS=[\"mwa_do_fetch\",\"mwa_get_user_agent\"]
call em++ -c src/StarMap.cpp -o src/StarMap.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17
call em++ -c src/TimeAttack.cpp -o src/TimeAttack.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17
call em++ -c src/TitleCard.cpp -o src/TitleCard.o -Iexternal/mwaWebApiClient/vendor -Isrc -std=c++17

REM link them and make binary
call em++ src/Ending.o src/Game.o src/Level.o src/main.o src/Menu.o src/Object.o src/olcPixelGameEngine.o src/Scoreboard.o src/StarMap.o src/TimeAttack.o src/TitleCard.o -o build/bin/Release/wasm/EscapeTheMachine.html -O3 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY -s ASYNCIFY_IMPORTS=[\"mwa_do_fetch\",\"mwa_get_user_agent\"] -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 --embed-file assets -std=c++17
pause