#include "GunSimple.hpp"

GunSimple::GunSimple()
    : readyGun(true)
{
    timer_to_shoot.setCallback([&]() { readyGun = true; });
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
        readyGun = false;
        timer_to_shoot.start(speed_shoot);
    }
}

void GunSimple::update_gun(float delta, std::list<Enemy*>& enemy_list)
{
    update_bullets(delta, enemy_list);

    timer_to_shoot.update(delta);
}

unsigned int GunSimple::count_bullets()
{
    return bullets.size();
}

void GunSimple::update_bullets(float delta, std::list<Enemy*>& enemy_list)
{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        it.operator*()->update(delta);

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

std::list<Bullet*>* GunSimple::getList_bullets()
{
    return &bullets;
}

bool GunSimple::check_collision(Bullet* bullet, std::list<Enemy*>& enemy_list)
{
    my_engine::vec2 pos_bullet_A = bullet->getPosition_A();
    my_engine::vec2 pos_bullet_B = bullet->getPosition_B();

    for (auto enemy = enemy_list.begin(); enemy != enemy_list.end();)
    {
        my_engine::vec2 pos_enemy_A = enemy.operator*()->getPosition_A();
        my_engine::vec2 pos_enemy_B = enemy.operator*()->getPosition_B();
        if (pos_bullet_A.x < pos_enemy_B.x && pos_bullet_A.y < pos_enemy_B.y &&
            pos_bullet_B.x > pos_enemy_A.x && pos_bullet_B.y > pos_enemy_A.y)
        {
            enemy.operator*()->setHealth(bullet->getDamage());    
            std::cout << "hit" << std::endl;
            std::cout << "health enemy:\t" << enemy.operator*()->getHealth() << std::endl;
            // std::cout << "position bullet A\t" << pos_bullet_A << std::endl;
            // std::cout << "position bullet B\t" << pos_bullet_B << std::endl;
            // std::cout << "position enemy A\t" << pos_enemy_A << std::endl;
            // std::cout << "position enemy B\t" << pos_enemy_B << std::endl;


            // delete enemy.operator*();
            // enemy = enemy_list.erase(enemy);
            return true;
        }
        else
        {
            enemy++;
        }
    }
    return false;
}