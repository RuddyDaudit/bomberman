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
#include "carte1.h"




void change_mode(){
	/* fonction qui change le mode du terminal
	 * canonique <----> non canonique */
	static struct termios *save = NULL;
	if (save == NULL) {
		// vers le mode non canonique
		struct termios *non_can;
		non_can = malloc(sizeof(struct termios));
		save = malloc(sizeof(struct termios));
		tcgetattr(0, non_can);
		tcgetattr(0, save);
		cfmakeraw(non_can);
		tcsetattr(1, 0, non_can);
		free(non_can);
	} else {
		// vers le mode canonique
		tcsetattr(1, 0, save);
		free(save);
		save = NULL;
	}
}

char choix_mod(){
  system("clear");
  char n;
  write(1,"Choissisez un mod, n pour normal, d pour difficile, t pour quitter",71);
  read(0,&n,1);
  if(n=='t'){
    change_mode();
     exit(0);
   }
  return n;
}

char* choix_niveaux(){
  char* niveaux = malloc(2*sizeof(char));
  niveaux[0] = choix_mod();
  system("clear");
  if(niveaux[0] == 'd'){
    write(1,"Vous avez choisi difficile, choissisez un niveau, tapez a ou b, t pour quitter",80);
    read(0,&niveaux[1],1);
  }else{
  write(1,"Vous avez choisi normal, choissisez un niveau, tapez a ou b, t pour quitter",76);
  read(0,&niveaux[1],1);
}
  if(niveaux[1]=='t'){
    change_mode();
     exit(0);
  }

  return niveaux;
}

void debut(){
  char* niveaux = choix_niveaux();
  lire(niveaux);

}

void game_over(Carte* carte){
  system("clear");
    if(carte->joueurs[0].vie == 0){
        if(carte->joueurs[0].vie == 0 && carte->joueurs[1].vie == 0){
          write(1,"Egalité ",8);
        }else{
          write(1,"Le joueur 1 a perdu car il n'a plus de vie Bravo au joueur 2 ! ",60);
        }
  } else{
    write(1,"Le joueur 2 a perdu car il n'a plus de vie Bravo au joueur 1 !",60);
  }
  sleep(3);
  debut();
}

void mouvement(int x, int y,int n, Carte* carte){
        carte->joueurs[n].posx += x;
        carte->joueurs[n].posy += y;
        int vit = carte->joueurs[n].vitesse;

          usleep(200000/vit);


}



void active_bon(Carte* carte,int x, int y, int n){
  
 if((carte->tab_bon[x][y].bonus) =='*'){
    carte->joueurs[n].vitesse =4;
  }else if((carte->tab_bon[x][y].bonus) =='+'){
    carte->bombes[n].bonus_r = 2;

  }else if((carte->tab_bon[x][y].bonus) =='@'){
    carte->bombes[n+2].delais =0;
    carte->bombes[n+2].est_active = 0;
  }

  carte->tab_bon[x][y].bonus = ' ';


}




