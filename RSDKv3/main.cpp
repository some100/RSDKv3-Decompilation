#include "RetroEngine.hpp"

#if !RETRO_USE_ORIGINAL_CODE

#if RETRO_PLATFORM == RETRO_WIN && _MSC_VER
#include "Windows.h"
#endif

#if RETRO_PLATFORM == RETRO_WEB
void Run()
{
    Engine.Run();
}
#if RETRO_USE_WEB_SAVES
extern "C" void callbackIDBFS() {
    Engine.Init();
    emscripten_set_main_loop(Run, 0, false);
};

EM_JS(void, loadFromIDBFS, (), {
    FS.mkdir('/savesCD');
    FS.mount(IDBFS, { autoPersist: true }, '/savesCD');
    FS.syncfs(true, function (err) {
        if (err)
            console.error("Failed to load from IDBFS: " + err);
        else
            console.log("Successfully loaded from IDBFS");
        _callbackIDBFS(); // callback after idbfs loads to prevent savedata being read before it loads
    });
});
#endif
#endif

void parseArguments(int argc, char *argv[])
{
    for (int a = 0; a < argc; ++a) {
        const char *find = "";

        find = strstr(argv[a], "stage=");
        if (find) {
            int b = 0;
            int c = 6;
            while (find[c] && find[c] != ';') Engine.startSceneFolder[b++] = find[c++];
            Engine.startSceneFolder[b] = 0;
        }

        find = strstr(argv[a], "scene=");
        if (find) {
            int b = 0;
            int c = 6;
            while (find[c] && find[c] != ';') Engine.startSceneID[b++] = find[c++];
            Engine.startSceneID[b] = 0;
        }

        find = strstr(argv[a], "console=true");
        if (find) {
            engineDebugMode       = true;
            Engine.devMenu        = true;
            Engine.consoleEnabled = true;
#if RETRO_PLATFORM == RETRO_WIN && _MSC_VER
            AllocConsole();
            freopen_s((FILE **)stdin, "CONIN$", "w", stdin);
            freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
            freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
#endif
        }

        find = strstr(argv[a], "usingCWD=true");
        if (find) {
            usingCWD = true;
        }
    }
}
#endif

int main(int argc, char *argv[])
{
#if !RETRO_USE_ORIGINAL_CODE
    parseArguments(argc, argv);
#endif

#if RETRO_USE_WEB_SAVES
    loadFromIDBFS();
#else
    Engine.Init();
#if RETRO_PLATFORM != RETRO_WEB
    Engine.Run();
#else
    emscripten_set_main_loop(Run, 0, true);
#endif

#if !RETRO_USE_ORIGINAL_CODE
    if (Engine.consoleEnabled) {
#if RETRO_PLATFORM == RETRO_WIN && _MSC_VER
        FreeConsole();
#endif
    }
#endif
#endif

    return 0;
}
