# Projeto de jogo3D

O projeto em questão consiste em um jogo 3D em primeira pessoa de sobrevivência de hordas desenvolvido em c++ e opengl, utilizando fontes de iluminação (difusa, especular e ambiente), câmera para navegação implementada usando projeção perspectiva, carregamento de objetos utilitários do glut, e remoção de superfícies ocultas.

## Dependências:
- `g++`  
- `OpenGL` 
- `freeGLUT`

## Como executar:

No Linux:
  Verifique se as bibliotecas estão instaladas.
  Execute no terminal:
    
```bash
    sudo apt update
    sudo apt install freeglut3-dev
```

  Na pasta raiz do projeto, exeute via terminal o seguinte comando para compilar o projeto e gerar seu executável:

```bash
    g++ src/*.cpp -I . -o game -lGL -lGLU -lglut
```
  Para executar o projeto, estando na raiz do projeto basta executar o comando abaixo:

```bash
    ./game
```
No Windows:
   Na pasta raiz do projeto, exeute via terminal o seguinte comando para compilar o projeto e gerar seu executável:

```bash
    g++ src/*.cpp -I . -o game.exe -lfreeglut -lopengl32 -lglu32
```

   Para executar o projeto, estando na raiz do projeto basta executar o comando abaixo:

```bash
    game
```


## Controles

| Tecla   | Função         |
|---------|----------------|
| `w`     | mover-se pra frente     |
| `a`     | mover-se pra trás     |
| `s`     | mover-se pra esquerda  |
| `d`     | mover-se pra direita  |
| `movimento do mouse` | controlar câmera |
| `clique esquerdo do mouse` | atacar |
| `ESC` | sair do jogo |

---
