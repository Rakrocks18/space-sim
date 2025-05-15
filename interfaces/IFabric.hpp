#pragma once

struct IFabric {
    virtual ~IFabric() {}
    virtual void draw() const =0;
};
