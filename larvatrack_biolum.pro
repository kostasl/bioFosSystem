TEMPLATE = app

QT += qml quick widgets gui


SOURCES += main.cpp \
    cvBlob/cvtrack.cpp \
    cvBlob/cvlabel.cpp \
    cvBlob/cvcontour.cpp \
    cvBlob/cvcolor.cpp \
    cvBlob/cvblob.cpp \
    cvBlob/cvaux.cpp

RESOURCES += qml.qrc

INCLUDEPATH +=/home/klagogia/opencv-3.0.0-installed/include/opencv2 \
                /home/klagogia/opencv-3.0.0-installed/include \
                /home/klagogia/opencv-3.0.0-installed/include/opencv

#INCLUDEPATH += /usr/local/include/opencv2
#INCLUDEPATH += /usr/include/cvblob

#INCLUDEPATH += /home/kostasl/workspace/cvblobLib
#INCLUDEPATH +=`pkg-config opencv qt --cflags`
##Figure out VERSION : pkg-config --modversion opencv
##Or Check CV_MAJOR_VERSION, CV_MINOR_VERSION


#LIBS += `pkg-config opencv --libs`
LIBS += -L/home/klagogia/opencv-3.0.0-installed/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_features2d\
            -lopencv_video \
            -lopencv_videoio \
            -lopencv_imgcodecs

LIBS += -L/home/kostasl/workspace -lcvblob
LIBS += -L/usr/local/cuda-7.5/targets/x86_64-linux/lib/ -lcudart #Necessary BS. cause linker looks for  and cudart.7.5 (installed)
LIBS += -L/home/klagogia/workspace/build-larvaltrack-Desktop-Release/libs/ -lcudart # cudart.7.0(not installed)
#LIBS += -L/usr/local/cuda-7.5/lib64/ -lcudart

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
##
##Assume Libs are copied with the package into
QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/libs
QMAKE_LFLAGS_RPATH=
###Using command : cp `ldd larvatrack | sed -re s/^.+\=\>// | sed -re 's/^(.+) \(.+\)/\1/'` /libs

# Default rules for deployment.
include(deployment.pri)

#unix: LIBS += -L$$PWD/../cvblob/build-cvBlobLib-Desktop-Release/ -lcvBlobLib
#INCLUDEPATH += $$PWD/../cvblob/cvBlob
#DEPENDPATH += $$PWD/../cvblob/cvBlob

OTHER_FILES += \
    ../cvblob/cvBlob/cvBlobLib.pro.user \
    cvBlob/cvBlobLib.pro.user \
    cvBlob/cvBlobLib.pro~ \
    cvBlob/CMakeLists.txt

SUBDIRS += \
    cvBlob/cvBlobLib.pro

HEADERS += \
    cvBlob/cvblob.h \
    larvatrack.h \
    ltROI.h
