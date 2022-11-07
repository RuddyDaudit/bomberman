

#ifndef CARTE
#define CARTE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <sys/types.h>
#include <unistd.h>
//#include <sys/wait.h>
#include <termios.h>
#include <curses.h>
#include <term.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#define MAX_HAUTEUR 10
#define MAX_LARGEUR 44

typedef struct{
  char forme;
  int vie;
  int posx;/*par rapport la carte, la ou tu veux le placer*/
  int posy;
  int vitesse;
}Player;


 typedef struct{
  int pt_de_vie; //negatif si pas de mur, 0 si indestructible, point de vie du mur sinon
}Mur;
typedef struct{
  int x;
  int y;
  int delais; //temps d'explosion
  int rayon;
  int est_active; //0 oui 1 non
  int bonus_r; // bonus de rayon

}Bombe;
typedef struct{
  char bonus;
}Bonus;

typedef struct{
  int x; // longueur
  int y; // largeur
  Mur tab_mur[10][44];
  Bonus tab_bon[10][44];
  Player joueurs[2]; // tableau contenant les personnages
  Bombe bombes[4];
}Carte;


//permet de passer du mode canonique a non canonique et vice versa
void change_mode();
char choix_mod();
char* choix_niveaux();
void game_over(Carte* carte);
//permet le mouvement des personnages
void mouvement(int x, int y,int n, Carte* carte);
//permet l'activation d'un bonus
void active_bon(Carte* carte,int x, int y, int n);
//gère les bombes
void temps_bombe(Carte* carte, int n1);
//gère le temps avant que la bombe explose
void delais_bombe(Carte * carte, int n);
//gère les entrées au clavier et réagit en fonction de l'entrée
void action_perso(Carte* carte);
//récupère le nombre de colonne du terminal
int col();
void bombe_affiche();
//afficher la carte
void affiche_carte(Carte* carte);
//creer la carte
Carte* creer_carte(char buf[]);
//lis la carte qui se trouve dans mod
void lire(char* n);

#endif
