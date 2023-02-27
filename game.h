#ifndef GAME_H
#define GAME_H
#include <vector>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>

#define NB_THREADS 2
class game {
public:
  //the grid
  int** grid;
  //constructeur
  game();
  //destructeur
  ~game();
  //methode pour lancer le jeu
  void run();
  //initialiser le grid du jeu
  void init_grid();
  //passage à la generation suivante
  static void next_generation_per_thread(game* g,int begin_x, int end_x,int begin_y, int end_y);
  //passage à la generation suivante
  void  next_generation();
  //affichage du gid
  void display_grid(); // show grid in terminal ( for test purposes )
  //getters
  int get_grid_x();
  int get_grid_y();
private:
  //verrou pour la protection du grid
  std::mutex* verrou;
  int grid_size_x = 10;
  int grid_size_y = 10;
  int cell_number = 100;
  bool isRunning=false;
  int getLivingNieghbors(int x,int y);
};
#endif // GAME_H
