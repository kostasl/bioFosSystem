// Copyright (C) 2007 by Cristóbal Carnero Liñán
// grendel.ccl@gmail.com
//
// This file is part of cvBlob.
//
// cvBlob is free software: you can redistribute it and/or modify
// it under the terms of the Lesser GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// cvBlob is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// Lesser GNU General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License
// along with cvBlob.  If not, see <http://www.gnu.org/licenses/>.
//


#include <cmath>
#include <iostream>
#include <sstream>
#include <stack>
#include <list>
using namespace std;

#define TRACK_LUM_REPORT_INTERVAL 600 //Show VAlue Every 500 frames


#if (defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || (defined(__APPLE__) & defined(__MACH__)))
    #include <cv.h>
#else
    #include <opencv/cv.h>
#endif

//Trying to Find Mat includes
#include <opencv/cv.h>   		// open cv general include file
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp" //Draw Polyline
#include "cvblob.h"

extern unsigned int nFrame;
extern double gdLumRecfps;
extern double gdvidfps;
extern unsigned int gmaxLumValue;
extern unsigned int gminLumValue;
extern unsigned int gframeLumValue;
extern unsigned int gminTrackLength; //Inactive tracks shorter than n points are deleted
//extern std::vector<unsigned int> vLumRec;

namespace cvb
{

 /// \note Need to understand this distance measure
  double distantBlobTrack(CvBlob const *b, CvTrack const *t)
  {
    double d1;
    if (b->centroid.x<t->minx)
    {
      if (b->centroid.y<t->miny)
        d1 = MAX(t->minx - b->centroid.x, t->miny - b->centroid.y);
      else if (b->centroid.y>t->maxy)
        d1 = MAX(t->minx - b->centroid.x, b->centroid.y - t->maxy);
      else // if (t->miny < b->centroid.y)&&(b->centroid.y < t->maxy)
        d1 = t->minx - b->centroid.x;
    }
    else if (b->centroid.x>t->maxx)
    {
      if (b->centroid.y<t->miny)
        d1 = MAX(b->centroid.x - t->maxx, t->miny - b->centroid.y);
      else if (b->centroid.y>t->maxy)
        d1 = MAX(b->centroid.x - t->maxx, b->centroid.y - t->maxy);
      else
        d1 = b->centroid.x - t->maxx;
    }
    else // if (t->minx =< b->centroid.x) && (b->centroid.x =< t->maxx)
    {
      if (b->centroid.y<t->miny)
           d1 = t->miny - b->centroid.y;
      else if (b->centroid.y>t->maxy)
            d1 = b->centroid.y - t->maxy;
      else 
	return 0.;
    }

    double d2;
    if (t->centroid.x<b->minx)
    {
      if (t->centroid.y<b->miny)
            d2 = MAX(b->minx - t->centroid.x, b->miny - t->centroid.y);
      else if (t->centroid.y>b->maxy)
            d2 = MAX(b->minx - t->centroid.x, t->centroid.y - b->maxy);
      else // if (b->miny < t->centroid.y)&&(t->centroid.y < b->maxy)
            d2 = b->minx - t->centroid.x;
    }
    else if (t->centroid.x>b->maxx)
    {
      if (t->centroid.y<b->miny)
           d2 = MAX(t->centroid.x - b->maxx, b->miny - t->centroid.y);
      else if (t->centroid.y>b->maxy)
           d2 = MAX(t->centroid.x - b->maxx, t->centroid.y - b->maxy);
      else
           d2 = t->centroid.x - b->maxx;
    }
    else // if (b->minx =< t->centroid.x) && (t->centroid.x =< b->maxx)
    {
      if (t->centroid.y<b->miny)
            d2 = b->miny - t->centroid.y;
      else if (t->centroid.y>b->maxy)
            d2 = t->centroid.y - b->maxy;
      else 
	return 0.;
    }

    return MIN(d1, d2);
  }

  //KL: MACROS To access Proximity Matrix Data
  // Access to matrix
#define C(blob, track) close[((blob) + (track)*(nBlobs+2))]
  // Access to accumulators //KLLast Rows/columns have Sums
#define AB(label) C((label), (nTracks)) //Return Number of tracks proximal to blob
#define AT(id) C((nBlobs), (id)) //Return Number of blobs proximal to Track
  // Access to identifications //KL : One Before Last has Labels
#define IB(label) C((label), (nTracks)+1)
#define IT(id) C((nBlobs)+1, (id))
  // Access to registers
#define B(label) blobs.find(IB(label))->second
#define T(id) tracks.find(IT(id))->second


