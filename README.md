# bioFosSystem

-Contains support code and CAD design files used in making of the demo bioluminescence apparatus described in the paper
by Marescotti, Lagogiannis, Davies, Webb and Armstrong 2018. 

The subfolder code contains the tracker used to analyse the larva videos and superimpose the bioluminescence data.
It also contains the firmware and supporting scripts for making the software required for recording from the CO2 sensor. 
The sensor is made up of an the MG811 sensor connected to the analog ports of an arduino nano, that does the Analog to Digital conversion 
along with a little noise filtering, aand then sends these to a PC where it is recorded, along with the PMT, and video data.



