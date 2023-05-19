cmake_minimum_required (VERSION 3.7)

project (tn5250)

set (LIB5250_SRC 
	src/lib5250/buffer.c
	src/lib5250/conf.c
	src/lib5250/cursesterm.c
	src/lib5250/dbuffer.c
	src/lib5250/debug.c
	src/lib5250/display.c
	src/lib5250/field.c
	src/lib5250/printsession.c
	src/lib5250/record.c
	src/lib5250/scs.c
	src/lib5250/session.c
	src/lib5250/sslstream.c
	src/lib5250/stream.c
	src/lib5250/telnetstr.c
	src/lib5250/utility.c
	src/lib5250/version.c
	src/lib5250/wtd.c
		src/wx5250/wx5250.cpp)

set (TN5250_SRC
	src/tn5250/tn5250.c
)

set (TN5250_Windows_SRC
	src/win32/tn5250-win.c
	src/win32/tn5250-res.rc
	src/win32/winterm.c
)

set ( TN5250_Chonky_SRC
        src/win32/chonky.c
	src/win32/tn5250-res.rc
        src/win32/winterm.c
)

include_directories (src)
include_directories (src/lib5250)

add_definitions (-DSYSCONFDIR="/etc")
add_definitions (-DUSE_CURSES)
add_library (5250 ${LIB5250_SRC})

# misc utils
if (WIN32)
	add_definitions (-DWIN32)
	add_executable (tn5250 ${TN5250_Windows_SRC})
	target_link_libraries (tn5250 5250)
	target_link_libraries (tn5250 winmm)
	target_link_libraries (tn5250 ws2_32)

	set_target_properties (tn5250 PROPERTIES
          LINK_FLAGS -mwindows
	)

        add_executable (chonky ${TN5250_Chonky_SRC})
	target_link_libraries (chonky 5250)
        target_link_libraries (chonky winmm)
        target_link_libraries (chonky ws2_32)

        set_target_properties (chonky PROPERTIES
          LINK_FLAGS -mwindows
        )
else()
	add_definitions (-DUSE_CURSES)
	add_executable (lp5250d src/lp5250d/lp5250d.c)
	target_link_libraries (lp5250d 5250)
	
	add_executable (scs2ascii src/lp5250d/scs2ascii.c)
	target_link_libraries (scs2ascii 5250)
	
	add_executable (scs2ps src/lp5250d/scs2ps.c)
	target_link_libraries (scs2ps 5250)
	
	add_executable (scs2pdf src/lp5250d/scs2pdf.c)
	target_link_libraries (scs2pdf 5250)

	# curses 5250 client
	add_executable (tn5250 ${TN5250_SRC})
	target_link_libraries (tn5250 5250)
	target_link_libraries (tn5250 curses)
endif ()

# wxWidgets experiments:
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

set (wx5250_SRC
	src/wx5250/wx5250.cpp
)

if(APPLE)
	add_executable(wx5250 MACOSX_BUNDLE ${wx5250_SRC})
	set_target_properties(wx5250 PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/src/wx5250/Info.plist)
else()
	add_executable(wx5250 WIN32 ${wx5250_SRC} wx5250.exe.manifest)
endif()

target_link_libraries(wx5250 PRIVATE ${wxWidgets_LIBRARIES})

# SDL2 experiments:
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set (sdl5250_SRC
		src/sdl5250/sdl5250.c
)
set(sdl5250_ASSETS
		src/sdl5250/3270-Regular.ttf
)


if(APPLE)
	add_executable(sdl5250 MACOSX_BUNDLE ${sdl5250_SRC})
	target_sources(sdl5250 PRIVATE ${sdl5250_ASSETS})
	set_target_properties(sdl5250 PROPERTIES
			MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/src/sdl5250/Info.plist
			RESOURCE "${sdl5250_ASSETS}")
else()
	add_executable(sdl5250 WIN32 ${sdl5250_SRC} sdl5250.exe.manifest)
endif()

target_link_libraries(sdl5250 ${SDL2_LIBRARIES} SDL2_ttf)

