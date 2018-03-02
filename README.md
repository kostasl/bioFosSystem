# bioFosSystem
## Bioluminescence Demo Supporting Material 2018

A repository containing support code and CAD design files that can be used to make with the bioluminescence apparatus method described in the paper "Monitoring brain activity and behavior in freely moving Drosophila larvae using bioluminescence" by Marescotti, Lagogiannis, Webb, Davies, and Armstrong 2018. 

The subfolder code contains the tracker used to analyse the larva videos and superimpose the bioluminescence data.
It also contains the firmware and supporting scripts for making the software required for recording from the CO2 sensor. 
The sensor is made up of an the MG811 sensor connected to the analog ports of an arduino nano, that does the Analog to Digital conversion 
along with a little noise filtering, aand then sends these to a PC where it is recorded, along with the PMT, and video data.


<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.

