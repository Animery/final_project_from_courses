#pragma once

#include "../engine/engine.hpp"

#include "GameConst.hpp"
#include <memory>

class Player
{
public:
    Player(my_engine::RenderObj*, Animate::Texture*, Animate::Texture*, my_engine::SoundBuffer*);
    ~Player();
    void update(std::array<bool, 10>& controls, float delta);

    // my_engine::RenderObj* getCorpse();
    // my_engine::RenderObj* getHead();

    // my_engine::matrix2x3& getMatrix_corpse();
    // my_engine::matrix2x3& getMatrix_head();

    float getCurrent_head_direction();
    void  setCurrent_head_direction(const float a);
    void  setMouse_pos(const my_engine::vec2& a);

    my_engine::vec2& getCurrent_current_pos();
    my_engine::vec2  getPosition_A();
    my_engine::vec2  getPosition_B();

    float getHealth();
    void  setHealth(float damage);

    void render_player();

protected:
    my_engine::RenderObj* tank_obj       = nullptr;
    Animate::Texture*              texture_corpse = nullptr;
    Animate::Texture*              texture_head   = nullptr;

    my_engine::matrix2x3 matrix_corpse;
    my_engine::matrix2x3 matrix_head;

    my_engine::vec2 current_tank_pos       = { -1.0f, -0.00000000000000001f };
    float           half_size              = 0.025;
    float           current_tank_direction = 0.0f;
    float           current_head_direction = 0.0f;

    float health         = 1000;
    float speed          = 0.0003125f;
    float speed_diagonal = 0.00022097f;

    my_engine::SoundBuffer* sound_dmg;

private:
    void            update_Head_dirrection();
    my_engine::vec2 mouse_pos_vec = { 0, 0 };
};