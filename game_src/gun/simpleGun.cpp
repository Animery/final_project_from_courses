#include "simpleGun.hpp"

namespace guns
{

GunSimple::GunSimple()
    : readyGun(true)
{
    timer_to_shoot.setCallback([&]() { readyGun = true; });
    timer_to_clip.setCallback([&]() {
        currentClip = maxClip;
        readyGun    = true;
    });
    std::cout << "size Bullet" << sizeof(Bullet) << std::endl;
    std::cout << "+++ ctor GunSimple" << std::endl;
}

GunSimple::~GunSimple()
{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        delete it.operator*();
        it = bullets.erase(it);
    }
    std::cout << "--- destor GunSimple" << std::endl;
}

void GunSimple::shoot(my_engine::vec2& temp_position, float temp_direction)
{
    if (readyGun)
    {
        bullets.push_back(new Bullet(
            temp_position, temp_direction, speed_bullet, damage_bullet));
        
        --currentClip;
        readyGun = false;

        if (currentClip <= 0)
        {
            timer_to_clip.start(speed_reload);
        }
        else
        {
            timer_to_shoot.start(speed_shoot);
        }
    }
}

std::string_view GunSimple::getNameGun()
{
    return name.data();
}

uint16_t GunSimple::getMaxClip() const
{
    return maxClip;
}

uint16_t GunSimple::getCurrentClip() const
{
    return currentClip;
}

#if defined(TEST_VECTOR)
void GunSimple::update_gun(float delta, std::vector<Enemy*>& enemy_list)
#else
void GunSimple::update_gun(float delta, std::list<Enemy*>& enemy_list)
#endif // TEST_VECTOR
{
    update_bullets(delta, enemy_list);

    timer_to_shoot.update_timer(delta);
    timer_to_clip.update_timer(delta);
}

unsigned int GunSimple::count_bullets()
{
    return bullets.size();
}

#if defined(TEST_VECTOR)
void GunSimple::update_bullets(float delta, std::vector<Enemy*>& enemy_list)
#else
void GunSimple::update_bullets(float delta, std::list<Enemy*>& enemy_list)
#endif // TEST_VECTOR

{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        it.operator*()->update_bullet(delta);

        if (check_collision(it.operator*(), enemy_list))
        {
            delete it.operator*();
            it = bullets.erase(it);
        }
        else
        {
            if (it.operator*()->getPosition().x > (1 / gameConst::size) ||
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
}

#if defined(TEST_VECTOR)
std::vector<Bullet*>* GunSimple::getList_bullets()
#else
std::list<Bullet*>* GunSimple::getList_bullets()
#endif // TEST_VECTOR
{
    return &bullets;
}

#if defined(TEST_VECTOR)
bool GunSimple::check_collision(Bullet* bullet, std::vector<Enemy*>& enemy_list)
#else
bool GunSimple::check_collision(Bullet* bullet, std::list<Enemy*>& enemy_list)
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
            enemy.operator*()->setHealth(bullet->getDamage());

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