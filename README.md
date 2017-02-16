# shadowbox

Shadowbox is currently being developed on an ODroid XU4 running Ubuntu.

## Dependencies

### OpenCV 3.X

On the XU4, when building OpenCV, there are several options which provide a
significant speedup (BasicTracer at this revision went from ~30fps to ~53fps):

```
cmake -D CMAKE_BUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=/usr/local -DENABLE_VFPV3=ON -DENABLE_NEON=ON
```

TODO: switch to Release build type

### SDL2
