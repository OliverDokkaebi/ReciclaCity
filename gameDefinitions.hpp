#pragma once

// --- Inclusão de Bibliotecas Padrão ---
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <string>

// --- Definições de Constantes ---
#define WIDTH 600
#define HEIGHT 900
#define pass 2.0f
#define zonaJogador 50 
#define zonaCarros 80
#define tamMapa 20


// --- Variáveis Globais (Declaradas com 'extern') ---
extern int screenWidth;
extern int screenHeight;
extern int nivel;
extern int qtdLixoDisponivel;
extern bool mensagem;
extern std::string text;
extern time_t tempoInicioMensagem;

class Player;
class Lane;
class Trash;
class Tree;
class Car;

// --- Instâncias de Objetos do Jogo (Declaradas com 'extern') ---
extern Player player;
extern std::deque<Lane> mapLanes;
extern std::deque<Trash> mapTrash;
extern std::deque<Tree> mapTree;
extern std::deque<Car> mapCars;
extern std::deque<Trash> mapDump;

// Controle do timer dos carros
extern bool carTimerActive;

// Dimensões e posições dos botões do menu
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50

// Dimensões e posições dos botões do menu de pausa
#define PAUSE_CONTINUE_BUTTON_Y (screenHeight / 2 + BUTTON_HEIGHT / 2 + 10)
#define PAUSE_RESTART_BUTTON_Y (screenHeight / 2 - BUTTON_HEIGHT / 2)
#define PAUSE_EXIT_BUTTON_Y (screenHeight / 2 - BUTTON_HEIGHT * 1.5 - 10)

// Posições dos botões do menu principal
#define MENU_START_BUTTON_Y (screenHeight / 2 + 25)
#define MENU_INSTRUCTIONS_BUTTON_Y (screenHeight / 2 - 25)
#define MENU_EXIT_BUTTON_Y (screenHeight / 2 - 75)