#include "GunSimple.hpp"

GunSimple::GunSimple()
    : readyGun(true)
{
    timer_to_shoot.setCallback([&]() { readyGun = true; });
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
        bullets.push_back(
            new Bullet(temp_position, temp_direction, speed_bullet));
        readyGun = false;
        timer_to_shoot.start(speed_shoot);
    }
}

void GunSimple::update_gun(size_t delta)
{
    update_bullets();

    timer_to_shoot.update(delta);
}

unsigned int GunSimple::count_bullets()
{
    return bullets.size();
}

void GunSimple::update_bullets()
{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        it.operator*()->update();
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

std::list<Bullet*>* GunSimple::getList_bullets()
{
    return &bullets;
}