cmake_minimum_required (VERSION 2.8.11)

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
)

set (TN5250_SRC
	src/tn5250/tn5250.c
)

include_directories (src)
include_directories (src/lib5250)

add_definitions (-DSYSCONFDIR="/etc")
add_definitions (-DUSE_CURSES)
add_library (5250 ${LIB5250_SRC})

# misc utils
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
