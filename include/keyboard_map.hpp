#ifndef KEYBOARD_MAP_HPP
#define KEYBOARD_MAP_HPP

#include <map>
#include <list>
#include <functional>
#include <string>

using namespace std;

/// Classe que gerencia o mapeamento de teclas para funções
class KeyboardMap {

private:
    static KeyboardMap* instance;
    map<unsigned char, list<function<void(int, int, int)>>> keyboard_func_with_input;
    map<unsigned char, list<function<void()>>> keyboard_func_action_only;

    KeyboardMap() = default;
    ~KeyboardMap() = default;

    // Impede cópia/atribuição
    KeyboardMap(const KeyboardMap&) = delete;
    KeyboardMap& operator=(const KeyboardMap&) = delete;
    
public:
    static KeyboardMap* get_instance();

    /**
     * Executa todas as funções associadas à tecla pressionada
     * @param key tecla pressionada
     * @param mousex posição x do mouse no momento em que a tecla é precionada
     * @param mousey posição y do mouse no momento em que a tecla é precionada
     */
    static void keyPressAction(unsigned char key, int mouse_x, int mouse_y);

    /**
     * Adiciona uma função a uma determinada tecla
     * @param keys  teclas que disparam a função (cada char da strings é associado
     * como um tecla que dispara a função)
     * @param func função a ser executada, a função deve receber três parâmetros: a tecla
     * precionada, e as posições x e y do mouse no momento.
     */
    static void add_key_func(string keys, function<void(int, int, int)> func);

    /**
     * Adiciona uma função a uma determinada tecla
     * @param keys  teclas que disparam a função (cada char da strings é associado
     * como um tecla que dispara a função)
     * @param func função a ser executada, a função não deve receber parâmetros.
     */
    static void add_key_func(string keys, function<void()> func);
};

#endif