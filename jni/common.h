#ifndef PEGAS_ENGINE_H_
#define PEGAS_ENGINE_H_

#ifndef ANDROID
#define ANDROID
#endif

#ifndef PEGAS_USE_GLES_1x
#define PEGAS_USE_GLES_1x
#endif

#ifndef PEGAS_USE_SCREEN_COORDS
#define PEGAS_USE_SCREEN_COORDS
#endif

//Standart lib
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>


//STL
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>


//Android
#include <android_native_app_glue.h>

//OpenGL ES v1.0
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

//Rapid XML
#include "xml/rapidxml.hpp"

//LibPNG
#include <png.h>

#include "core/includes.h"

#endif /* ENGINE_H_ */
