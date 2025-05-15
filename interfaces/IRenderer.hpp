#pragma once

#include <memory>
#include <vector>

#include "IBody.hpp"

struct IRenderer {
    virtual ~IRenderer() {}
    virtual void renderBodies(const std::vector<std::shared_ptr<IBody>>& bodies) const =0;
    virtual void renderFabric() const =0;
};
