#include "miniGun.hpp"

namespace guns
{

miniGun::miniGun(Animate::Texture*     temp_tex_bul,
                 my_engine::RenderObj* temp_bul_obj)
{
    readyGun       = true;
    texture_bullet = temp_tex_bul;
    bullet_obj     = temp_bul_obj;

    timer_to_shoot.setCallback([this]() {
        if (speed_shoot > 20)
        {
            speed_shoot -= 10;
        }
        readyGun = true;
    });
    timer_to_clip.setCallback([this]() {
        speed_shoot = 200;
        currentClip = maxClip;
        readyGun    = true;
    });
    std::cout << "size Bullet" << sizeof(Bullet) << std::endl;
    std::cout << "+++ ctor miniGun" << std::endl;
}

miniGun::~miniGun()
{
    for (auto it = bullets.begin(); it != bullets.end();)
    {
        // delete it.operator*();
        it = bullets.erase(it);
    }
    std::cout << "--- destor miniGun" << std::endl;
}

void miniGun::shoot(my_engine::vec2& temp_position, float temp_direction)
{
    if (readyGun)
    {
        // bullets.push_back(new Bullet(
        //     temp_position, temp_direction, speed_bullet, damage_bullet));
        bullets.push_back(std::make_unique<Bullet>(
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

std::string_view miniGun::getNameGun()
{
    return name.data();
}

uint16_t miniGun::getMaxClip() const
{
    return maxClip;
}

uint16_t miniGun::getCurrentClip() const
{
    return currentClip;
}

#if defined(TEST_VECTOR)
void miniGun::update_gun(float delta, std::vector<Enemy*>& enemy_list)
#else
void miniGun::update_gun(float                                       delta,
                         std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list)
#endif // TEST_VECTOR
{
    update_bullets(delta, enemy_list);

    timer_to_shoot.update_timer(delta);
    timer_to_clip.update_timer(delta);
}

unsigned int miniGun::count_bullets()
{
    return bullets.size();
}

void miniGun::render_bullets()
{
    for (auto&& bullet : bullets)
    {
        my_engine::render(*bullet_obj, *texture_bullet, bullet->getMatrix());
    }
}

#if defined(TEST_VECTOR)
void miniGun::update_bullets(float delta, std::vector<Enemy*>& enemy_list)
#else
void miniGun::update_bullets(
    float delta, std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list)
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
// std::vector<Bullet*>* miniGun::getList_bullets()
// #else
// std::deque<Bullet*>* miniGun::getList_bullets()
// #endif // TEST_VECTOR
// {
//     return &bullets;
// }

#if defined(TEST_VECTOR)
bool miniGun::check_collision(Bullet* bullet, std::vector<Enemy*>& enemy_list)
#else
bool miniGun::check_collision(
    Bullet* bullet, std::deque<std::unique_ptr<enemy::iEnemy>>& enemy_list)
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
            (*enemy)->setHealth(bullet->getDamage());

#ifdef DEBUG_LEVEL
            std::cout << "hit" << std::endl;
            std::cout << "health enemy:\t" << (*enemy)->getHealth()
                      << std::endl;
#endif
            if ((*enemy)->getHealth() < 0)
            {
                enemy_list.erase(enemy);
            }
            return true;
        }
        else
        {
            enemy++;
        }
    }
    return false;
}

bool miniGun::out_screen(const Bullet* bullet)
{
    return bullet->getPosition().x > (1 / gameConst::size) ||
           bullet->getPosition().x < -(1 / gameConst::size) ||
           bullet->getPosition().y >
               (1 / gameConst::aspect) / gameConst::size ||
           bullet->getPosition().y < -(1 / gameConst::aspect) / gameConst::size;
}
} // namespace guns