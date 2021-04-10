#include "shotGun.hpp"
#include <algorithm>

namespace guns
{

shotGun::shotGun()
    : readyGun(true)
{
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
        delete it.operator*();
        it = bullets.erase(it);
    }
    std::cout << "--- destor shotGun" << std::endl;
}

void shotGun::shoot(my_engine::vec2& temp_position, float temp_direction)
{
    if (readyGun)
    {
        for (size_t i = 0; i < 7; i++)
        {
            float           rand_factor = rand() % 1001 / 10000.f - 0.05f;
            my_engine::vec2 rand_pos{
                temp_position.x +
                    std::abs(temp_position.x * (rand_factor / 1.5f)),
                temp_position.y +
                    std::abs(temp_position.y * (rand_factor / 1.5f))
            };
            bullets.push_back(
                new Bullet(rand_pos,
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
void shotGun::update_gun(float delta, std::deque<Enemy*>& enemy_list)
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

#if defined(TEST_VECTOR)
void shotGun::update_bullets(float delta, std::vector<Enemy*>& enemy_list)
#else
void shotGun::update_bullets(float delta, std::deque<Enemy*>& enemy_list)
#endif // TEST_VECTOR
{
    // // TODO Anton helper
    // bullets.erase(
    //     std::remove_if(
    //         bullets.begin(),
    //         bullets.end(),
    //         [&enemy_list, delta, this](Bullet* elem) {
    //             elem->update_bullet(delta);
    //             return check_collision(elem, enemy_list) ||
    //                    elem->getPosition().x > (1 / gameConst::size) ||
    //                    elem->getPosition().x < -(1 / gameConst::size) ||
    //                    elem->getPosition().y >
    //                        (1 / gameConst::aspect) / gameConst::size ||
    //                    elem->getPosition().y <
    //                        -(1 / gameConst::aspect) / gameConst::size;
    //         }),
    //     bullets.end());

    for (auto it = bullets.begin(); it != bullets.end();)
    {
        it.operator*()->update_bullet(delta);

        if (check_collision(it.operator*(), enemy_list) ||
            it.operator*()->getPosition().x > (1 / gameConst::size) ||
            it.operator*()->getPosition().x < -(1 / gameConst::size) ||
            it.operator*()->getPosition().y >
                (1 / gameConst::aspect) / gameConst::size ||
            it.operator*()->getPosition().y <
                -(1 / gameConst::aspect) / gameConst::size)
        {
            delete it.operator*();
            it = bullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

#if defined(TEST_VECTOR)
std::vector<Bullet*>* shotGun::getList_bullets()
#else
std::deque<Bullet*>* shotGun::getList_bullets()
#endif // TEST_VECTOR
{
    return &bullets;
}

#if defined(TEST_VECTOR)
bool shotGun::check_collision(Bullet* bullet, std::vector<Enemy*>& enemy_list)
#else
bool shotGun::check_collision(Bullet* bullet, std::deque<Enemy*>& enemy_list)
#endif // TEST_VECTOR

{
    my_engine::vec2 pos_bullet_A = bullet->getPosition_A();
    my_engine::vec2 pos_bullet_B = bullet->getPosition_B();

    for (auto enemy = enemy_list.begin(); enemy != enemy_list.end();)
    {
        my_engine::vec2 pos_enemy_A = enemy.operator*()->getPosition_A();
        my_engine::vec2 pos_enemy_B = enemy.operator*()->getPosition_B();
        if (my_engine::vec2::check_AABB(
                pos_bullet_A, pos_bullet_B, pos_enemy_A, pos_enemy_B))
        {
            // TODO
            enemy.operator*()->setHealth(bullet->getDamage());
            // (*enemy)->setHealth(bullet->getDamage());

#ifdef DEBUG_LEVEL
            std::cout << "hit" << std::endl;
            std::cout << "health enemy:\t" << enemy.operator*()->getHealth()
                      << std::endl;
#endif
            // std::cout << "position bullet A\t" << pos_bullet_A << std::endl;
            // std::cout << "position bullet B\t" << pos_bullet_B << std::endl;
            // std::cout << "position enemy A\t" << pos_enemy_A << std::endl;
            // std::cout << "position enemy B\t" << pos_enemy_B << std::endl;

            return true;
        }
        else
        {
            enemy++;
        }
    }
    return false;
}
} // namespace guns