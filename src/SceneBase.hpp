//
//  SceneBase.hpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/09.
//
//

#ifndef SceneBase_hpp
#define SceneBase_hpp

class SceneBase{
public:
    virtual void setup(){};
    virtual void update(float dt){};
    virtual void draw(float vol){};
    virtual void randomize(){};
    virtual void setParam(int ch, float val){};
    virtual void debugDraw() const {};
};

#endif /* SceneBase_hpp */
