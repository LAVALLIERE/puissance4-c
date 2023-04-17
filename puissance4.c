/* #include <termios.h>
#include <unistd.h>
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum _boolean { FALSE, TRUE } boolean;

#define LIGNE 6
#define COLONNE 7

#define PION_ROUGE "\033[31;1m@\033[0m"
#define PION_JAUNE "\033[33;1m@\033[0m"

typedef enum {
  ROUGE,
  JAUNE,
  VIDE
} Case;

typedef enum {
  FACILE,
  NORMAL,
  DIFFICILE
} Niv;

typedef struct {
  char nom[50];
  int nb_joue[DIFFICILE+1];
  int win[DIFFICILE+1];
  int niveau;
} Joueur;

/*nn
void mode_raw(int activer)
{
   static struct termios cooked;
   static int raw_actif = 0;

   if (raw_actif == activer)
   {
      return;
   }

   if (activer)
   {
      struct termios raw;

      tcgetattr(STDIN_FILENO, &cooked);

      raw = cooked;
      cfmakeraw(&raw);
      tcsetattr(STDIN_FILENO, TCSANOW, &raw);

   }
   else
   {
      tcsetattr(STDIN_FILENO, TCSANOW, &cooked);
   }

   raw_actif = activer;
}
*/

void commencement(Case tab[LIGNE][COLONNE]){
  int i, j;
  for (i = 0; i < LIGNE; i++) {
    for (j = 0; j < COLONNE; j++) {
      tab[i][j] = VIDE;
    }
  }
}

void viderTamponClavier()
   {
   int c = 0;
   while ((c = getchar()) != '\n' && c != EOF); /* mange le tampon */
   }

Joueur nouveauJoueur(void)
{
  int niveau = 0;
   char nomJoueur[30];
   Niv niveauJoueur;
   Joueur joueur;


   printf("\033[37;40mCreation d'un joueur\033[0m\n");

   printf("Nom du joueur:");
   scanf("%s",nomJoueur);

   viderTamponClavier();
  
   // F = 70 M = 77 D = 68
   while ( niveau != 70 && niveau != 77 && niveau != 68 )
   {
      printf("Niveau du joueur: (F,M,D)");
      niveau = getchar();
viderTamponClavier();
   }



   switch ( niveau )
   { case 70:
        joueur.niveau = FACILE;
     break;
     case 77:
        joueur.niveau = NORMAL;
     break;
     case 68:
        joueur.niveau = DIFFICILE;
     break;
   }
   strcpy( joueur.nom, nomJoueur);


   joueur.nb_joue[FACILE] = 0;
   joueur.win[FACILE] = 0;

   joueur.nb_joue[NORMAL] = 0;
   joueur.win[NORMAL] = 0;

   joueur.nb_joue[DIFFICILE] = 0;
   joueur.win[DIFFICILE] = 0;

   return joueur;
}

boolean estPartieTerminee (Case tab[LIGNE][COLONNE], int colonne, int ligne ){
  int i, j;
   // Couleur du pion joue
   Case couleur;
   boolean ligneEst, ligneOuest, ligneSud, ligneNordEst, ligneNordOuest, ligneSudEst, ligneSudOuest;
   boolean gagne = FALSE;

   couleur = tab[ligne][colonne];
  
  // Verifier s'il y a une partieTermineerligne horizontale
  ligneEst = TRUE;
  ligneOuest = TRUE;
  ligneSud = TRUE;
  ligneNordEst = TRUE;
  ligneNordOuest= TRUE;
  ligneSudEst = TRUE;
  ligneSudOuest = TRUE;
 i=1;
   while ( i < 4 )
   {
      if ( ligne - i >= 0 ) {
        if ( tab[ligne-i][colonne] != couleur )
          ligneSud = FALSE;
      } else {
         ligneSud= FALSE;
      }
      if ( colonne + i < COLONNE && ligne+i < LIGNE ) {
        if ( tab[ligne+i][colonne+i] != couleur )
          ligneNordEst= FALSE;
      } else {
         ligneNordEst= FALSE;
      }
      if ( colonne - i >= 0 && ligne-i >= 0 ) {
        if ( tab[ligne-i][colonne-i] != couleur )
          ligneSudOuest= FALSE;
      } else {
         ligneSudOuest= FALSE;
      }
      if ( colonne - i >= 0 && ligne+i < LIGNE ) {
        if ( tab[ligne+i][colonne-i] != couleur )
          ligneSudEst= FALSE;
      } else {
         ligneSudEst= FALSE;
      }
      if ( colonne + i < COLONNE && ligne-i >= 0 ) {
        if ( tab[ligne-i][colonne+i] != couleur )
          ligneNordOuest= FALSE;
      } else {
         ligneNordOuest= FALSE;
      }
      if ( colonne - i >= 0 ) {
        if ( tab[ligne][colonne-i] != couleur )
          ligneOuest= FALSE;
      } else {
         ligneOuest= FALSE;
      }
      if ( colonne + i < COLONNE ) {
        if ( tab[ligne][colonne+i] != couleur )
          ligneEst = FALSE;
      } else {
         ligneEst= FALSE;
      }
     i++;
   }
   if ( ligneNordEst == TRUE || ligneSudEst == TRUE || ligneNordOuest == TRUE || ligneSudOuest == TRUE || ligneOuest == TRUE || ligneSud == TRUE ) {
      gagne = TRUE;
   }

/*
  for (i = 0; i < LIGNE; i++) {
    for (j = 0; j < COLONNE; j++) {
    // Y a t'il une ligne vertcale
    //   tab[i][j] = VIDE;
    }
  }
*/
  return gagne;

}

