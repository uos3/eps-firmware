# Code for the Electronic Power Supply (EPS) board of the UoS3 CubeSat

This repository's base commit contains the code submitted by the Electronics and Computer Science (ECS) Group-Design Project (GDP). This GDP made significant progress on the software, however there were big chunks left out. These have been worked on by the recent commits.

## How to start
For setting-up the hardware and software development environment to debug or flash new code to the EPS computer, please refer to the EPS_Flashing_Guide on the private groupsite.

## Current State
The current software is very close to finished. It only needs testing on hardware to verify, with perhaps some small changes. **When this is done in confidence, the next commit should be finalised with a suffix of V1.0**; the EPS-firmware should be left alone as early as possible to focus on the ambient testing campaign. Bug fixing should then occur on a separate dev branch, and integrated into master as V1.x when ready. 

## Further details
If given time, there is room for additional functionality which can be help diagnose problems with the battery. Refer to pages 49-51 of the battery report/datasheet for more detail.

Refer to the following for more detail about remainings tasks in general:
- GitHub issues list
- Trello (Software and EPS lists)

#### Commit Notation
NC - Not compiled
C - Compiled, not hardware-tested
F - Hardware-tested and working

#### Reference Documents
- Battery Datasheet/Report (UoS3 cubesat > Internships 2018 > Electrical Power Subsystem > Datasheets > Battery Board > Battery User Manual Rev. C)
- Software September Summary (ECS GDP Report + EPS flashing guide)
 
_Code is owned by the University of Southampton or the European Space Agency (ESA) depending on who wins the legal quarrel.
Contact Tomas Piga for any questions._
