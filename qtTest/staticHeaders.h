#ifndef STATICHEADERS_H
#define STATICHEADERS_H
#define BOOST_ALL_NO_LIB 1
#include <Eigen/Core>
#include <opencv2/core/types_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#ifndef Q_MOC_RUN
#include <pcl/point_types.h>
#endif

#if defined __cplusplus

#include <QMainWindow>
#include <QFileDialog>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>
#include <QtOpenGL/QGLShaderProgram>
#include <QPointF>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

#ifndef Q_MOC_RUN
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/vtk_io.h>
#endif
#include "fsdefines.h"
#include "fsgeometries.h"

#endif

#endif // STATICHEADERS_H
