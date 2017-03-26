# shadowbox

Shadowbox is currently being developed on an ODroid XU4 running Ubuntu.

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
