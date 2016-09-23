TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/../SSU/

SOURCES += main.cpp \
    ssuexception.cpp \
    frame.cpp \
    framereader.cpp \
    newfeatureextractor.cpp \
    FeatureExtractor.cpp \
    posefeatures.cpp \
    BodyLocator.cpp \
    newbodylocator.cpp \
    recognitionsettings.cpp

HEADERS += \
    ssuexception.h \
    log.h \
    easylogging++.h \
    frame.h \
    pose.h \
    framereader.h \
    FeatureExtractor.h \
    newfeatureextractor.h \
    posefeatures.h \
    BodyLocator.h \
    newbodylocator.h \
    recognitionsettings.h

CONFIG += link_pkgconfig

PKGCONFIG += opencv