void temps_bombe(Carte* carte, int n1){

  int x = carte->bombes[n1].x;
  int y = carte->bombes[n1].y;
  int rayon = carte->bombes[n1].rayon;
  int h;
  int a =1;
  int b =1;
  int c=1;
  int d=1;
  int e=1;
  int f=1;
  for(h=1; h<=rayon;h++){

    if(x+h < carte->x && carte->tab_mur[x+h][y].pt_de_vie!=0 && carte->tab_mur[x+h][y].pt_de_vie && a!=0){
      carte->tab_mur[x+h][y].pt_de_vie --;
      if (carte->tab_mur[x+h][y].pt_de_vie<=0 ) {
        carte->tab_mur[x+h][y].pt_de_vie = -80;
      }

    }
    if(x-h >= 0 && carte->tab_mur[x-h][y].pt_de_vie!=0 && carte->tab_mur[x-h][y].pt_de_vie && b!=0){
      carte->tab_mur[x-h][y].pt_de_vie --;
      if (carte->tab_mur[x-h][y].pt_de_vie<=0) {
        carte->tab_mur[x-h][y].pt_de_vie = -80;
      }

    }
    if(y+h < carte->y && carte->tab_mur[x][y+h].pt_de_vie!=0 && carte->tab_mur[x][y+h].pt_de_vie && c!=0){
      carte->tab_mur[x][y+h].pt_de_vie --;
      if (carte->tab_mur[x][y+h].pt_de_vie <=0) {
        carte->tab_mur[x][y+h].pt_de_vie = -80;
      }

    }
    if(y-h >= 0 && carte->tab_mur[x][y-h].pt_de_vie!=0 && carte->tab_mur[x][y-h].pt_de_vie && d!=0){
      carte->tab_mur[x][y-h].pt_de_vie --;
      if (carte->tab_mur[x][y-h].pt_de_vie <= 0) {
        carte->tab_mur[x][y-h].pt_de_vie = -80;
      }

    }

    if(x+h >= carte->x || carte->tab_mur[x+h][y].pt_de_vie==0){
      a=0;
    }
    if (x-h <= 0 || carte->tab_mur[x-h][y].pt_de_vie==0) {
      b=0;
    }
    if (y+h >= carte->y || carte->tab_mur[x][y+h].pt_de_vie==0) {
      c=0;
    }
    if (y-h <= 0 || carte->tab_mur[x][y-h].pt_de_vie==0) {
      d=0;
    }


    if(((carte->joueurs[0].posx == x+h && carte->joueurs[0].posy == y) ||
    (carte->joueurs[0].posx == x-h && carte->joueurs[0].posy == y) ||
    (carte->joueurs[0].posx == x && carte->joueurs[0].posy == y+h) ||
    (carte->joueurs[0].posx == x && carte->joueurs[0].posy == y-h) ||
    (carte->joueurs[0].posx == x && carte->joueurs[0].posy == y)) && e!=0){
      carte->joueurs[0].vie --;
      e=0;
    }
    if(((carte->joueurs[1].posx == x+h && carte->joueurs[1].posy == y) ||
    (carte->joueurs[1].posx == x-h && carte->joueurs[1].posy == y) ||
    (carte->joueurs[1].posx == x && carte->joueurs[1].posy == y+h) ||
    (carte->joueurs[1].posx == x && carte->joueurs[1].posy == y-h)||
    (carte->joueurs[1].posx == x && carte->joueurs[1].posy == y)) && f!=0){
      carte->joueurs[1].vie --;
      f=0;
    }
  }

}


void delais_bombe(Carte * carte, int n){
if(carte->bombes[n].delais ==1){
    temps_bombe(carte, n);
    carte->bombes[n].delais = 0;
  }else   if(carte->bombes[n].delais >0){
      carte->bombes[n].delais --;
    }
}


void affiche_carte(Carte* carte);

