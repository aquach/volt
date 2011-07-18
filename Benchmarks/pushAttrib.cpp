#include <iostream>
#include <SDL/SDL.h>
#include "Volt/Core/Time.h"
#include <GL/gl.h>

using namespace std;

GLuint texture;

class Benchmark {
public:
    Benchmark (string name) : name(name) { };
    virtual ~Benchmark () { }

    virtual void iterate () = 0;

    void Run () {
        int NUM_TESTS = 100000;

        Volt::Time t;
        t.Start();
        for (int i = 0; i < NUM_TESTS; i++) {
            iterate();
        }
        cout << name << ": " << t.GetMilliseconds() << " ms" << endl;
    }

    string name;
};

class QuickBenchmark : public Benchmark {
public:
    QuickBenchmark (string name, void(*func)())
        : Benchmark(name), func(func) { }
    virtual ~QuickBenchmark () { }

    virtual void iterate () {
        func();
    }

    void(*func)();
};

void QuickTest (string name, void(*func)()) {
    QuickBenchmark b(name, func);
    b.Run();
}

void pushAttribAll () {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 0.5);
    glPopAttrib();
}

void pushAttribSmall () {
    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 0.5);
    glPopAttrib();
}

void nopushAttrib () {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 0.5);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1, 1, 1);
}

void bindOnce () {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_TEXTURE_2D);
}

void bindALot () {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDisable(GL_TEXTURE_2D);
}

void flippingTexture () {
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);
}

void changeOnce () {
    glLineWidth(6.0);
    glLineWidth(1.0);
}

void changeALot () {
    glLineWidth(6.0);
}

void textureNone () {
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glTexCoord2i(1, 0);
    glVertex2i(50, 0);
    glTexCoord2i(0, 1);
    glVertex2i(0, 50);
    glEnd();
}

void textureUnbound () {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_TRIANGLES);
    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glTexCoord2i(1, 0);
    glVertex2i(50, 0);
    glTexCoord2i(0, 1);
    glVertex2i(0, 50);
    glEnd();
}

int main (int argc, char** argv) {
    // Initialize screen buffer
    SDL_Surface *screen;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    if (!(screen = SDL_SetVideoMode(1024, 768, 0, SDL_SWSURFACE | SDL_OPENGL))) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        exit(1);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512,
                 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    QuickTest("pushAttribAll", pushAttribAll);
    QuickTest("pushAttribSmall", pushAttribSmall);
    QuickTest("nopushAttrib", nopushAttrib);
    QuickTest("bindOnce", bindOnce);
    QuickTest("bindALot", bindALot);
    QuickTest("changeOnce", changeOnce);
    QuickTest("changeALot", changeALot);
    QuickTest("flippingTexture", flippingTexture);
    QuickTest("textureNone", textureNone);
    QuickTest("textureUnbound", textureUnbound);
    
    SDL_Quit();

    return 0;
}
