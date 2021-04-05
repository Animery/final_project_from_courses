#include "game_impl.hpp"
#include <ctime>
// #include "Bullet.hpp"
// #include <cmath>

namespace my_game
{

game_impl::game_impl(my_engine::engine* _engine)
{
    engine = _engine;
    srand(time(NULL));
    float temp_pos_x;
    float temp_pos_y;

    for (size_t i = 0; i < 4000; i++)
    {
        temp_pos_x = (rand() % 2000 / 1000.0f) - 1;
        temp_pos_y = (rand() % 2000 / 1000.0f) - 1;
        add_enemy({ temp_pos_x, temp_pos_y });
        // add_enemy({ 0.5, 0.5 });
        // add_enemy({ -0.5, 0.5 });
        // add_enemy({ 0.5, -0.5 });
        // add_enemy({ -0.5, -0.5 });
    }

    // enemy_list.push_back(new Enemy);
    std::cout << "sizeof Player" << sizeof(Player) << std::endl;
    std::cout << "sizeof Enemy" << sizeof(Enemy) << std::endl;
    std::cout << "size enemy_list" << enemy_list.size() << std::endl;
    std::cout << "+++ ctor game_impl" << std::endl;
}

game_impl::~game_impl()
{
    // delete engine;
    for (auto it : enemy_list)
    {
        delete it;
    }

    delete gfx01;
    std::cout << "--- destor game_impl" << std::endl;
}

bool game_impl::getIsRunning()
{
    return isRunning;
}

void game_impl::on_initialize()
{
    engine->initialize("", this);

    // gfx_prog
    const std::string path("shader/");
    const std::string name_shader("corpse");
    gfx01 = my_engine::create_gfx_prog(path, name_shader);
    gfx01->bind_attrib(0, "a_position");
    gfx01->bind_attrib(1, "a_tex_coord");
    gfx01->bind_attrib(2, "a_color");
    gfx01->link();
    // gfx_prog

    // Texture init
    std::string tex_name = "s_texture";
    texture_head         = std::make_unique<Texture>(tex_name);
    {
        Image image = Image::loadFromFile("res/head.png");
        texture_head->setImage(image);
    }

    texture_corpse = std::make_unique<Texture>(tex_name);
    {
        Image image = Image::loadFromFile("res/corpse.png");
        texture_corpse->setImage(image);
    }

    texture_bullet = std::make_unique<Texture>(tex_name);
    {
        Image image = Image::loadFromFile("res/bullet.png");
        texture_bullet->setImage(image);
    }
    // Texture init

    // Render_obj
    tank_obj = my_engine::create_RenderObj();
    tank_obj->setProg(gfx01);
    tank_obj->load_mesh_from_file("res/tank.txt");

    bullet_obj = my_engine::create_RenderObj();
    bullet_obj->setProg(gfx01);
    bullet_obj->load_mesh_from_file("res/bullet.txt");
    // Render_obj

    // Gun
    gun_current = std::make_unique<GunSimple>();
    // Gun

    player    = std::make_unique<Player>();
    isRunning = true;

    // Enemy

    // Enemy
}

void game_impl::on_event(my_engine::event& event)
{
    while (engine->read_event(event))
    {
        // std::cout << event << std::endl;
        switch (event.type)
        {
            case my_engine::event_type::hardware:
            {
                if (std::get<my_engine::hardware_data>(event.info).is_reset)
                {
                    isRunning = false;
                }
                break;
            }
            case my_engine::event_type::input_key:
            {
                const auto& key_data =
                    std::get<my_engine::input_data>(event.info);

                controls[static_cast<unsigned>(key_data.key)] =
                    key_data.is_down;

                if (key_data.is_down)
                {
                    if (key_data.key == my_engine::keys_type::select)
                    {
                        isRunning = false;
                    }

                    else if (key_data.key == my_engine::keys_type::button1)
                    {
                        my_engine::matrix2x3 rot_head =
                            my_engine::matrix2x3::rotation(
                                player->getCurrent_head_direction());
                        std::cout << "rotation head:\n"
                                  << rot_head << std::endl;
                        // s->play(my_engine::SoundBuffer::properties::once);
                    }
                    else if (key_data.key == my_engine::keys_type::button2)
                    {
                        // s->play(my_engine::SoundBuffer::properties::looped);
                    }
                }
                break;
            }
            case my_engine::event_type::mouse:
            {
                player->setMouse_pos(my_engine::vec2{ event.x, event.y });

                break;
            }
            default:
                break;
        }
    }
}

void game_impl::on_update(std::chrono::microseconds frame_delta)
{
    // std::cout << "frame_delta:\t" << frame_delta.count() << std::endl;
    const float delta = frame_delta.count() * 0.001f;

    if (controls[static_cast<unsigned>(my_engine::keys_type::button2)])
    {
        gun_current->shoot(player->getCurrent_tank_pos(),
                           player->getCurrent_head_direction());
    }

    gun_current->update_gun(delta, enemy_list);

    player->update(controls, delta);

    for (auto&& monster : enemy_list)
    {
        monster->update(delta, player->getCurrent_tank_pos());
    }
}

void game_impl::on_render()
{
#if defined(TEST_VECTOR)
    std::vector<Bullet*>* temp_bullets = gun_current->getList_bullets();
#else
    std::list<Bullet*>* temp_bullets = gun_current->getList_bullets();
#endif // TEST_VECTOR
    for (auto&& bullet : *temp_bullets)
    {
        engine->render(*bullet_obj, *texture_bullet, bullet->getMatrix());
    }

    engine->render(*tank_obj, *texture_corpse, player->getMatrix_corpse());
    engine->render(*tank_obj, *texture_head, player->getMatrix_head());

    for (auto enemy = enemy_list.begin(); enemy != enemy_list.end();)
    {
        if (enemy.operator*()->getHealth() <= 0)
        {
            delete enemy.operator*();
            enemy = enemy_list.erase(enemy);
        }
        else
        {
            engine->render(*tank_obj,
                           *texture_corpse,
                           enemy.operator*()->getMatrix_corpse());
            // engine->render(
            //     *tank_obj, *texture_head,
            //     enemy.operator*()->getMatrix_head());
            ++enemy;
        }
    }
}

void game_impl::add_enemy(my_engine::vec2 pos_enemy)
{
    enemy_list.push_back(new Enemy(pos_enemy));
}

} // namespace my_game

static bool already_exist_game = false;

my_engine::game* my_engine::create_game(my_engine::engine* engine)
{
    if (already_exist_game)
    {
        throw std::runtime_error("engine already exist");
    }
    my_engine::game* result = new my_game::game_impl(engine);
    already_exist_game      = true;
    return result;
}

void my_engine::destroy_game(my_engine::game* e)
{
    if (already_exist_game == false)
    {
        throw std::runtime_error("game not created");
    }
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}