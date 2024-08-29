greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/animator.cpp \
    src/bomb.cpp \
    src/button.cpp \
    src/creditsscene.cpp \
    src/fontmanager.cpp \
    src/gamescene.cpp \
    src/level.cpp \
    src/main.cpp \
    src/menuscene.cpp \
    src/optionsscene.cpp \
    src/physicalbody.cpp \
    src/physicsengine.cpp \
    src/pixmapmanager.cpp \
    src/player.cpp \
    src/ray.cpp \
    src/view.cpp

HEADERS += \
    src/animator.h \
    src/bomb.h \
    src/button.h \
    src/creditsscene.h \
    src/fontmanager.h \
    src/gamescene.h \
    src/level.h \
    src/menuscene.h \
    src/optionsscene.h \
    src/physicalbody.h \
    src/physicsengine.h \
    src/pixmapmanager.h \
    src/player.h \
    src/ray.h \
    src/resourceholder.h \
    src/utils.h \
    src/view.h

RESOURCES += \
    resource.qrc
