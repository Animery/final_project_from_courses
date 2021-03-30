#include "GunSimple.hpp"

GunSimple::GunSimple()
{
    std::cout << "+++ ctor GunSimple" << std::endl;
}

GunSimple::~GunSimple()
{
    std::cout << "--- destor GunSimple" << std::endl;
}

void GunSimple::shoot(my_engine::vec2& temp_position, float temp_direction)
{
    // if (bullets.size() != 0)
    // {
    bullets.push_back(new Bullet(temp_position, temp_direction));
    // }
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
        if (it.operator*()->getPosition().x > (1 / size) ||
            it.operator*()->getPosition().x < -(1 / size) ||
            it.operator*()->getPosition().y > (1 / aspect) / size ||
            it.operator*()->getPosition().y < -(1 / aspect) / size)
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

// void GunSimple::render_bullets()
// {
//     if (bullet != nullptr)
//     {
//         engine->render(*bullet_obj, *texture_bullet, bullet->getMatrix());
//     }
// }

std::list<Bullet*>& GunSimple::getList_bullets() 
{
    return bullets;
}