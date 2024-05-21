#pragma once
//シーンの基底クラス


class BaseScene {
public:

    virtual void Update(float delta_time) {};
    virtual void Draw() = 0;

    virtual ~BaseScene() {};
};