void dessinePlateau(Case tab[LIGNE][COLONNE]){
  int i, j;
  printf("\n");
  i = LIGNE;
  while (i-- > 0) {
    for (j = 0; j < COLONNE; j++) {
      printf("¬¶");
      switch (tab[i][j]) {
      case ROUGE: printf(PION_ROUGE); break;
      case JAUNE: printf(PION_JAUNE); break;
      case VIDE: printf(" ");
      }
    }
    printf("¬¶\n");
    for (j = 0; j < COLONNE; j++) {
      printf("+-");
    }
    printf("+\n");
  }
  for (j = 1; j <= COLONNE; j++) {
    printf(" %d", j);
  }
  printf("\n");
}

int jouerCoup(Case tab[LIGNE][COLONNE], int num_col, Case pion){
  int l = 0, trouve = 0;
//  num_col--;
  while (!trouve && (l < LIGNE)) {
    trouve = (tab[l][num_col] == VIDE);
    if (trouve) {
      tab[l][num_col] = pion;
    } else {
      l++;
    }
  }
  if (trouve == 0)
     return -1;

  return l;
}

void afficheStatistiquesJoueur( Joueur joueur )
{
  char niveauString[20];

  switch (joueur.niveau) {
  case FACILE :
    strcpy( niveauString, "FACILE");break;
  case NORMAL :
    strcpy( niveauString, "NORMAL");break;
  case DIFFICILE :
    strcpy( niveauString, "DIFFICILE");break;
  }

  printf ("\033[37;40m Statisique de %s - Niveau %s : \033[0m \n", joueur.nom, niveauString );
  printf ("Partie faciles Jouées    : %02d | Gagnées : %02d \n", joueur.nb_joue[FACILE], joueur.win[FACILE] );
  printf ("Parie normales Jouées    : %02d | Gagnées : %02d \n", joueur.nb_joue[NORMAL], joueur.win[NORMAL] );
  printf ("Partie difficiles Jouées : %02d | Gagnées : %02d\n", joueur.nb_joue[DIFFICILE], joueur.win[DIFFICILE] );


}

int main(int argc,char**argv) {
  Case grille[LIGNE][COLONNE];
  boolean partieTerminee;
  int nbCoup;
  int niveau;
  int rejouer, quitter = 0;

  Joueur joueur1, joueur2;
  int colonneJoue;
   int hauteurPion;



while ( quitter != 81 )
{

  printf("Entrez les informations du joueur 1 (ROUGE) : \n" );
  joueur1 = nouveauJoueur();
  printf("\n\n" );
  printf("Entrez les informations du joueur 2 (JAUNE) : \n" );
  joueur2 = nouveauJoueur();



  rejouer = 79;

while ( rejouer == 79 ) {
  afficheStatistiquesJoueur( joueur1 );
  afficheStatistiquesJoueur( joueur2 );
  rejouer = FALSE;
  // Initialiser la nouvelle grille
  commencement(grille);


  //Afficher le grille
  dessinePlateau(grille);

partieTerminee = FALSE;
nbCoup = 0;


while ( partieTerminee == FALSE && nbCoup < COLONNE*LIGNE )
{
  nbCoup++;
  // Choix colonne joueur 
  colonneJoue = -1;
  // mode_raw(1);
  hauteurPion = -1;
  while ( hauteurPion == -1 ) {

  while ( colonneJoue < 48 || colonneJoue > 57 )
  {
    if ( nbCoup % 2 )
      printf ("\033[1;43;30m A %s JAUNE de jouer \033[0m \n", joueur1.nom);
    else
     printf("\033[1;41;30m A %s en ROUGE de jouer\033[0m \n", joueur2.nom);

    printf("Tapez une touche entre 1 et 7 :\n");
     colonneJoue = getchar();
     viderTamponClavier();
 
  }
  colonneJoue -= 49;
  // mode_raw(0);


  // Joue le coup
  hauteurPion = jouerCoup(grille,colonneJoue ,( (nbCoup % 2) == 0) ? ROUGE : JAUNE );
  if ( hauteurPion == -1 )
    printf("Coup impossible!!!\n");
  }

  // Affiche la grille
  dessinePlateau(grille);


  // La partie est elle terminee?
  if ( estPartieTerminee(grille,colonneJoue,hauteurPion) == TRUE )
      partieTerminee = TRUE;

}

if ( partieTerminee == FALSE )
   printf("EGALITE");
 else
  if (nbCoup % 2 ) {
     printf( "Les JAUNES ont gagnés\n" );
     joueur1.win[joueur2.niveau]++;

   }
   else {
     printf( "Les ROUGES ont gagnés\n" );
     joueur2.win[joueur1.niveau]++;

   }

     joueur2.nb_joue[joueur1.niveau]++;
     joueur1.nb_joue[joueur2.niveau]++;



// DEmander si les joeurs vuelent rejouer i
// 79 N
// 78 O
 printf("Rejouer (O/N)\n");
 while ( rejouer != 79 && rejouer != 78 )
 {
   rejouer = getchar();
   viderTamponClavier();
 }


 }

 printf("Quitter ou Continuer (Q/C)\n");
 quitter = 0;
 while ( quitter != 67 && quitter != 81 )
 {
   quitter = getchar();
   viderTamponClavier();
 }

 }

  return 0;
}
