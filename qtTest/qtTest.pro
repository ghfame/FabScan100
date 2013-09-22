#-------------------------------------------------
#
# Project created by QtCreator 2012-09-17T22:38:59
#
#-------------------------------------------------

QT += core gui opengl multimedia multimediawidgets testlib

CONFIG += static noframework

TARGET = FabScan100
TEMPLATE = app

CONFIG += precompile_header
PRECOMPILED_HEADER = staticHeaders.h

include(qextserialport/src/qextserialport.pri)
#include(qextserialport-1.2beta2/src/qextserialport.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    geometryengine.cpp \
    mainwidget.cpp \
    fscontroller.cpp \
    fsmodel.cpp \
    fsserial.cpp \
    fsdialog.cpp \
    fscontrolpanel.cpp \
    fslaser.cpp \
    fsturntable.cpp \
    fsvision.cpp \
    fswebcam.cpp

HEADERS  += mainwindow.h \
    geometryengine.h \
    mainwidget.h \
    fscontroller.h \
    fsmodel.h \
    staticHeaders.h \
    fsserial.h \
    fswebcam.h \
    fsdialog.h \
    fscontrolpanel.h \
    fsdefines.h \
    fsgeometries.h \
    fslaser.h \
    fsvision.h \
    fsturntable.h

FORMS    += mainwindow.ui \
    fsdialog.ui \
    fscontrolpanel.ui

OTHER_FILES += \
    fshader.glsl \
    vshader.glsl

RESOURCES += \
    shaders.qrc \
    textures.qrc

macx {
    message("Buildng for Mac.")
#    INCLUDEPATH += /usr/local/Cellar/opencv/2.4.2/include
#    LIBS += -L/usr/local/Cellar/opencv/2.4.2
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_calib3d

    #LIBS += -L/usr/local/lib/vtk-5.10 \
    #-lvtkCommon \
    #-lvtkWidgets \
    #-lvtkIO \
    #-lvtkImaging \
    #-lvtkHybrid \
    #-lvtkVolumeRendering \
    #-lvtkRendering \
    #-lvtkGraphics \
    #-lvtkFiltering \
    #-lvtksys

    INCLUDEPATH += /usr/local/include/
    #INCLUDEPATH += /usr/local/include/pcl-1.7
    INCLUDEPATH += /usr/local/include/pcl-1.6
    LIBS += -L/usr/local/lib \
    -lpcl_common \
    -lpcl_io \
    -lpcl_io_ply \
    -lpcl_sample_consensus \
    -lpcl_octree \
#    -lpcl_ml \
    -lpcl_filters \
    -lpcl_kdtree \
    -lpcl_registration \
    -lpcl_features \
    -lpcl_segmentation \
    -lpcl_surface \
    -lpcl_search

    #INCLUDEPATH += /usr/local/Cellar/vtk/5.10.0/include/vtk-5.10

    INCLUDEPATH += /usr/local/Cellar/eigen/3.1.1/include/eigen3
    INCLUDEPATH += /usr/local/Cellar/flann/1.7.1/include
    INCLUDEPATH += /usr/local/Cellar/boost/1.50.0/include/boost

        LIBS += -L/usr/local/Cellar/boost/1.50.0 \
        -lboost_chrono-mt \
#        -lboost_context-mt \
        -lboost_date_time-mt \
        -lboost_exception-mt \
        -lboost_filesystem-mt \
        -lboost_graph-mt \
        -lboost_iostreams-mt \
        -lboost_locale-mt \
        -lboost_math_c99-mt \
        -lboost_math_c99f-mt \
        -lboost_math_c99l-mt \
        -lboost_math_tr1-mt \
        -lboost_math_tr1f-mt \
        -lboost_math_tr1l-mt \
        -lboost_prg_exec_monitor-mt \
        -lboost_program_options-mt \
        -lboost_python-mt \
        -lboost_random-mt \
        -lboost_regex-mt \
        -lboost_serialization-mt \
        -lboost_signals-mt \
        -lboost_system-mt \
        #-lboost_test_exec_monitor-mt \
        -lboost_thread-mt \
        -lboost_timer-mt \
        -lboost_unit_test_framework-mt \
        -lboost_wave-mt \
        -lboost_wserialization-mt \
}

