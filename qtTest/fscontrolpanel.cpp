#include "fscontrolpanel.h"
#include "ui_fscontrolpanel.h"
#include "fscontroller.h"
#include "fslaser.h"
#include "fsturntable.h"
#include "fsserial.h"

#include <QDebug>
#include <QtTest/QTest>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

FSControlPanel::FSControlPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FSControlPanel)
{
    ui->setupUi(this);
    this->installEventFilter(this);
    qDebug("controlpanel constr called");
}

FSControlPanel::~FSControlPanel()
{
    delete ui;
}

void FSControlPanel::on_fetchFrameButton_clicked()
{
    FSController::getInstance()->fetchFrame();
}

void FSControlPanel::on_laserOnButton_clicked()
{
    FSController::getInstance()->laser->turnOn(); //turns on both lasers
///    FSController::getInstance()->laser2->turnOn();
}

void FSControlPanel::on_laserOffButton_clicked()
{
    FSController::getInstance()->laser->turnOff(); //turns off both lasers
///    FSController::getInstance()->laser2->turnOff();
}

void FSControlPanel::on_checkBox_stateChanged(int state)
{
    if(state==2){
        FSController::getInstance()->turntable->enable();
    }else{
        FSController::getInstance()->turntable->disable();
    }
}

void FSControlPanel::on_stepLeftButton_clicked()
{
    FSController::getInstance()->turntable->setDirection(FS_DIRECTION_CW);
    QString str = ui->degreesEdit->text();
    double degrees = str.toDouble();
    FSController::getInstance()->turntable->turnNumberOfDegrees(degrees);
}

void FSControlPanel::on_stepRightButton_clicked()
{
    FSController::getInstance()->turntable->setDirection(FS_DIRECTION_CCW);
    QString str = ui->degreesEdit->text();
    double degrees = str.toDouble();
    FSController::getInstance()->turntable->turnNumberOfDegrees(degrees);
}

void FSControlPanel::on_autoResetButton_clicked()
{
    if(FSController::getInstance()->webcam->info.portName.isEmpty()){
        FSController::getInstance()->mainwindow->showDialog("No webcam selected!");
        return;
    }
    FSController::getInstance()->detectLaserLine();
    cv::Mat shot = FSController::getInstance()->webcam->getFrame();
    cv::resize( shot,shot,cv::Size(1280,960) );
    shot = FSController::getInstance()->vision->drawLaserLineToFrame(shot);
    cv::resize(shot,shot,cv::Size(800,600));
    cv::imshow("Laser Frame",shot);
///    cv::waitKey(0);
///    cvDestroyWindow("Laser Frame");
    this->raise();
    this->focusWidget();
    this->setVisible(true);
}

void FSControlPanel::on_laserEnable_stateChanged(int state)
{
    if(state==2){
        FSController::getInstance()->laser->enable();
    }else{
        FSController::getInstance()->laser->disable();
    }
}

void FSControlPanel::on_laserEnable_2_stateChanged(int state)
{
    if(state==2){
        FSController::getInstance()->laser2->enable();
    }else{
        FSController::getInstance()->laser2->disable();
    }
}

void FSControlPanel::on_laserStepLeftButton_clicked()
{
    FSController::getInstance()->laser->setDirection(FS_DIRECTION_CCW);
    FSController::getInstance()->laser->turnNumberOfDegrees(0.1);
}

void FSControlPanel::on_laserStepLeftButton_2_clicked()
{
    FSController::getInstance()->laser2->setDirection(FS_DIRECTION_CCW);
    FSController::getInstance()->laser2->turnNumberOfDegrees(0.1);
}

void FSControlPanel::on_laserStepRightButton_clicked()
{
    FSController::getInstance()->laser->setDirection(FS_DIRECTION_CW);
    FSController::getInstance()->laser->turnNumberOfDegrees(0.1);
}

void FSControlPanel::on_laserStepRightButton_2_clicked()
{
    FSController::getInstance()->laser2->setDirection(FS_DIRECTION_CW);
    FSController::getInstance()->laser2->turnNumberOfDegrees(0.1);
}

void FSControlPanel::on_diffImage_clicked()
{
    if(FSController::getInstance()->webcam->info.portName.isEmpty()){
        FSController::getInstance()->mainwindow->showDialog("No webcam selected!");
        return;
    }
    cv::Mat shot = FSController::getInstance()->diffImage();
    cv::resize(shot,shot,cv::Size(800,600));
    cv::imshow("Laser Frame",shot);
///    cv::waitKey(0);
///    cvDestroyWindow("Laser Frame");
    this->raise();
    this->focusWidget();
    this->setVisible(true);
}

void FSControlPanel::on_laserSwipeMaxEdit_returnPressed()
{
    FSController::getInstance()->laserSwipeMax = (ui->laserSwipeMaxEdit->text()).toDouble();
}

void FSControlPanel::on_laserSwipeMaxEdit_2_returnPressed()
{
    FSController::getInstance()->laserSwipeMax = (ui->laserSwipeMaxEdit_2->text()).toDouble();
}

void FSControlPanel::setLaserAngleText(double angle)
{
    //QString a = QString::number(angle);
    QString a = QString("Angle: %1º").arg(angle);
    this->ui->laserAngle->setText(a);
}

void FSControlPanel::setLaser2AngleText(double angle)
{
    //QString a = QString::number(angle);
    QString a = QString("Angle: %1º").arg(angle);
    this->ui->laserAngle_2->setText(a);
}

void FSControlPanel::on_laserSwipeMinEdit_returnPressed()
{
    FSController::getInstance()->laserSwipeMin = (ui->laserSwipeMinEdit->text()).toDouble();
}

void FSControlPanel::on_laserSwipeMinEdit_2_returnPressed()
{
    FSController::getInstance()->laserSwipeMin = (ui->laserSwipeMinEdit_2->text()).toDouble();
}

void FSControlPanel::on_pushButton_2_clicked()
{

    FSController::getInstance()->laser->enable();
    FSController::getInstance()->laser->turnOff();
///    QThread::msleep(200);
    QTest::qWait(200);
    cv::Mat laserOffFrame = FSController::getInstance()->webcam->getFrame();
///    QThread::msleep(200);
    QTest::qWait(200);
    FSController::getInstance()->laser->turnOn();
///    QThread::msleep(200);
    QTest::qWait(200);
    cv::Mat laserOnFrame = FSController::getInstance()->webcam->getFrame();
///    QThread::msleep(200);
    QTest::qWait(200);
    cv::resize( laserOnFrame,laserOnFrame,cv::Size(1280,960) );
    cv::resize( laserOffFrame,laserOffFrame,cv::Size(1280,960) );

    qDebug() << "pressed";
    cv::Mat shot = FSVision::subLaser2(laserOffFrame, laserOnFrame);
    cv::resize(shot,shot,cv::Size(800,600));
    cv::imshow("Laser Frame",shot);
///    cv::waitKey(0);
///    cvDestroyWindow("Laser Frame");
    this->raise();
    this->focusWidget();
    this->setVisible(true);
}
