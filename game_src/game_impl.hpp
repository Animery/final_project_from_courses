#pragma once

#include "Player.hpp"
#include "enemy/iEnemy.hpp"
#include "gun/gun.hpp"
#include "spawn_levels/spawn_enemy.hpp"

#include "../engine/sprite.hpp"

#include <memory>

namespace my_game
{

class game_impl : public my_engine::game
{
public:
    game_impl();
    ~game_impl();
    virtual bool getIsRunning() final;
    void         on_initialize() final;
    void         on_event(my_engine::event&) final;
    void         on_update(std::chrono::microseconds frame_delta) final;
    // void on_render(my_engine::RenderObj&, Texture&) const final;
    void on_render() final;
    void add_spider(my_engine::vec2 pos_enemy);
    void add_big_spider(my_engine::vec2 pos_enemy);
    void add_nest(my_engine::vec2 pos_enemy);

private:
    void update_imGui();
    void swap_gun();

    std::unique_ptr<Player> player;

    std::vector<std::unique_ptr<guns::Gun>> guns;
    guns::Gun*                              gun_current;
    uint8_t                                 gun_current_ID = 0;

    my_engine::gfx_prog* gfx_obj;
    my_engine::gfx_prog* gfx_map;
#if defined(TEST_VECTOR)
    std::vector<Enemy*> enemy_list;
#else
    std::deque<std::unique_ptr<enemy::iEnemy>> enemy_list;
#endif // TEST_VECTOR

    my_engine::RenderObj*          map_obj = nullptr;
    std::vector<Animate::Texture*> map_texture;

    my_engine::RenderObj*             tank_obj = nullptr;
    std::unique_ptr<Animate::Texture> texture_corpse;
    std::unique_ptr<Animate::Texture> texture_head;

    my_engine::RenderObj*             bullet_obj = nullptr;
    // std::unique_ptr<Animate::Texture> texture_bullet;
    std::unique_ptr<Animate::Texture> tex_light_shell;
    std::unique_ptr<Animate::Texture> tex_medium_shell;
    std::unique_ptr<Animate::Texture> tex_plasma;

    my_engine::RenderObj*             spider = nullptr;
    std::unique_ptr<Animate::Texture> texture_spider;


    my_engine::RenderObj* big_spider = nullptr;
    // std::unique_ptr<Animate::Texture> texture_big_spider;
    std::unique_ptr<Animate::sprite> spire_big_spider;
    my_engine::RenderObj*             fire_ball_obj = nullptr;
    std::unique_ptr<Animate::Texture> texture_fire_ball;

    my_engine::RenderObj*             nest_obj = nullptr;
    std::unique_ptr<Animate::Texture> tex_nest;

    std::unique_ptr<spawn::spawn_enemy> spawn_monster;
    size_t                              max_enemy = 2500;
    std::array<bool, 10>                controls{ false };

    // uint16_t                            count_spider = 0;

    std::vector<my_engine::SoundBuffer*> sounds;

    bool game = true;
    bool isRunning;
};

} // namespace my_game