 //Populates list of tracks proximal to blob - and removes them from the proximity matrix
  //This goes around all columns of C matrix, and for each it calls getClusterForTrack which goes down every row of that column
  void getClusterForBlob(unsigned int blobPos, CvID *close, unsigned int nBlobs, unsigned int nTracks, CvBlobs const &blobs, CvTracks const &tracks, list<CvBlob*> &bb, list<CvTrack*> &tt)
  {
    //For each Track
    for (unsigned int j=0; j<nTracks; j++)
    {
        //Check if blob is proxim. to track
      if (C(blobPos, j))
      {
       //Add track to track list
        tt.push_back(T(j));

        //Get blob count for track
        unsigned int c = AT(j);

        C(blobPos, j) = 0; //Reset proximity to 0
        AB(blobPos)--; //reduce track Accumulator for blob
        AT(j)--; //Reduce blob accum for track

        if (c>1) //If track blob was connected - run Cluster for Track
        {
          getClusterForTrack(j, close, nBlobs, nTracks, blobs, tracks, bb, tt);
        }
      }
    }
  }

//Populates list of blobs proximal to a track - and removes them from the proximity matrix
  void getClusterForTrack(unsigned int trackPos, CvID *close, unsigned int nBlobs, unsigned int nTracks, CvBlobs const &blobs, CvTracks const &tracks, list<CvBlob*> &bb, list<CvTrack*> &tt)
  {
      //For each blob
    for (unsigned int i=0; i<nBlobs; i++)
    {
       //KL: check if blob close to track using / Proximity matrix
      if (C(i, trackPos))
      {
         //Add blob to list of blobs
        bb.push_back(B(i));

        //Check number of tracks assigned to blob
        unsigned int c = AB(i);

        //Reset Prox. entry to 0
        C(i, trackPos) = 0;
        AB(i)--; //reduce track Accumulator for blob
        AT(trackPos)--; //Reduce blob accum for track

        if (c>1) //If these were connected Call
        {
          getClusterForBlob(i, close, nBlobs, nTracks, blobs, tracks, bb, tt);
        }
      }
    }
  }

