# README #


### What is this repository for? ###

* Quick summary
A software that tracks moving larvae in timelapse videos, exporting 3 data csv files for each region of interest - vial. _N file contains the larvae count per frame, _pos contains position information of identified blobs (most possibly larvae) in the ROI, _pos_track.csv file exports the data from a basic tracking algorithm that attempts to link blobs across frames via track ID. 
The package further contains a subfolder /matlabscripts, that allows for importing and analysing basic statistics of the exported data. 
The data is not assumed clean, and some problems from the tracker are dealt with within the those scripts to clear the data.
* Version
* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)


### Using the app ###
User:
 * Chooses input video file, then on the second dialogue choose the text file to export track info in CSV format.
The directory where the data csv files are exported must have  format EXP_N_YYYYMMDD_Ssec giving the experiment number N (can be a range or anything really),the date when the embryos were collected, and the timelapse frame period in seconds).

 * The video begins paused -  use to left mouse clicks to define a new region in the image over which you want to count the larvae.
 * Define the regions of interest using two left mouse click for each region. The red box defines the region over which the larvae are counted-tracked and recorded to file VN_XXX.csv - with N defined by the order by which the ROI was created. First ROI 1, 2nd ROI gets V2_...


 * Press p to pause Image. once paused:
 *  s to save snapshots in CSV outdir pics subfolder.
 *  2 Left Clicks to define the 2 points of region-of interest for tracking.
 *  m to show the masked image of the larva against BG.
 *  t Start Tracking
 *  q Exit Quit application
 * left click while tracking allows manual track point to be added to the track closest to click point 

### How do I get set up? ###

* Summary of set up
Install opencv 3.0 / best compiled with with flag -D WITH_QT=ON, improves experience and range of available functions.
http://opencv.org/downloads.html

* Uses a version of cvBlob which is embedded in the package, for independent development.
* Configuration
* Dependencies
 openCV3.0 (compilied with WITH_QT support)
 Qt4


* How to run tests

* Deployment instructions


### Contribution guidelines ###

* Writing tests
* Code review
* Other guidelines

### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact