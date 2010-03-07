#ifndef OGL_H
#define OGL_H

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX	/* Don't defined min() and max() */
#endif
#include <windows.h>
#endif

#if defined(__MACOSX__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(__MACOS__)
#include <gl.h>
#include <glu.h>
#else
#include <GL/glee.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#endif

#endif