  /// \brief Initializes empty a proximity matrix, and uses macros C, AB,AT,.. to access label and counting the blob label proximities
  /// \param thDistance pixel distance used for radius that clusters tracks
  /// Reassign Max Track ID
  /// Detect inactive tracks
  /// Calculate/Fill Proximity Matrix :I added a limit to the track's ROI when assigning proximity based on fixed thDistance
  /// Detect inactive tracks
  /// // Detect new tracks
  /// // Clustering
  /// Updates Active/Inactive Stats
  /// Erase Tracks that have been Inactive according to rule + Add filter to erase only small tracks
  /// delete Proximity Matrix
  void cvUpdateTracks(CvBlobs const &blobs, CvTracks &tracks, ltROIlist& vRoi, const double thDistance, const unsigned int thInactive, const unsigned int thActive)
  {
    CV_FUNCNAME("cvUpdateTracks");
    __CV_BEGIN__;

    unsigned int nBlobs = blobs.size();
    unsigned int nTracks = tracks.size();

    // Proximity matrix:
    // Last row/column is for ID/label.
    // Last-1 "/" is for accumulation.
    CvID *close = new unsigned int[(nBlobs+2)*(nTracks+2)]; // XXX Must be same type than CvLabel.

    //KL: Note Huge Try Block
    try
    {
      // Inicialization:
      unsigned int i=0;
      for (CvBlobs::const_iterator it = blobs.begin(); it!=blobs.end(); ++it, i++)
      {
        AB(i) = 0;
        IB(i) = it->second->label;
      }

      //KL:Reassign Max Track ID - Initialize TrackLabels Search through all trackss
      CvID maxTrackID = 0;
      unsigned int j=0;
      for (CvTracks::const_iterator jt = tracks.begin(); jt!=tracks.end(); ++jt, j++)
      {
        AT(j) = 0;
        IT(j) = jt->second->id;
        if (jt->second->id > maxTrackID)
          maxTrackID = jt->second->id;
      }

      // Proximity matrix calculation and "used blob" list inicialization:
      for (i=0; i<nBlobs; i++)
      {
        for (j=0; j<nTracks; j++)
        {
          CvTrack* t = T(j); //Fetch The blob to examine ROI
          CvBlob* b = B(i); //Fetch The blob to examine ROI
          C(i, j) = distantBlobTrack(b,t) < min( 2.0*(t->effectiveDisplacement+thDistance/2.0),3*thDistance);
          //if (C(i, j) < thDistance  ) //< thDistance (t->effectiveDisplacement + 5)
          if(C(i, j))
          {

             //No Need If blobs Are filtered by ROI / But need to check if they are in the same ROI
             ltROI* blbroi = ltGetFirstROIContainingPoint(vRoi ,cv::Point(b->centroid.x,b->centroid.y) );
             if (blbroi == 0 ) //Not In any tracked ROI - so ignore
                continue;

             //Check if they Are in the Same ROI before Adding to accumulator
             if (*blbroi == *(t->pROI) )
             {
                AB(i)++; //Add blobcount to the track
                AT(j)++; //Add trackcount to the blob

                //cout << t->effectiveDisplacement << endl;
             }
          }
        }//Each Track
      } //Each Blob

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Detect inactive tracks
      for (j=0; j<nTracks; j++)
      {
        unsigned int c = AT(j);

        if (c==0)
        {
          //cout << "Inactive track: " << j << endl;

          // Inactive track.
          CvTrack *track = T(j);
          track->inactive++;
          track->label = 0;
        }
       }

      // Detect new tracks
      for (i=0; i<nBlobs; i++)
      {
        unsigned int c = AB(i);

        if (c==0)
        {
          CvBlob *blob = B(i);
          //cout << "Blob (new track): " << maxTrackID+1 << endl;
          //cout << *B(i) << endl;
          //Check If Blob is within some ROI
          cv::Point pntCentroid = cv::Point(blob->centroid.x,blob->centroid.y);
          //KL: Detect Which ROI
          ltROI* proi = ltGetFirstROIContainingPoint(vRoi ,pntCentroid);
          if (proi == 0)
              continue; //Ignore this blob its out of ROI

          // New track.
          maxTrackID++;
          CvTrack *track = new CvTrack;

          //Copies Blob data to track
          track->id = maxTrackID;
          track->label = blob->label;
          track->minx = blob->minx;
          track->miny = blob->miny;
          track->maxx = blob->maxx;
          track->maxy = blob->maxy;
          track->centroid = blob->centroid;
          track->effectiveDisplacement = sqrt((double)blob->area); //Set To largest value initially
          track->lifetime = 0;
          track->active = 0;
          track->inactive = 0;


          track->pROI = proi; //Set Pointer to ROI containing the 1st blob
          track->pointStack.push_back(std::pair<cv::Point,int>(pntCentroid,gframeLumValue)); //Add 1st Point to list of Track
          tracks.insert(CvIDTrack(maxTrackID, track));
        }
      } //END NEW Tracks
      ////////////////
      unsigned int area;
      double dist;

      // Clustering of the Tracks
      for (j=0; j<nTracks; j++)
      {
        //KL:Accumulators
        unsigned int c = AT(j);
        CvTrack* cTrack = T(j);

        if (c) //If Connected / Proximal to blob
        {
          list<CvTrack*> tt; tt.push_back(T(j));
          list<CvBlob*> bb;

          getClusterForTrack(j, close, nBlobs, nTracks, blobs, tracks, bb, tt);

//          // Select track
//          //KL :SEG FAULT is caused by these searches failing -low rate occurance)
            CvTrack *track    = cTrack; //Start With the initial Track as picked //TODO:Change to NULL?
            area = (cTrack->maxx-cTrack->minx)*(cTrack->maxy-cTrack->miny); //Area Of track we compare against
            dist       = thDistance/2; //Distance over which Tracks are Clustered (Make Small so that we have higher track resolution)
//          // Go Through List Of tracks Around track -
//          // Pick the one associated with the blob with larges area in proximity with picked track
           for (list<CvTrack*>::const_iterator it=tt.begin(); it!=tt.end(); ++it)
           {
            CvTrack *t = *it;

            unsigned int a = (t->maxx-t->minx)*(t->maxy-t->miny);
            double d = round(sqrt( pow(t->centroid.x - cTrack->centroid.x,2) + pow( (t->centroid.y - cTrack->centroid.y),2)  ));

            if (d<=dist)
            {
                if (a>area) //Chooses the Track With Largest Area - To cluster Smaller Ones in the vicinity
                {
                  area = a;
                  dist = d;
                  track = t;
                }
            }
          } //Finished Clustering tracks picked the one

          // Select blob //KL SET TO NULL Detect not found
          CvBlob *blob  = NULL;
          area          = 0;
          dist          = thDistance; //Reset to Max Distance

          //Go through All blobs - Find the one that is closeset to track and largest Area
          for (list<CvBlob*>::const_iterator it=bb.begin(); it!=bb.end(); ++it)
          {
            CvBlob *b = *it;
            //cout << b->label << " ";
            //Remove blobs that are not in the same ROI as the track - and those that fail the filter
            if (track != NULL)
            {
//              Also Checked at Proximity Matrix Calc. //But it doesnt seem to solve all prob.
               ltROI* blbroi = ltGetFirstROIContainingPoint(vRoi ,cv::Point(b->centroid.x,b->centroid.y) );
               if (blbroi == 0 )
                   continue;

               double d = round(sqrt( pow(b->centroid.x - track->centroid.x,2) + pow( (b->centroid.y - track->centroid.y),2)  ));

               /// Assigning Blob to track
               /// 1st Criterion to assignment is to minimize distance
               /// For equal distance among candidates Use Area
               if (*blbroi == *track->pROI)
               {
                   if (d<dist || (d==dist && b->area>area) )
                   {
                       area = b->area;
                       dist = d; //Change to New minimum Distance
                       blob = b;
                   }
               }

            }
          }
          //cout << endl;
          //KL: SKip If not found - Think this Matching is what updates the state of the tracks
          if  (track != NULL && blob != NULL )
          {

                  // Update track
                  //cout << "Matching: track=" << track->id << ", blob=" << blob->label << endl;
                  track->label = blob->label;
                  //Calc Last Displacement as a figure of last speed
                  track->effectiveDisplacement = 1.0 + round(sqrt( pow(track->centroid.x - blob->centroid.x,2) + pow( (track->centroid.y - blob->centroid.y),2)  )); // round(distantBlobTrack(blob,track)+0.5)
                  track->centroid = blob->centroid;
                  //KL: Make A point list
                  track->pointStack.push_back(std::pair<cv::Point,int>(cv::Point(blob->centroid.x,blob->centroid.y),gframeLumValue)); //KL:Add The new point to the List

                  track->minx = blob->minx;
                  track->miny = blob->miny;
                  track->maxx = blob->maxx;
                  track->maxy = blob->maxy;

                  if (track->inactive)
                    track->active = 0;
                  track->inactive = 0;
           }


          // Others to inactive
          for (list<CvTrack*>::const_iterator it=tt.begin(); it!=tt.end(); ++it)
          {
            CvTrack *t = *it;

            if (t!=track)
            {
              //cout << "Inactive: track=" << t->id << endl;
              t->inactive++;
              t->label = 0;
            }
          }
        } //If c
      }// Loop Over nTracks (CLUSTERINg)
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///Erase short Tracks (< gminTrackLength) that have been Inactive for min time of thInactive
      for (CvTracks::iterator jt=tracks.begin(); jt!=tracks.end();)

        if ((jt->second->inactive>=thInactive && (jt->second->pointStack.size() < gminTrackLength)) || ((jt->second->inactive)&&(thActive)&&(jt->second->active<thActive)))
        {
          delete jt->second;
          tracks.erase(jt++);
        }
        else
        {
          jt->second->lifetime++;
          if (!jt->second->inactive)
            jt->second->active++;
          ++jt;
        }

    } //Closes Huge Try block
        catch (...)
        {
          delete[] close;
          throw; // TODO: OpenCV style.
        }

        delete[] close;

    __CV_END__;
  }

