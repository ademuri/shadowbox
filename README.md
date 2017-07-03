# shadowbox

Shadowbox is currently being developed on an ODroid XU4 running Ubuntu.

## System config

### Startup

Since Shadowbox needs do graphics stuff, it needs to be run in the context of
MATE. To do this, go to System > Preferences > Personal > Startup Applications,
and add the shadowbox binary. You should also add the Fadecandy server here.

### Drivers

The graphics drivers from the Odroid Github repos are more current than the
ones from their packages. This is also useful if you accidentally install the
Mesa drivers. Note that these drivers will be clobbered if you update the
mali-x11 package.

```bash
git clone https://github.com/mdrjr/5422_mali.git
cd x11
sudo make install
```

## Dependencies

### OpenCV 3.X

On the XU4, when building OpenCV, there are several options which provide a
significant speedup (BasicTracer at this revision went from ~30fps to ~53fps):

```
cmake -D CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=/usr/local -DENABLE_VFPV3=ON -DENABLE_NEON=ON -DWITH_TBB=ON ..
```

This require building TBB from source and running `source tbbvars.sh` from the
build folder before running cmake. OpenCV has an option to automatically build
TBB (-DBUILD_TBB), but it doesn't work with the above options.

TODO: switch to Release build type

### SDL2

### git-lfs

Shadowbox uses git-lfs to store video files for testing. There isn't a package
for it on arm, so you'll need to build it yourself. First [download
Golang](https://golang.org/dl/) and install it (as of writing, Go 1.7.3 is
required, and only 1.6 is available in the repos). Then, follow the instructions
[here](https://github.com/git-lfs/git-lfs/blob/master/CONTRIBUTING.md#building-git-lfs)
to build git-lfs. Copy the binary produced into `/usr/local/bin` and run `git
lfs install`.

### Google Test

Shadowbox uses Google Test, Google's C++ testing and mocking framework.
Download it from [the site](https://github.com/google/googletest), build it
with CMake, and `make install` it.

### Fadecandy

Shadowbox uses a Fadecandy to control "projector" lights that shine from the
camera on the underside of the screen, to create the "shadow" illusion. To set
up the Fadecandy server"

1. Clone the shadowbox repo:
```bash
git clone https://github.com/scanlime/fadecandy.git
cd fadecandy
```

Now, run the server:
```bash
bin/fcserver-rpi
```
