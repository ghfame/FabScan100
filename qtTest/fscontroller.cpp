#include "fscontroller.h"
#include "fsdialog.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <QFuture>
#include <QtTest/QTest>
#include <QtConcurrent/QtConcurrentRun>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

using namespace std;

FSController* FSController::singleton=0;

FSController::FSController()
{
    //Software
    geometries = new GeometryEngine();
    model = new FSModel();
    //Harware
    serial = new FSSerial();
    webcam = new FSWebCam();
    turntable = new FSTurntable();
    laser = new FSLaser(true);      // right laser
    laser2 = new FSLaser(false);    // left  laser
    vision = new FSVision();
    scanning = false;
    //all in degrees; (only when stepper is attached to laser)
    laserSwipeMin = 25; //30; //18
    laserSwipeMax = 35; //45; //50
    laserStepSize = 0.1;
    laserSwipeMin2 = 385;
    laserSwipeMax2 = 395;
    laserStepSize2 = 0.1;
    meshComputed = false;
}

FSController* FSController::getInstance()
{
    if (singleton == 0){
        singleton = new FSController();
    }
    return singleton;
}

void FSController::destroy()
{
    if (singleton != 0) {
        delete singleton;
        singleton = 0;
    }
}

void FSController::fetchFrame()
{
    if(webcam->info.portName.isEmpty()){
        mainwindow->showDialog("No webcam selected!");
        return;
    }

    cv::Mat frame;
    frame = FSController::getInstance()->webcam->getFrame();
    //cv::imshow("Extracted Frame",frame);
    //cv::waitKey(0);
    cv::resize(frame,frame,cv::Size(1280,960));
    cv::Mat result = vision->drawHelperLinesToFrame(frame);
    cv::resize(result,result,cv::Size(800,600)); //this is the resolution of the preview
    cv::imshow("Extracted Frame",result);
///    cv::waitKey(0);
///    cvDestroyWindow("Extracted Frame");
}

void FSController::hideFrame()
{
    cvDestroyWindow("Extracted Frame");
}

void FSController::scan()
{
    if(webcam->info.portName.isEmpty()){
        mainwindow->showDialog("No webcam selected!");
        return;
    }
    QFuture<void> future = QtConcurrent::run(this, &FSController::scanThread);
}

void FSController::scanThread()
{
    //check if camera is connected
    if(webcam->info.portName.isEmpty()){
        mainwindow->showDialog("No webcam selected!");
        return;
    }
    //detect laser line

    // PW why are we detecting it here during scanning?
    // we should only do this if laser position has not been established yet
    // otherwise this leads to problems when line is either obscured or we get wrong angle
    // this is not reliable and laser line should be detected only during calibration
///    this->detectLaserLine();
    //turn off stepper (if available)
    // PW do not do that, we use enable/disable to select laser for scanning
///    this->laser->disable();

    scanning = true; //start scanning, if false, scan stops
    FSFloat stepDegrees = turntableStepSize;

    laser->turnOn();
    turntable->setDirection(FS_DIRECTION_CCW);
    turntable->enable();

    //iterate over a complete turn of the turntable
    for(FSFloat i=0; i<360 && scanning==true; i+=stepDegrees){
        if(!scanning) {
            mainwindow->doneScanning();
            break;
        }
        //take picture without laser
        laser->turnOff();
///        QThread::msleep(200);
		QTest::qWait(200);
        cv::Mat laserOff = webcam->getFrame();
        cv::resize( laserOff,laserOff,cv::Size(1280,960) );

        //take picture with laser
        laser->turnOn();
///        QThread::msleep(200);
        QTest::qWait(200);
        cv::Mat laserOn = webcam->getFrame();
        cv::resize( laserOn,laserOn,cv::Size(1280,960) );

        //here the magic happens
        vision->putPointsFromFrameToCloud(laserOff, laserOn, yDpi, 0);
        //update gui
        geometries->setPointCloudTo(model->pointCloud);
        mainwindow->redraw();
        //turn turntable a step
        turntable->turnNumberOfDegrees(stepDegrees);
///        QThread::msleep(  300+stepDegrees*100);
		QTest::qWait(300+stepDegrees*100);
    }
    if(scanning) mainwindow->doneScanning();
    scanning = false; //stop scanning
}