  CvFont *defaultFont = NULL;

  void cvRenderTracks(CvTracks const tracks,std::vector<unsigned int>& vLumRec, IplImage *imgSource, IplImage *imgDest, unsigned short mode, CvFont *font )
  {
    CV_FUNCNAME("cvRenderTracks");
    __CV_BEGIN__;

    CV_ASSERT(imgDest&&(imgDest->depth==IPL_DEPTH_8U)&&(imgDest->nChannels==3));



    if ((mode&CV_TRACK_RENDER_ID)&&(!font))
    {
      if (!defaultFont)
      {
	font = defaultFont = new CvFont;
	cvInitFont(font, CV_FONT_HERSHEY_DUPLEX, 0.5, 0.5, 0, 1);
	// Other fonts:
	//   CV_FONT_HERSHEY_SIMPLEX, CV_FONT_HERSHEY_PLAIN,
	//   CV_FONT_HERSHEY_DUPLEX, CV_FONT_HERSHEY_COMPLEX,
	//   CV_FONT_HERSHEY_TRIPLEX, CV_FONT_HERSHEY_COMPLEX_SMALL,
	//   CV_FONT_HERSHEY_SCRIPT_SIMPLEX, CV_FONT_HERSHEY_SCRIPT_COMPLEX
      }
      else
	font = defaultFont;
    }

    if (mode)
    {
        for (CvTracks::const_iterator it=tracks.begin(); it!=tracks.end(); ++it)
        {
            cvRenderTrack(*((*it).second),vLumRec ,it->first , imgSource, imgDest, mode, font );
        }
    }

    __CV_END__;
  }


