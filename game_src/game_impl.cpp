#include "game_impl.hpp"
// #include "Bullet.hpp"
#include <cmath>

namespace my_game
{

game_impl::game_impl(my_engine::engine* _engine)
{
    engine = _engine;

    std::cout << "+++ ctor game_impl" << std::endl;
}

game_impl::~game_impl()
{
    // delete engine;
    delete gfx01;
    std::cout << "--- destor game_impl" << std::endl;
}

bool game_impl::getIsRunning()
{
    return isRunning;
}

void game_impl::on_initialize()
{
    engine->initialize("");

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
                        gun_current->shoot(player->getCurrent_tank_pos(),
                                           player->getCurrent_head_direction());
                        // if (bullet == nullptr)
                        // {
                        //     bullet =
                        //         new Bullet(player->getCurrent_tank_pos(),
                        //                    player->getCurrent_head_direction());
                        // }

                        // s->play(my_engine::SoundBuffer::properties::looped);
                    }
                }
                break;
            }
            case my_engine::event_type::mouse:
            {
                my_engine::vec2 temp = my_engine::vec2{
                    event.x / (1920 / 2) - 1, -(event.y / (1080 / 2) - 1)
                } - player->getCurrent_tank_pos();

                // std::cout << "coord : " << temp.x << "\t" << temp.y
                //           << std::endl;

                float a;
                // if (temp.x == 0)
                //     a = (temp.y > 0) ? pi : 0;
                a = std::atan2(temp.y, temp.x) + pi / 2;
                a = -a;
                // std::cout << "current_head_direction : " << a << std::endl;
                player->setCurrent_head_direction(a);

                break;
            }
            default:
                break;
        }
    }
}

void game_impl::on_update(std::chrono::milliseconds frame_delta)
{
    player->update(controls);
    gun_current->update_bullets();
    // if (bullet != nullptr)
    // {
    //     bullet->update();
    //     if (bullet->getPosition().x > (1 / size) ||
    //         bullet->getPosition().x < -(1 / size) ||
    //         bullet->getPosition().y > (1 / aspect) / size ||
    //         bullet->getPosition().y < -(1 / aspect) / size)
    //     {
    //         delete bullet;
    //         bullet = nullptr;
    //     }
    // }
}

void game_impl::on_render() const
{

    std::list<Bullet*> temp_bullets = gun_current->getList_bullets();
    for (auto &&bullet : temp_bullets)
    {
        engine->render(*bullet_obj, *texture_bullet, bullet->getMatrix());
    }
    
    
    
    // if (bullet != nullptr)
    // {
    //     engine->render(*bullet_obj, *texture_bullet, bullet->getMatrix());
    // }

    engine->render(*tank_obj, *texture_corpse, player->getMatrix_corpse());
    engine->render(*tank_obj, *texture_head, player->getMatrix_head());
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