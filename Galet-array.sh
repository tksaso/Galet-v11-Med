#! /bin/sh
#
#
# submit this like: 
#  $ bsub -J 'Galet[1-10]' -o "out.%J.%I" -e "err.%J.%I" ./Galet-array.sh
#
#
script_name=$(basename $0)
script_dir=$(cd $(dirname $0) && pwd)
dir_at_exec=$(cd . && pwd)
#
# You need to modify this for your Galet directry.  
galet_src_dir=${HOME}/Galet
galet_build_dir=${HOME}/Galet-build
#
# Check Geant4 version number and GCC version number.
geant4_version=10.4.0
gcc_version=485
#
#
workdir=$dir_at_exec/${LSB_JOBID}/${LSB_JOBINDEX}
mkdir -p $workdir
cd $workdir
#
module load gcc/$gcc_version
. /sw/packages/geant4/$geant4_version/bin/geant4.sh
/sw/packages/cmake3/bin/cmake -DCMAKE_CXX_COMPILER=$(which g++) $galet_src_dir
make
#
# Copy files. (Add lines if you need.) 
cp -r $galet_build_dir/data .
cp -r $galet_build_dir/material.mac .
cp -r $galet_build_dir/phys.mac .
cp -r $galet_build_dir/init.mac .
#
# Description of  Macro file
#
mac=sim.mac
cat <<EOF > $mac
/control/execute  init.mac
/run/printProgress 1000000
###
/random/setSeeds ${LSB_JOBID} ${LSB_JOBINDEX}
###
/gps/particle ion
/gps/ion 55 137
/gps/position 0 0 16 mm
/gps/energy 0 MeV
#
/run/beamOn 10000
EOF

./Galet -m $mac

exit 0
