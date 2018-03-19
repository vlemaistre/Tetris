#include <iostream>
#include "Jeu.hpp"
#include <string>
#include <time.h>
#include <ncurses.h>

using namespace std;

void Jeu::init(){
  b.init();
  jeu=true;
  b.init();
  PieceEnCours;
  PieceStocke;
  cout<<b<<endl;
}

void Jeu::MaJ(){
  //On intègre la pièce en cours bloqué dans le Board b
  for (int i = 0; i < 4; i++) {
    b.setGrille(PieceEnCours.getPosx(i), PieceEnCours.getPosy(i));
  }
  //On vérifie pour toutes les lignes qu'il n'y a pas de ligne formé
  for (int i = 0; i < b.getHauteur(); i++) {
    bool LigneCompletee=true;
    for (int j = 0; j < b.getLargeur(); j++) {
      if (b.getGrille(j,i)==0){
        LigneCompletee=false;
      }
    }
    //Si la ligne en question est complétée :
    if (LigneCompletee){
      //On vide la Ligne en question
      for (int j = 0; j < b.getLargeur(); j++)
        b.setGrille(j,i);
      //Puis on fait tomber tous les autres blocs d'une case :
      for (int k = i+1; k < b.getHauteur(); k++) {
        for (int j = 0; j < b.getLargeur(); j++)
          if (b.getGrille(j,k)==1){
            //On vide la case
            b.setGrille(j,k);
            //On remplit la case du dessous qui est forcément vide
            b.setGrille(j,k-1);
          }
      }
      //On décrémente ensuite i pour que la même ligne soit recheck
      i--;
    }

  }
}
// Envoie l'ordre de bouger une pièce, renvoie true si une pièce est bougé
void Jeu::move(char c){
  if (c=='q')
    PieceEnCours.MoveLeft(b);
  if (c=='d')
    PieceEnCours.MoveRight(b);
  if (c=='s')
    PieceEnCours.MoveDown(b);
  if (c==' ')
    while(!PieceEnCours.getbloque())
      PieceEnCours.MoveDown(b);
}

void Jeu::afficher(Board b, Piece p){
  clear();
  //On affiche le board
  for (int i = 0; i<b.getHauteur(); i++)
    for (int j = 0; j <= b.getLargeur(); j++)
      if (b.getGrille(j,i)==1)
        mvaddch(b.getHauteur()-i-1,j+1,'x');
  //On affiche la bordure
  for (int i=0; i<b.getHauteur();i++){
    mvaddch(i,0,'x');
    mvaddch(i,b.getLargeur()+1,'x');
  }
  for (int i=0; i<b.getLargeur()+2;i++){
    mvaddch(b.getHauteur(),i,'x');
  }
  //On affiche la pièce
  for (int i = 0; i < 4; i++) {
    mvaddch(b.getHauteur()-p.getPosy(i)-1,p.getPosx(i)+1,'x');
  }

}




void Jeu::play(){
  /*Test LigneCompletee
  for (int i = 0; i < b.getLargeur(); i++) {
    if (i!=4)
      b.setGrille(i,0);
  }
  for (int i = 0; i < b.getLargeur(); i++) {
    if (i!=4)
      b.setGrille(i,3);
  }*/
  while (jeu) {
    initscr();
    noecho();
    //cbreak();
    nodelay(stdscr, TRUE);

    clock_t temps=clock();
    while (PieceEnCours.getbloque()==false) {
        if (clock()-temps>(CLOCKS_PER_SEC/6)){
            PieceEnCours.MoveDown(b);
            temps=clock();
          }
        int n=getch();
        if (n!=-1)
          move((char)n);
        afficher(b, PieceEnCours);
        refresh();
        }
      MaJ();
      PieceEnCours=PieceStocke;
    }
    endwin();
}
