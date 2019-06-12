#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>

// Shadow Mapping
GLuint shadow_fbo;
GLuint shadow_tex;
GLuint shadow_depth;

//TODO
inline void shadowInitialize(int frameBufferWidth, int frameBufferHeight, int unit)
{

}

inline void reallocateShadowTexture(int frameBufferWidth, int frameBufferHeight, int unit)
{
    glDeleteTextures(1, &shadow_tex);
    glDeleteTextures(1, &shadow_depth);
	shadowInitialize(frameBufferWidth, frameBufferHeight, unit);
}

inline void deleteShadowResources()
{
    glDeleteTextures(1, &shadow_tex);
    glDeleteRenderbuffers(1, &shadow_depth);
    glDeleteFramebuffers(1, &shadow_fbo);
}