#pragma once
//�V�[���̊��N���X


class BaseScene {
public:

    virtual void Update(float delta_time) {};
    virtual void Draw() = 0;

    virtual ~BaseScene() {};
};


