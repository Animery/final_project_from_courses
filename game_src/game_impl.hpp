#pragma once

#include "Player.hpp"
#include "enemy/iEnemy.hpp"
#include "gun/gun.hpp"
#include "spawn_levels/spawn_enemy.hpp"

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

private:
    void update_imGui();
    void swap_gun();

    std::unique_ptr<Player>    player;
    guns::Gun* gun_current;
    uint8_t gun_current_ID = 0;

    std::vector<std::unique_ptr<guns::Gun>> guns;

    my_engine::gfx_prog* gfx_obj;
    my_engine::gfx_prog* gfx_map;
#if defined(TEST_VECTOR)
    std::vector<Enemy*> enemy_list;
#else
    std::deque<std::unique_ptr<enemy::iEnemy>> enemy_list;
#endif // TEST_VECTOR

    std::vector<Texture*>    map_texture;
    std::unique_ptr<Texture> texture_corpse;
    std::unique_ptr<Texture> texture_head;
    std::unique_ptr<Texture> texture_bullet;
    std::unique_ptr<Texture> texture_spider;
    std::unique_ptr<Texture> texture_big_spider;

    my_engine::RenderObj* tank_obj   = nullptr;
    my_engine::RenderObj* bullet_obj = nullptr;
    my_engine::RenderObj* spider     = nullptr;
    my_engine::RenderObj* big_spider = nullptr;
    my_engine::RenderObj* map_obj    = nullptr;

    std::array<bool, 10> controls{ false };

    std::unique_ptr<spawn::spawn_enemy> spawn_monster;
    size_t                              max_enemy = 1000;
    // uint16_t                            count_spider = 0;

    std::vector<my_engine::SoundBuffer*> sounds;

    bool isRunning;
};

} // namespace my_game