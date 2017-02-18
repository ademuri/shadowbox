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
