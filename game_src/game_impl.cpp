#include "game_impl.hpp"
#include <cmath>
#include <numbers>

namespace my_game
{

constexpr float size_tank    = 0.3f;
constexpr float size_tank_qw = 1.0f + size_tank / 2;
constexpr float pi           = std::numbers::pi_v<float>;

game_impl::game_impl(my_engine::engine* _engine)
{
    std::cout << "+++ ctor game_impl" << std::endl;
    player    = std::make_shared<Player>();
    engine    = _engine;
    isRunning = true;
}

game_impl::~game_impl()
{
    // delete engine;
    std::cout << "--- destor game_impl" << std::endl;
}

bool game_impl::getIsRunning()
{
    return isRunning;
}

void game_impl::on_initialize() {}

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
                if (key_data.is_down)
                {
                    if (key_data.key == my_engine::keys_type::select)
                    {
                        isRunning = false;
                    }

                    else if (key_data.key == my_engine::keys_type::button1)
                    {
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
                my_engine::vec2 temp = my_engine::vec2{
                    event.x / (1920 / 2) - 1, -(event.y / (1080 / 2) - 1)
                } - player->current_tank_pos;

                std::cout << "coord : " << temp.x << "\t" << temp.y
                          << std::endl;

                float a;
                // if (temp.x == 0)
                //     a = (temp.y > 0) ? pi : 0;
                a = std::atan2(temp.y, temp.x) + pi / 2;
                a = -a;
                std::cout << "current_head_direction : " << a << std::endl;
                player->current_head_direction = a;

                break;
            }
            default:
                break;
        }
    }
}

void game_impl::on_update(std::chrono::milliseconds frame_delta)
{

    if (engine->is_key_down(my_engine::keys_type::left))
    {
        player->current_tank_pos.x -= 0.01f;
        if (player->current_tank_pos.x < -size_tank_qw)
        {
            player->current_tank_pos.x = size_tank_qw;
        }
        player->current_tank_direction = pi / 2.f;
    }
    else if (engine->is_key_down(my_engine::keys_type::right))
    {
        player->current_tank_pos.x += 0.01f;
        if (player->current_tank_pos.x > size_tank_qw)
        {
            player->current_tank_pos.x = -size_tank_qw;
        }
        player->current_tank_direction = -pi / 2.f;
    }
    else if (engine->is_key_down(my_engine::keys_type::up))
    {
        player->current_tank_pos.y += 0.01f;
        if (player->current_tank_pos.y > (size_tank_qw - size_tank))
        {
            player->current_tank_pos.y = -(size_tank_qw - size_tank);
        }
        player->current_tank_direction = pi;
    }
    else if (engine->is_key_down(my_engine::keys_type::down))
    {
        player->current_tank_pos.y -= 0.01f;
        if (player->current_tank_pos.y < -(size_tank_qw - size_tank))
        {
            player->current_tank_pos.y = (size_tank_qw - size_tank);
        }
        player->current_tank_direction = 0.0f;
        // player->current_tank_direction = pi;
    }

    my_engine::matrix2x3 move =
        my_engine::matrix2x3::move(player->current_tank_pos);

    my_engine::matrix2x3 rot =
        my_engine::matrix2x3::rotation(player->current_tank_direction);
    player->matrix_corpse = rot * move * player->aspect;
    my_engine::matrix2x3 rot_head =
        my_engine::matrix2x3::rotation(player->current_head_direction);
    player->matrix_head = rot_head * move * player->aspect;
}

void game_impl::on_render(my_engine::RenderObj& vao, Texture& tex) const
{
    engine->render(vao, tex, player->matrix_corpse);
}

void game_impl::on_render() const
{
    // engine->render(*(player->getCorpse()), *(player->getTexture_corpse()),
    // matrix); engine->render(*(player->getHead()),
    // *(player->getTexture_head()), matrix);
    engine->render(
        *(player->corpse), *(player->texture_corpse), player->matrix_corpse);
    engine->render(
        *(player->head), *(player->texture_head), player->matrix_head);
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