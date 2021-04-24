SRCDIR = ../../src
TMPDIR = ../../buildtmp

DESTDIR = ../../bin

MOC_DIR = $$TMPDIR/$$TARGET
OBJECTS_DIR = $$TMPDIR/$$TARGET
RCC_DIR = $$TMPDIR/$$TARGET
UI_DIR = $$TMPDIR/$$TARGET
#UI_HEADERS_DIR
#UI_SOURCES_DIR

INCLUDEPATH += \
    $$SRCDIR

HEADERS += \
    $$SRCDIR/gradientmap.h \
    $$SRCDIR/gradientmapbase.h \
    $$SRCDIR/abstractpainter.h
