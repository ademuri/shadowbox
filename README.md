# shadowbox

Shadowbox is currently being developed on an ODroid XU4 running Ubuntu.

## Dependencies

### OpenCV 3.X

On the XU4, when building OpenCV, there are several options which provide a
significant speedup (BasicTracer at this revision went from ~30fps to ~53fps):

```
cmake -D CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=/usr/local -DENABLE_VFPV3=ON -DENABLE_NEON=ON -DWITH_TBB=ON ..
```

This requires building TBB from source and running `source tbbvars.sh` from the
build folder before running cmake. OpenCV has an option to automatically build
TBB (-DBUILD_TBB), but it doesn't work with the above options.

TODO: switch to Release build type

### SFML

Shadowbox uses SFML as its graphics framework. SFML is an object-oriented 2D
graphics framework that (in our case) uses OpenGL ES. On the XU4, you'll need
to build it yourself, since the official package comes with the mesa drivers
for OpenGL (not ES) support. The mesa drivers (which are software only) don't
  play nicely with the hardware-accelerated Mali drivers.

You'll need to install a few packages to build SFML:

```
sudo apt-get install libvorbis-dev libflac-dev libopenal-dev libudev-dev
```

Then just grab the source from the website, configure it for OpenGL ES support,
and build it..

### Mali drivers (XU4)

The Mali drivers that ship with Hardkernel's build of Ubuntu Xenial are not
current. When I switched to the latest driver from Hardkernel's github, I saw a
moderate performance improvement. Installing these will also fix the drivers if
you accidentally install the Mesa drivers.

Get the Mali drivers from [Hardkernel's
github](https://github.com/mdrjr/5422_mali) and run 'sudo make install' in the
x11 directory.