  void cvRenderTrack(CvTrack& track,std::vector<unsigned int>& vLumRec, const unsigned int trackID, IplImage *imgSource, IplImage *imgDest, unsigned short mode, CvFont *font )
  {
      CV_FUNCNAME("cvRenderTrack");
      __CV_BEGIN__;

      int skipFrame = gdvidfps/gdLumRecfps; //Use Ratio of fps to calculate Frame Lag before drawing 1st track segment

        if (mode&CV_TRACK_RENDER_ID)
          if (!track.inactive)
          {
            stringstream buffer;
            buffer << trackID;
            cvPutText(imgDest, buffer.str().c_str(), cvPoint((int)track.centroid.x, (int)track.centroid.y), font, CV_RGB(0.,255.,0.));
          }

        if (mode&CV_TRACK_RENDER_BOUNDING_BOX)
          if (track.inactive)
            cvRectangle(imgDest, cvPoint(track.minx, track.miny), cvPoint(track.maxx-1,track.maxy-1), CV_RGB(0., 0., 50.));
          else
            cvRectangle(imgDest, cvPoint(track.minx, track.miny), cvPoint(track.maxx-1, track.maxy-1), CV_RGB(0., 0., 255.));

        if (mode&CV_TRACK_RENDER_TO_LOG)
        {
          clog << "Track " << track.id << endl;
          if (track.inactive)
            clog << " - Inactive for " << track.inactive << " frames" << endl;
          else
            clog << " - Associated with blob " << track.label << endl;
          clog << " - Lifetime " <<track.lifetime << endl;
          clog << " - Active " << track.active << endl;
          clog << " - Bounding box: (" << track.minx << ", " <<track.miny << ") - (" << track.maxx << ", " << track.maxy << ")" << endl;
          clog << " - Centroid: (" << track.centroid.x << ", " << track.centroid.y << ")" << endl;
          clog << endl;
        }

        if (mode&CV_TRACK_RENDER_TO_STD)
        {
          cout << "Track " << track.id << endl;
          if (track.inactive)
            cout << " - Inactive for " <<track.inactive << " frames" << endl;
          else
            cout << " - Associated with blobs " << track.label << endl;
          cout << " - Lifetime " << track.lifetime << endl;
          cout << " - Active " << track.active << endl;
          cout << " - Bounding box: (" <<track.minx << ", " << track.miny << ") - (" << track.maxx << ", " <<track.maxy << ")" << endl;
          cout << " - Centroid: (" << track.centroid.x << ", " << track.centroid.y << ")" << endl;
          cout << endl;
        }


        //Render Path
        //cv::Mat img = cv::Mat::zeros(400, 400, CV_8UC3);
        if (mode&CV_TRACK_RENDER_PATH) //With BIOLUM COLOURED Values
        {
            int c1 ; //Colour R
            int t1; //Line Thickness
            //std::vector<CvPoint>* pvec = &track.pointStack;
            //Lum Colour
            const int cavgSamples = 20;
            //Draw each seg with colour
            //Problem: How do we know each point on track corresponds to a video frame?
            for (int i=skipFrame+cavgSamples;i < track.pointStack.size();i++)
            {

                //unsigned int vLumIndex  = (unsigned int)(i/(double)skipFrame);
                double dnorm            = (double)(gmaxLumValue-gminLumValue);

                //Calc recent Avg Lum Signal
                int ilum = 0;
                for (int j=0;j<cavgSamples;j++)
                    if (track.pointStack[i-j].second >= gminLumValue)
                        ilum += track.pointStack[i-j].second;
                    else
                       ilum = -1;

               //Defaults to use When Lum Value is missing
                int ithickness = 1;
                cv::Scalar cvcolour(120,120,120);

                if (ilum > 0){ //only show where Lum Values Exist/ Otherwise Change Colour
                    ilum = min((int)gmaxLumValue,ilum/cavgSamples); //Calc Avg Lum, so as to colour the track smoothly
                    c1 =  255.0*(double)(ilum-gminLumValue)/dnorm; // 255.0*((double)vLumRec[vLumIndex]-gminLumValue)/dnorm;
                    cvcolour =  cymk2rgb(cv::Scalar(20,min(c1,255),max(255-c1,0),80)); //Replace With Lum Colour
                    ithickness = max(1,(int)(5.0*(float)ilum/dnorm));
                }
                cvLine(imgDest,track.pointStack[i-1].first,track.pointStack[i].first, cvcolour,ithickness,CV_AA,0);


                //Do regular Reporting of luminance value on the track / only if value high enough
                if ((mode & CV_TRACK_RENDER_LUM) && (i % TRACK_LUM_REPORT_INTERVAL == 0) && (ilum > gmaxLumValue/3 || (i==skipFrame+cavgSamples) ) ) ///Display Text of Lum Value
                {

                    CvFont* font =  new CvFont;
                    cvInitFont(font, CV_FONT_HERSHEY_DUPLEX, 0.6, 0.6, 0, 1);
                    stringstream buffer;
                    buffer << ilum;
                    //cvPutText(imgDest, buffer.str().c_str(), CvPoint(vsubSeg[0].x + 5,vsubSeg[0].y + 5), font, cvcolour);
                    cvPutText(imgDest, buffer.str().c_str(), CvPoint(track.pointStack[i-1].first.x + 5,track.pointStack[i-1].first.y + 5), font, cv::Scalar(20,255,0));

                }



            }
        } //Render Track

        if (mode & CV_TRACK_RENDER_HEATMAP) //With BIOLUM COLOURED Values
        {
            const float rangesteps = 50.0f;
            int c1                  = 0;
            double dnorm            = (double)(gmaxLumValue-gminLumValue);
            float increm              = (dnorm+1)/rangesteps;
            int xstart              = 20; //Position to start drawing heatmap
            int ystart              = 125;
            //Go through Range - In step increments
            int iclrVal; //Current Lum Value drawn in the map
            int ithickness = 20;

            for (int i=0; i < rangesteps;i++) //Split Full Range into 100 steps
            {
              iclrVal = gminLumValue + increm*i; //add increment * iteration
              c1  =  255.0*(double)(iclrVal-gminLumValue)/dnorm; // 255.0*((double)vLumRec[vLumIndex]-gminLumValue)/dnorm;

              cv::Scalar cvcolour =  cymk2rgb(cv::Scalar(20,min(c1,255),max(255-c1,0),80));


              ithickness = int(6+14.0*(double)(iclrVal-gminLumValue)/dnorm);
              //Paint line segment of heatmap
              cvLine(imgDest,cv::Point(xstart, ystart), cv::Point(xstart,ystart+5), cvcolour,ithickness,CV_AA,0);
              ystart+=5; //Move down for next heat map segment

              if (i==0 || i>= (rangesteps -1))
              {
                  CvFont* font =  new CvFont;
                  cvInitFont(font, CV_FONT_HERSHEY_DUPLEX, 0.8, 0.8, 0, 1);
                  stringstream buffer;
                  buffer << iclrVal;
                  cvPutText(imgDest, buffer.str().c_str(), CvPoint(xstart + 20,ystart), font, cvcolour);
              }

            }




        }




__CV_END__;
  }


} //END OF NAMESPACE