void action_perso(Carte* carte){


          //si tous va bien ça devrait faire le lien entre les perso
          // et les touches du clavier
  struct pollfd fds[1];
          /*On définit la structure pollfd pour surveiller l'entrée standard à intervalles de temps réguliers*/


          /*La valeur de retour de poll*/
  int ret;
  int t = 1;

          /*On associe fds[0] à l'entrée standard (0)*/
  fds[0].fd=0;
          /*L'évènement attendu est POLLIN*/
  fds[0].events=POLLIN;

          /*On initialise un pointeur pour enregistrer les touches de frappes*/
  char *mov=NULL;
  mov=malloc(sizeof(char));

  while(t){ //&& (carte->joueurs[0].vie>0 || carte->joueurs[1].vie>0)){
    ret=poll(fds,1,5000);
            /*Si il y a un retour positif*/
      if(ret>0){
              /*Si l'évènement detécté est POLLIN*/
        if(fds[0].revents&POLLIN){

                /*1)On lit une touche*/

        //  read(fds[0].fd,mov,3);
          read(fds[0].fd,mov,3);



                    //joueur 1
  int xr = carte->joueurs[0].posx;
  int yr = carte->joueurs[0].posy;
  int xs = carte->joueurs[1].posx;
  int ys = carte->joueurs[1].posy;




  if(strcmp(mov,"d")==0 && (carte->tab_mur[xr][yr+1].pt_de_vie)<0){
    if(xr != xs || yr+1!= ys)
      mouvement(0,1,0,carte);

  }else if(strcmp(mov,"z")==0 && (carte->tab_mur[xr-1][yr].pt_de_vie)<0){
    if(xr-1 != xs || yr!= ys)
      mouvement(-1,0,0,carte);
  }else if(strcmp(mov,"s")==0 && (carte->tab_mur[xr+1][yr].pt_de_vie)<0){
    if(xr+1 != xs || yr!= ys)
      mouvement(1,0,0,carte);
  }else if(strcmp(mov,"q")==0 && (carte->tab_mur[xr][yr-1].pt_de_vie)<0){
    if(xr != xs || yr-1!= ys)
      mouvement(0,-1,0,carte);
  }


  if(carte->tab_bon[xr][yr].bonus != ' '){
      active_bon(carte,xr,yr,0);
      //carte->tab_bon[xr][yr].bonus = ' ';
  }
  if(strcmp(mov,"f")==0){
    if((carte->bombes[0].est_active)==0 && (carte->bombes[0].delais)==0){
      carte->bombes[0].delais = 15;
      carte->bombes[0].x = xr;
      carte->bombes[0].y = yr;
      carte->bombes[0].rayon = 2+(carte->bombes[0].bonus_r);

    }

    if((carte->bombes[2].est_active) == 0 && (carte->bombes[2].delais)==0){
      carte->bombes[2].delais = 15;
      carte->bombes[2].x = xr;
      carte->bombes[2].y = yr;
      carte->bombes[2].rayon = 2+(carte->bombes[0].bonus_r);
    }
  }

  if((carte->bombes[0].delais) != 0){
    delais_bombe(carte,0);
  }
  if((carte->bombes[2].delais) != 0){
    delais_bombe(carte,2);
  }







     //joueur 2


     if (mov[0] == '\033' && mov[1] == '['){
      // read(fds[0].fd,mov,3);
       if(mov[2]=='A' && (carte->tab_mur[xs-1][ys].pt_de_vie)<0){
         if(xs-1 != xr || yr!= ys)
           mouvement(-1,0,1,carte); // haut
     }else if(mov[2]=='D' && (carte->tab_mur[xs][ys-1].pt_de_vie)<0){
       if(xr != xs || ys-1!= yr)
         mouvement(0,-1,1,carte); //gauche
     }else if(mov[2]=='C' && (carte->tab_mur[xs][ys+1].pt_de_vie)<0){
       if(xr != xs || ys+1!= yr)
         mouvement(0,1,1,carte); //droite
     }else if(mov[2]=='B' && (carte->tab_mur[xs+1][ys].pt_de_vie)<0){
       if(xs+1 != xr || yr!= ys)
         mouvement(1,0,1,carte); //bas

     }

   }


     if(carte->tab_bon[xs][ys].bonus != ' '){
       active_bon(carte,xs,ys,1);
     }


     if(strcmp(mov,"m")==0){
       if((carte->bombes[1].est_active)==0 && (carte->bombes[1].delais)==0){
         carte->bombes[1].delais = 15;
         carte->bombes[1].x = xs;
         carte->bombes[1].y = ys;
         carte->bombes[1].rayon = 2+(carte->bombes[1].bonus_r);

       }
       carte->bombes[3].delais =0;
       if((carte->bombes[3].est_active) == 0 && (carte->bombes[3].delais)==0){
         carte->bombes[3].delais = 15;
         carte->bombes[3].x = xs;
         carte->bombes[3].y = ys;
         carte->bombes[3].rayon = 2+(carte->bombes[1].bonus_r);
       }


     }

     if((carte->bombes[1].delais) != 0){
       delais_bombe(carte,1);
     }
     if((carte->bombes[3].delais) != 0){
       delais_bombe(carte,3);
     }

     if(strcmp(mov,"t")==0){ //normalement se fait après le delais passé pas avec une touche
       change_mode();
       exit(0);
     }




        }
      }
      t = 0;


}



  affiche_carte(carte);
}

int col(){
	struct winsize fen;
	ioctl(0, TIOCGWINSZ, &fen);
	return fen.ws_col;
}

void bombe_affiche(){
  char c = 64;
  write(1,"\033[31m",sizeof("\033[31m"));
  write(1,&c,1);
  write(1,"\e[1;37m",sizeof("\e[1;37m"));
}

