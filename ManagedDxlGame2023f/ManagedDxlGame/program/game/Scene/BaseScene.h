#pragma once
#include"../Object/Actor.h"
#include "../../dxlib_ext/dxlib_ext.h"

//�V�[���̊��N���X

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

    //Actor���X�g
    std::list<Shared<Actor>>actor_list;
    
    //���F
    const int COLOR_BLACK = 0;
    //���F
    const int COLOR_WHITE = -1;

};


