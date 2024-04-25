#ifndef TEXTURE_BAKER_HPP
#define TEXTURE_BAKER_HPP

#include <imgui.h>
#include <cstdlib>
#include "glpp/glpp.hpp"
#include "img/src/Image.h"

class TextureBaker {
public:
    static GLuint bake(img::Image& img);
};

#endif
