#ifndef LARVATRACK_H
#define LARVATRACK_H


#include <iostream>
#include <sstream>

#include <QString>
#include <QApplication>
//#include <QQmlApplicationEngine>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QElapsedTimer>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/video/background_segm.hpp"

#include <cvBlob/cvblob.h>
#include <ltROI.h> //Defines the ROI types


/// \file larvatrack.h
/// \brief OpenCV based in-vial larva tracker header file.



/// \fn processVideo
/// \brief  Main Loop function runs for each video in a list assumes 1st frame id is istartFrame.
/// \returns Returns the last frame number
/// \param videoFilename
/// \param outFileCSV
/// \param istartFrame
unsigned int processVideo(QString videoFilename,QString outFileCSV,unsigned int istartFrame);
void checkPauseRun(int& keyboard,std::string frameNumberString);
bool saveImage(std::string frameNumberString,QString dirToSave,cv::Mat& img);
int countObjectsviaContours(cv::Mat& srcimg );
int countObjectsviaBlobs(cv::Mat& srcimg,cvb::CvBlobs& blobs,cvb::CvTracks& tracks,QString outFileCSV,std::string& frameNumberString,double& dMeanBlobArea);

int saveTracks(cvb::CvTracks& tracks,QString filename,std::string frameNumber);
int saveTrackedBlobs(cvb::CvBlobs& blobs,QString filename,std::string frameNumber,cv::Rect& roi);
int saveTrackedBlobsTotals(cvb::CvBlobs& blobs,cvb::CvTracks& tracks,QString filename,std::string frameNumber,cv::Rect& roi);

///\fn readBiolumFile
/// \brief Read The bioluminesce values from a text (non-binary values) file and save them to provided vector
unsigned int readBiolumFile(std::vector<unsigned int> &vBioLumRec,QString filename,unsigned int &imaxValue, unsigned int &iminValue,unsigned int& imeanValue);

void CallBackFunc(int event, int x, int y, int flags, void* userdata); //Mouse Callback


///
/// \brief addClickAsTrackPoint allows manual tracking by adding a track point where the
/// left mouse clicked, to the track found closest to the click point.
/// The track's centroid is also updated to the click point- THis allows the tracking to resume on the same track, once the blob is available again
///
/// \param x Mouse click x
/// \param y Mouse click y
void addClickAsTrackPoint(int x, int y);

#endif // LARVATRACK_H
