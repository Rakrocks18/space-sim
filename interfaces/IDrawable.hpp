#pragma once

#include "../src/shaderprogram.hpp"

class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw(const ShaderProgram& shader) const = 0;
};