#pragma once

class SceneBase {
public:
    virtual ~SceneBase() = default;

    virtual void setup() {}
    virtual void update(float dt) {}
    virtual void draw(float vol) {}
    virtual void randomize() {}
    virtual void setParam(int ch, float val) {}
    virtual void debugDraw() const {}
};