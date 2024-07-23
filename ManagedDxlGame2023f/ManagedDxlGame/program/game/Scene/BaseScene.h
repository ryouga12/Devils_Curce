#pragma once
#include"../Object/Actor.h"
#include "../../dxlib_ext/dxlib_ext.h"

//シーンの基底クラス

class BaseScene {
public:

    virtual void Update(float delta_time);
    virtual void Draw();
  
    virtual ~BaseScene() {};

    enum class SceneState {
        TITTLE,
        INMAP,
        WORLDMAP,
        BATTLE,
        RESULT
    };

protected:

    SceneState curent_scene = SceneState::TITTLE;

    //Actorリスト
    std::list<Shared<Actor>>actor_list;
    
    //黒色
    const int COLOR_BLACK = 0;
    //白色
    const int COLOR_WHITE = -1;

};


