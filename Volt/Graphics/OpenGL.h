#include <gl/glew.h>
#include <SDL/SDL.h>
#if COMPILER_VCC
    #define NO_SDL_GLEXT
    #include <SDL/SDL_OpenGL.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
