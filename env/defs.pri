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
    $$SRCDIR/abstractpainter.h \
    $$SRCDIR/axes2d.h \
    $$SRCDIR/axespolar.h \
    $$SRCDIR/gradientmap.h \
    $$SRCDIR/graph2d.h \
    $$SRCDIR/graphpolar.h
