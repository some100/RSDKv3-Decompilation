add_executable(RetroEngine ${RETRO_FILES})

set(DEP_PATH web)

target_link_options(RetroEngine PRIVATE 
                    -sINITIAL_MEMORY=128mb 
                    -Oz
                    -sLEGACY_GL_EMULATION
                    -sWASM_BIGINT 
                    --use-port=ogg
                    --use-port=vorbis
                    --use-preload-cache
                    --preload-file ../Data.rsdk@/ 
                    --embed-file ../settings.ini@/
)
target_compile_options(RetroEngine PRIVATE 
                    --use-port=ogg
                    --use-port=vorbis
                    -Oz
)

set(COMPILE_THEORA on)

if(RETRO_SDL_VERSION STREQUAL "2")
    target_compile_options(RetroEngine PRIVATE --use-port=sdl2)
    target_link_options(RetroEngine PRIVATE --use-port=sdl2)
elseif(RETRO_SDL_VERSION STREQUAL "1")
    target_link_options(RetroEngine PRIVATE -lSDL)
endif()

if(RETRO_WEB_SAVES)
    target_link_options(RetroEngine PRIVATE -lidbfs.js -sEXPORTED_FUNCTIONS=_main,_callbackIDBFS)
    target_compile_definitions(RetroEngine PRIVATE RETRO_USE_WEB_SAVES=1)
endif()

if(RETRO_MOD_LOADER)
    set_target_properties(RetroEngine PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
endif()

set(CMAKE_EXECUTABLE_SUFFIX ".html")