Welcome to AML v1.0

AML stands for Anisotropic Morphological Leveling as proposed by K.Karantzalos and D Argialas, 2006
The algorithm is implemented in C with some optimizations for remote sensing imagery. 

AML is Free Software licenced under the GPLv3.

Installation:

1. Dependencies: CMake, OpenCV

2. Run cmake configuration:

cmake -DCMAKE_BUILD_TYPE:STRING="Release" \		#or "Debug"
      -DCMAKE_INSTALL_PREFIX:PATH=/usr \
      -DLIB_SUFFIX="64" 				#use in case of 64bit OS

3. Compile the sources:

make

4. Install:

make install

For problems / suggestions / bugs etc please contact Angelos Tzotsos (tzotsos@gmail.com)