void affiche_carte(Carte* carte){
  system("clear");
  int i,j,k;
  int xr =carte->joueurs[0].posx;
  int yr = carte->joueurs[0].posy;
  int xs =carte->joueurs[1].posx;
  int ys = carte->joueurs[1].posy;

  for(i=0;i<10;i++){
    for(j=0;j<44;j++){

      if(j== yr && i ==xr){
        write(1,&(carte->joueurs[0].forme),1);
      }else if(j == ys && i == xs){
        write(1,&(carte->joueurs[1].forme),1);
      }else if(carte->bombes[0].delais !=0 && i==carte->bombes[0].x && j==carte->bombes[0].y){
        bombe_affiche();
      }else if(carte->bombes[1].delais !=0 && i==carte->bombes[1].x && j==carte->bombes[1].y){
        bombe_affiche();
      }else if(carte->bombes[2].delais !=0 && i==carte->bombes[2].x && j==carte->bombes[2].y){
        bombe_affiche();
      }else if(carte->bombes[3].delais !=0 && i==carte->bombes[3].x && j==carte->bombes[3].y){
        bombe_affiche();
      }else if((carte->tab_mur[i][j].pt_de_vie) <0){
        if(carte->tab_mur[i][j].pt_de_vie == -80){
          write(1,"X",1);
          carte->tab_mur[i][j].pt_de_vie --;
        }else if((carte->tab_bon[i][j].bonus)== ' '){
          write(1," ",1);
        }else{
          write(1,&(carte->tab_bon[i][j].bonus),1);
        }
      }else if(carte->tab_mur[i][j].pt_de_vie>-1 && carte->tab_mur[i][j].pt_de_vie <10){
        if (carte->tab_mur[i][j].pt_de_vie==0) {
          char n = carte->tab_mur[i][j].pt_de_vie +48;
          write(1,&n,1);
        }else{
        char n2 = carte->tab_mur[i][j].pt_de_vie +48;
        write(1,&n2,1);
      }
    }

    }

    int n = col() -44;
    for(k=0;k<n;k++){
      //printf(" ");
      write(1, " ",1);
    }
  }

  char n2 = (carte->joueurs[0].vie +48);
  write(1,"Le joueur 1 a ",15);
  write(1,&n2, 1);
  write(1," vies.",6);
  char n3 = (carte->joueurs[1].vie +48);
  write(1,"      Le joueur 2 a ",20);
  write(1,&n3, 1);
  write(1," vies.",6);

  if(carte->joueurs[0].vie == 0 || carte->joueurs[1].vie == 0){
    game_over(carte);
  }
  action_perso(carte);
}

Carte* creer_carte(char buf[]){
  int i,j,k;
  while(buf[k]!='\n'){
    k++;
  }
  k++;


  Carte* carte = malloc(sizeof(Carte));
  carte->x = 10;
  carte->y = 44;
  carte->joueurs[0].vie = 2;
  carte->joueurs[0].forme = 'R';
  carte->joueurs[0].posx=1;
  carte->joueurs[0].posy=1;
  carte->joueurs[1].vie = 2;
  carte->joueurs[1].forme = 'S';
  carte->joueurs[1].posx=8;
  carte->joueurs[1].posy=42;
  carte->joueurs[0].vitesse=1;
  carte->joueurs[1].vitesse=1;
  carte->bombes[0].delais = 0;
  carte->bombes[1].delais = 0;
  carte->bombes[2].delais = 0;
  carte->bombes[3].delais = 0;
  carte->bombes[0].est_active = 0;
  carte->bombes[1].est_active = 0;
  carte->bombes[2].est_active = 1;
  carte->bombes[3].est_active = 1;
  carte->bombes[0].bonus_r = 1;
  carte->bombes[1].bonus_r = 1;


  for(i=0;i<10;i++){
    for(j=0;j<45;j++){
      if(buf[k]== ' '){
        carte->tab_mur[i][j].pt_de_vie = -1;
      }else if(buf[k]=='\n'){

      }else{
        carte->tab_mur[i][j].pt_de_vie = buf[k] -48;
      }
      k++;
    }
  }

  for(i=0;i<10;i++){ //les bonus
    for(j=0;j<45;j++){
      if(buf[k]== ' '){
        carte->tab_bon[i][j].bonus = ' ';
      }else if(buf[k]=='\n'){

      }else{
        carte->tab_bon[i][j].bonus = buf[k];
      }
      k++;
    }
  }

  affiche_carte(carte);

  return carte;
}

void lire(char* n){
  system("clear");

  int fd, nread;
  char buf[1024];
  if(n[0]=='d'){
    if(n[1]=='a'){
        fd = open ("difficile/niveaux/1", O_RDONLY);
    }else{
      fd = open ("difficile/niveaux/2", O_RDONLY);
    }

  }else{
    if(n[1]=='a'){
       fd = open ("normal/niveaux/1", O_RDONLY);
     }else{ //if(n[1]=='b'){
       fd = open ("normal/niveaux/2", O_RDONLY);
     }
  }


  if (fd == -1) {
  //  perror (filename);
    return;
  }
  while ((nread = read (fd, buf, sizeof (buf))) > 0){
  }
  write(1,"Les deplacements se font avec les touches ZQSD et les flèches. Pour poser une bombe il y a respectivement la touche f et la touche m. Vous pouvez quitter quand vous voulez en appuyant sur t", 190);
  sleep(5);
  creer_carte(buf);
  close(fd);

}






int main (int argc, char *argv[]){
    change_mode();
    debut();


    return 0;
}
