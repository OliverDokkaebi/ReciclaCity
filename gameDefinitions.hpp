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
