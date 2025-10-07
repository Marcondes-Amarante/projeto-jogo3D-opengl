#include <stdexcept>
#include "arena.hpp"

Arena::Arena() {
    set_position(0, -1, 0);
    set_scale(200);
    if (!load_model("assets/arena_pokemon.obj"))
        throw std::invalid_argument("Carregamento da arena falhou");
    if(!load_texture("assets/textures/fireRed_loreleiRoom_baseColor.png"))
        throw std::invalid_argument("Carregamento da textura da arena falhou");
}