linux-g++ {
    message("Buildng for Linux.")

    #CONFIG += link_pkgconfig
    #PKGCONFIG += opencv
    #INCLUDEPATH += /usr/local/include/opencv2/

    INCLUDEPATH += /usr/include/eigen3/

    LIBS += -L/usr/local/lib/ \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_calib3d

    INCLUDEPATH += /usr/include/

    INCLUDEPATH += /usr/include/pcl-1.6
    LIBS += -L/usr/lib \
    -lpcl_common \
    -lpcl_io \
    -lpcl_filters \
    -lpcl_kdtree \
    -lpcl_registration \
    -lpcl_features \
    -lpcl_segmentation \
    -lpcl_surface \
    -lpcl_search
}

win32 {
    message("Buildng for Win.")

    INCLUDEPATH += /32bitmicro/hbbr/src/Modules/Vision/install/include

    LIBS += -L/32bitmicro/hbbr/src/Modules/Vision/install/lib/ \
    opencv_core245.lib \
    opencv_highgui245.lib \
    opencv_imgproc245.lib \
    opencv_features2d245.lib \
    opencv_calib3d245.lib

    INCLUDEPATH += /usr/include/

    INCLUDEPATH += /32bitmicro/hbbr/src/Modules/Vision/install/include/pcl-1.6
    LIBS += -L/usr/lib \
    pcl_common_release.lib \
    pcl_io_release.lib \
    pcl_filters_release.lib \
    pcl_kdtree_release.lib \
    pcl_registration_release.lib \
    pcl_features_release.lib \
    pcl_segmentation_release.lib \
    pcl_surface_release.lib \
    pcl_search_release.lib

    INCLUDEPATH += /32bitmicro/hbbr/src/Modules/Math/install/include/eigen3
    INCLUDEPATH += /32bitmicro/hbbr/src/Modules/Vision/install/include/flann
    INCLUDEPATH += /32bitmicro/hbbr/src/Modules/C++/install/include/boost-1_50

#        -lboost_exception-vc90-mt-1_50.lib \
#        -lboost_context-vc90-mt-1_50.lib \

    LIBS += /LIBPATH:/32bitmicro/hbbr/src/Modules/C++/install/lib \
        boost_chrono-vc90-mt-1_50.lib \
        boost_date_time-vc90-mt-1_50.lib \
        boost_filesystem-vc90-mt-1_50.lib \
        boost_graph-vc90-mt-1_50.lib \
        boost_iostreams-vc90-mt-1_50.lib \
        boost_locale-vc90-mt-1_50.lib \
        boost_math_c99-vc90-mt-1_50.lib \
        boost_math_c99f-vc90-mt-1_50.lib \
        boost_math_c99l-vc90-mt-1_50.lib \
        boost_math_tr1-vc90-mt-1_50.lib \
        boost_math_tr1f-vc90-mt-1_50.lib \
        boost_math_tr1l-vc90-mt-1_50.lib \
        boost_prg_exec_monitor-vc90-mt-1_50.lib \
        boost_program_options-vc90-mt-1_50.lib \
        boost_python-vc90-mt-1_50.lib \
        boost_random-vc90-mt-1_50.lib \
        boost_regex-vc90-mt-1_50.lib \
        boost_serialization-vc90-mt-1_50.lib \
        boost_signals-vc90-mt-1_50.lib \
        boost_system-vc90-mt-1_50.lib \
        #boost_test_exec_monitor-vc90-mt-1_50.lib \
        boost_thread-vc90-mt-1_50.lib \
        boost_timer-vc90-mt-1_50.lib \
        boost_unit_test_framework-vc90-mt-1_50.lib \
        boost_wave-vc90-mt-1_50.lib \
        boost_wserialization-vc90-mt-1_50.lib \
}
