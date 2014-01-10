# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = SportsTracker

CONFIG += sailfishapp

SOURCES += src/SportsTracker.cpp \
    src/workout.cpp

OTHER_FILES += qml/SportsTracker.qml \
    qml/cover/CoverPage.qml \
    qml/pages/SecondPage.qml \
    rpm/SportsTracker.spec \
    rpm/SportsTracker.yaml \
    SportsTracker.desktop \
    qml/pages/MainPage.qml \
    qml/pages/Tracking.qml \
    qml/pages/Settings.qml

HEADERS += \
    src/workout.h

QT += positioning
