# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-sportstracker

CONFIG += sailfishapp c++11

SOURCES += src/SportsTracker.cpp \
    src/workout.cpp \
    src/gpx.cpp \
    src/workoutsummary.cpp \
    src/workoutsummarylist.cpp

OTHER_FILES += qml/harbour-sportstracker.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-sportstracker.spec \
    rpm/harbour-sportstracker.yaml \
    harbour-sportstracker.desktop \
    qml/pages/MainPage.qml \
    qml/pages/Tracking.qml \
    qml/pages/Settings.qml \
    qml/util.js \
    qml/components/WorkoutSummary.qml \
    translations/en.ts \
    translations/fr.ts

HEADERS += \
    src/workout.h \
    src/gpx.h \
    src/workoutsummary.h \
    src/workoutsummarylist.h

QT += positioning location xml

TRANSLATIONS = \
    translations/en.qm \
    translations/fr.qm

translation.files = TRANSLATIONS
translation.path = /usr/share/${TARGET}
INSTALLS += translation

lupdate_only{
SOURCES = \
    qml/*.qml \
    qml/components/*.qml \
    qml/cover/*.qml \
    qml/pages/*.qml
}
