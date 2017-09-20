#!/bin/bash
echo "Run by run analysis"
#PERIOD="LHC15o"
#TRAIN=771

PERIOD="LHC16t"
TRAIN=846

#CODEDIR="$ALICE_PHYSICS/../../../../ali-master/AliPhysics/PWGPP/EMCAL/BCMacros/runAnalysisBC.C"
CODEDIR="$ALICE_WORK_DIR/../ali-master/AliPhysics/PWGPP/EMCAL/BCMacros/runAnalysisBC.C" 
BASEDIR=$PWD
BASEFOLDER="AnalysisInput"
BASE=$BASEDIR/$BASEFOLDER
#SUFFILE="_AnyINTnoBCFiltered.root"
SUFFILE="_INT7Filtered.root"

#####LHC15o#####
#for j in 244917 244918 244975 244980 244982 244983 245061 245064 245066 245068 245145 245146 245151 245152 245231 245232 245259 245343 245345 245346 245347 245349 245353 245396 245397 245401 245407 245409 245411 245439 245441 245446 245454 245496 245497 245501 245504 245505 245507 245535 245540 245542 245543 245544 245545 245554 245683 245700 245702 245705 245738 245829 245831 245833 245949 245952 245954 245963 246001 246003 246037 246042 246052 246053 246087 246089 246113 246115 246217 246222 246225 246271 246272 246390 246391 246392 246424 246434 246487 246488 246493 246495 246750 246751 246757 246758 246759 246760 246765 246766 246804 246805 246807 246808 246809 246810 246844 246845 246846 246928 246945 245148 246575 246583 246648
#for j in 244918 
#for j in 244918 244975 244980 244982 244983 245064 245066 245068 245145 245146 245151 245152 245231 245232 245259
#for j in 244917 244918
#do

#####LHC16l#####
#for j in 259888 259868 259867 259866 259860 259842 259841 259822 259788 259781 259756 259752 259751 259750 259748 259747 259711 259704 259703 259697 259668 259650 259649 259477 259473 259396 259395 259394 259389 259388 259382 259378 259342 259341 259340 259339 259336 259334 259307 259305 259302 259274 259273 259272 259271 259270 259269 259263 259164 259118 259117 259099 259096 259091 259090 259088 258964 258962 258923 258919
#do

#####LHC16r#####
#for j in 266208 266197 266196 266187 265744 266304 266299 266296 266193 266189 266117 266086 266085 266084 266083 266081 266034 265797 265795 265789 265788 265756 265742 265741 265714 265713 265709 265705 265701 265700 265698 265697 265696 265669 265665 265658 265656 265632 265630
#for j in 266318 266317 266316
#do

#####LHC16s#####
#for j in 267110 267081 267077 267072 267070 266998 266997 266994 266993 266944 266886 266885 266883 266882 266437 266881 266549 266543 266441 266439 266438 266405
#do

#####LHC16t#####
for j in 267166 267165 267164 267163 267161
do

   root -b -q $CODEDIR+\(-1,\"$PERIOD\",\"Train_$TRAIN\",\"INT7\",$j,\"$j$SUFFILE\",\"\",\".\"\)
done