void FSController::scanThread2()
{
    if(webcam->info.portName.isEmpty()){
        mainwindow->showDialog("No webcam selected!");
        return;
    }
    scanning = true; //start scanning
    qDebug() << "done with turn to angle";
    //laser->turnNumberOfDegrees( laser->getRotation().y - LASER_SWIPE_MIN );
    turntable->setDirection(FS_DIRECTION_CW);
    for(FSFloat j=0; j<360 && scanning==true; j+=turntableStepSize){
        turntable->disable();
        laser->turnOn();
        laser->enable();
        laser->turnToAngle(laserSwipeMin);
///        QThread::msleep(2500);
		QTest::qWait(2500);
        laser->setDirection(FS_DIRECTION_CCW);
        for(FSFloat i=laserSwipeMin; i<laserSwipeMax && scanning==true; i+=laserStepSize){
            qDebug() << i;
            laser->turnOff();
///            QThread::msleep(200);
            QTest::qWait(200);
            cv::Mat laserOff = webcam->getFrame();
            cv::resize( laserOff,laserOff,cv::Size(1280,960) );

            laser->turnOn();
///            QThread::msleep(200);
            QTest::qWait(200);
            cv::Mat laserOn = webcam->getFrame();
            cv::resize( laserOn,laserOn,cv::Size(1280,960) );

            vision->putPointsFromFrameToCloud(laserOff, laserOn, 5, 0);
            geometries->setPointCloudTo(model->pointCloud);
            mainwindow->redraw();
            laser->turnNumberOfDegrees(laserStepSize);
///            QThread::msleep(laserStepSize*100);
			QTest::qWait(laserStepSize*100);
        }
        laser->disable();
        turntable->enable();
        turntable->turnNumberOfDegrees(turntableStepSize);
        std::string name;
        name.append(boost::lexical_cast<std::string>(j));
        name.append(".ply");
        //model->savePointCloudAsPLY(name);
        //model->pointCloud->clear();
///        QThread::msleep(turntableStepSize*100);
		QTest::qWait(turntableStepSize*100);
    }
    if(scanning) mainwindow->doneScanning();
    scanning = false; //stop scanning
}

cv::Mat FSController::diffImage()
{
    laser->turnOff();
///    laser2->turnOff();
///    QThread::msleep(200);
    QTest::qWait(200);

    cv::Mat laserOff = webcam->getFrame();
    cv::resize( laserOff,laserOff,cv::Size(1280,960) );

    laser->turnOn();
///    laser2->turnOn();
///    QThread::msleep(200);
    QTest::qWait(200);
    cv::Mat laserOn = webcam->getFrame();
    cv::resize( laserOn,laserOn,cv::Size(1280,960) );

    return vision->diffImage(laserOff,laserOn);
}


bool FSController::detectLaserLine()
{
    unsigned int threshold = 40;
    laser->turnOff();
///    QThread::msleep(200);
    QTest::qWait(200);
    cv::Mat laserOffFrame = webcam->getFrame();
///    QThread::msleep(200);
    QTest::qWait(200);
    laser->turnOn();
///    QThread::msleep(200);
    QTest::qWait(200);
    cv::Mat laserOnFrame = webcam->getFrame();
///    QThread::msleep(200);
    QTest::qWait(200);
    cv::resize( laserOnFrame,laserOnFrame,cv::Size(1280,960) );
    cv::resize( laserOffFrame,laserOffFrame,cv::Size(1280,960) );

    qDebug("images loaded, now detecting...");
    if ( !laser2->getEnabled() ) {
        FSPoint p = vision->detectLaserLine( laserOffFrame, laserOnFrame, threshold, 0); // 1 laser
        if(p.x == 0.0){return false;}
        laser->setLaserPointPosition(p);
    } else {
        FSPoint p = vision->detectLaserLine( laserOffFrame, laserOnFrame, threshold, 1); // first laser
        if(p.x != 0.0){
            laser->setLaserPointPosition(p);
        }

        FSPoint p2 = vision->detectLaserLine( laserOffFrame, laserOnFrame, threshold, 2); // second laser
        if(p2.x != 0.0){
            laser2->setLaserPointPosition(p2);
        }

        if(p.x == 0.0 || p2.x == 0.0)
            return false;

    }
    return true;
}

void FSController::computeSurfaceMesh()
{
    //model->convertPointCloudToSurfaceMesh();
    //geometries->setSurfaceMeshTo(model->surfaceMesh,model->pointCloud);
    model->convertPointCloudToSurfaceMesh2();
    model->convertPolygons2Triangles();
    //cout << "FSController:computesurfaceMesh: convert done, now setting" << endl;

    //geometries->setSurfaceMeshTo(model->surfaceMesh,model->pointCloud);

    //mainwindow->redraw();
}
