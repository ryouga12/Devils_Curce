#pragma once
#include"../Object/Actor.h"
#include "../../dxlib_ext/dxlib_ext.h"
//シーンの基底クラス


class BaseScene {
public:

    virtual void Update(float delta_time) {};
    virtual void Draw() = 0;

    virtual ~BaseScene() {};

protected:

   /* Shared<Inventory>inventory = nullptr;
    Shared<Player>player = nullptr;
    Shared<KonCamera>camera = nullptr;
    Shared<Skill>skill = nullptr;*/

    //Actorリスト
    std::list<Shared<Actor>>actor_list;
    
};


