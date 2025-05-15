#pragma once

#include <vector>
#include <memory>

#include "IBody.hpp"

struct ISimulation {
    virtual ~ISimulation() {}
    virtual void update(float dt) =0;
    virtual const std::vector<std::shared_ptr<IBody>>& bodies() const =0;
};