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

QWEBDAVLIB_SRC = qwebdavlib/qwebdavlib/qwebdav.cpp \
    qwebdavlib/qwebdavlib/qwebdavitem.cpp \
    qwebdavlib/qwebdavlib/qwebdavdirparser.cpp \
    qwebdavlib/qwebdavlib/qnaturalsort.cpp

QWEBDAVLIB_HDR = qwebdavlib/qwebdavlib/qnaturalsort.h \
    qwebdavlib/qwebdavlib/qwebdavdirparser.h \
    qwebdavlib/qwebdavlib/qwebdav_global.h \
    qwebdavlib/qwebdavlib/qwebdav.h \
    qwebdavlib/qwebdavlib/qwebdavitem.h

SOURCES += src/SportsTracker.cpp \
    src/workout.cpp \
    src/gpx.cpp \
    src/workoutsummary.cpp \
    src/workoutsummarylist.cpp \
    src/gpxloader.cpp \
    src/gpxwriter.cpp \
    src/qmlplot.cpp \
    src/qmlplotdata.cpp \
    src/workoutdetails.cpp \
    src/logfilepositionsource.cpp \
    src/qmlplotdatasource.cpp \
    src/settings.cpp \
    src/gpxcache.cpp \
    src/gpsfilter.cpp \
    src/geodesy.cpp \
    $${QWEBDAVLIB_SRC}

OTHER_FILES += qml/harbour-sportstracker.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-sportstracker.spec \
    rpm/harbour-sportstracker.yaml \
    harbour-sportstracker.desktop \
    qml/pages/MainPage.qml \
    qml/pages/Tracking.qml \
    qml/util.js \
    qml/components/WorkoutSummary.qml \
    translations/en.ts \
    translations/fr.ts \
    qml/pages/WorkoutDetailsPage.qml \
    qml/pages/WorkoutDetailsPlot.qml \
    qml/pages/SplitViewPage.qml \
    qml/pages/WorkoutDetailsMap.qml \
    qml/pages/SettingsPage.qml

HEADERS += \
    src/workout.h \
    src/gpx.h \
    src/workoutsummary.h \
    src/workoutsummarylist.h \
    src/qmlplot.h \
    src/qmlplotdata.h \
    src/workoutdetails.h \
    src/logfilepositionsource.h \
    src/qmlplotdatasource.h \
    src/settings.h \
    src/gpsfilter.h \
    src/geodesy.h \
    $${QWEBDAVLIB_HDR}

QT += positioning location xml

TRANSLATIONS = \
    translations/en.qm \
    translations/fr.qm

translation.files = TRANSLATIONS
translation.path = /usr/share/${TARGET}
INSTALLS += translation

INSTALLS += images
images.files = images/Running.png \
    images/Cycling.png \
    images/Skiing.png \
    images/cover.png
images.path = /usr/share/$${TARGET}/images

OTHER_FILES += images/Running.png \
    images/Cycling.png \
    images/Skiing.png \
    images/cover.png


lupdate_only{
SOURCES = \
    qml/*.qml \
    qml/components/*.qml \
    qml/cover/*.qml \
    qml/pages/*.qml
}
