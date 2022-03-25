
set -e

# Required packages
sudo apt-get install -y dpkg-dev cmake g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev python libssl-dev

# Most common optional packages
sudo apt-get install -y gfortran libpcre3-dev xlibmesa-glu-dev libglew1.5-dev libftgl-dev libmysqlclient-dev libfftw3-dev libcfitsio-dev graphviz-dev libavahi-compat-libdnssd-dev libldap2-dev python-dev libxml2-dev libkrb5-dev libgsl0-dev libz3-dev libmpich-dev

sudo apt-get autoremove -y



ROOT_VERSION_MAJOR=6
ROOT_VERSION_MINOR=26
ROOT_VERSION_PATCH=00

CORE=20 ########################################### number of thread to build ROOT. 

TOOL_DIR=/home/software #################################################### directory where to install ROOT
BASH_FILE=/home/USERNAME/.profile ############################################### replace USERNAME



ROOT_VERSION=$ROOT_VERSION_MAJOR.$ROOT_VERSION_MINOR.$ROOT_VERSION_PATCH
TDIR=$TOOL_DIR/ROOT-v$ROOT_VERSION
SOURCE_DIR=$TDIR/root-$ROOT_VERSION-source
BUILD_DIR=$TDIR/root-$ROOT_VERSION-build
INSTALL_DIR=$TDIR/root-$ROOT_VERSION-install


mkdir -p $TDIR && cd $TDIR
echo TDIR is $TDIR
if [ -e $TDIR/building_env.txt ]; then
        rm $TDIR/building_env.txt
fi
gcc --version 1>> $TDIR/building_env.txt
echo >> $TDIR/building_env.txt
cmake --version >> $TDIR/building_env.txt
echo >> $TDIR/building_env.txt
fftw-wisdom --version >> $TDIR/building_env.txt


mkdir -p $TDIR && cd $TDIR
git clone --branch latest-stable https://github.com/root-project/root.git $SOURCE_DIR
mkdir -p $BUILD_DIR $INSTALL_DIR && cd $BUILD_DIR
cmake $SOURCE_DIR -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -Dasimage=ON -Dmathmore=On -Dminuit2=ON -Dminuit2_mpi=ON -Dminuit2_omp=ON -Dxml=ON -Dimt=ON -Dspectrum=ON
cmake --build . -- install -j$CORE
cd ..

echo alias ROOT$ROOT_VERSION_MAJOR.$ROOT_VERSION_MINOR.p$ROOT_VERSION_PATCH=\'source $INSTALL_DIR/bin/thisroot.sh\' >> $BASH_FILE
echo
echo alias ROOT$ROOT_VERSION_MAJOR.$ROOT_VERSION_MINOR.p$ROOT_VERSION_PATCH added to $BASH_FILE
echo
