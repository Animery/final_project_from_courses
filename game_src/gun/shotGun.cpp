#include "shotGun.hpp"
// #include "../include/engine.hpp"
#include <algorithm>

namespace guns
{

shotGun::shotGun(Texture* temp_tex_bul, my_engine::RenderObj* temp_bul_obj)
// : readyGun(true)
// , texture_bullet(temp_tex_bul)
// , bullet_obj(temp_bul_obj)
{
    readyGun       = true;
    texture_bullet = temp_tex_bul;
    bullet_obj     = temp_bul_obj;

    timer_to_shoot.setCallback([&]() { readyGun = true; });
    timer_to_clip.setCallback([&]() {
        currentClip = maxClip;
        readyGun    = true;
    });
    std::cout << "size Bullet" << sizeof(Bullet) << std::endl;
    std::cout << "+++ ctor shotGun" << std::endl;
}

shotGun::~shotGun()
{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        // delete it.operator*();
        it = bullets.erase(it);
    }
    std::cout << "--- destor shotGun" << std::endl;
}

void shotGun::shoot(my_engine::vec2& temp_position, float temp_direction)
{
    if (readyGun)
    {
        for (size_t i = 0; i < 10; i++)
        {
            float           rand_factor = rand() % 1001 / 10000.f - 0.05f;
            my_engine::vec2 rand_pos{
                temp_position.x +
                    std::abs(temp_position.x * (rand_factor / 1.5f)),
                temp_position.y +
                    std::abs(temp_position.y * (rand_factor / 1.5f))
            };
            bullets.push_back(std::make_unique<Bullet>(
                rand_pos,
                temp_direction + temp_direction * rand_factor / 1.f,
                speed_bullet,
                damage_bullet + damage_bullet * rand_factor * 10));
        }

        --currentClip;
        readyGun = false;

        if (currentClip > 0)
        {
            timer_to_shoot.start(speed_shoot);
        }
        else
        {
            timer_to_clip.start(speed_reload);
        }
    }
}

std::string_view shotGun::getNameGun()
{
    return name.data();
}

uint16_t shotGun::getMaxClip() const
{
    return maxClip;
}

uint16_t shotGun::getCurrentClip() const
{
    return currentClip;
}

#if defined(TEST_VECTOR)
void shotGun::update_gun(float delta, std::vector<Enemy*>& enemy_list)
#else
void shotGun::update_gun(float delta, std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list)
#endif // TEST_VECTOR
{
    update_bullets(delta, enemy_list);

    timer_to_shoot.update_timer(delta);
    timer_to_clip.update_timer(delta);
}

unsigned int shotGun::count_bullets()
{
    return bullets.size();
}

void shotGun::render_bullets()
{
    for (auto&& bullet : bullets)
    {
        my_engine::render(*bullet_obj, *texture_bullet, bullet->getMatrix());
    }
}

#if defined(TEST_VECTOR)
void shotGun::update_bullets(float delta, std::vector<Enemy*>& enemy_list)
#else
void shotGun::update_bullets(float delta, std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list)
#endif // TEST_VECTOR
{
    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),
            [&enemy_list, delta, this](std::unique_ptr<Bullet>& elem) {
                (*elem).update_bullet(delta);
                return check_collision(elem.get(), enemy_list) ||
                       out_screen(elem.get());
            }),
        bullets.end());
}

// #if defined(TEST_VECTOR)
// std::vector<Bullet*>* shotGun::getList_bullets()
// #else
// std::deque<Bullet*>* shotGun::getList_bullets()
// #endif // TEST_VECTOR
// {
//     return &bullets;
// }

#if defined(TEST_VECTOR)
bool shotGun::check_collision(Bullet* bullet, std::vector<Enemy*>& enemy_list)
#else
bool shotGun::check_collision(Bullet* bullet, std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list)
#endif // TEST_VECTOR

{
    my_engine::vec2 pos_bullet_A = bullet->getPosition_A();
    my_engine::vec2 pos_bullet_B = bullet->getPosition_B();

    for (auto enemy = enemy_list.begin(); enemy != enemy_list.end();)
    {
        my_engine::vec2 pos_enemy_A = (*enemy)->getPosition_A();
        my_engine::vec2 pos_enemy_B = (*enemy)->getPosition_B();
        if (my_engine::vec2::check_AABB(
                pos_bullet_A, pos_bullet_B, pos_enemy_A, pos_enemy_B))
        {
            // TODO
            (*enemy)->setHealth(bullet->getDamage());
            
#ifdef DEBUG_LEVEL
            std::cout << "hit" << std::endl;
            std::cout << "health enemy:\t" << (*enemy)->getHealth()
                      << std::endl;
#endif

            return true;
        }
        else
        {
            enemy++;
        }
    }
    return false;
}

bool shotGun::out_screen(const Bullet* bullet)
{
    return bullet->getPosition().x > (1 / gameConst::size) ||
           bullet->getPosition().x < -(1 / gameConst::size) ||
           bullet->getPosition().y >
               (1 / gameConst::aspect) / gameConst::size ||
           bullet->getPosition().y < -(1 / gameConst::aspect) / gameConst::size;
}

} // namespace guns