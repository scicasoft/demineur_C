#ifndef _UTILS_H
#define	_UTILS_H

#define DEJAVU 1
#define PASVU 0

typedef struct parcelle {
    int mine; /*1 s'il y a une mine, 0 sinon*/
    int status; /*DEJAVU si deja vu, PASVU sinon */
} parcelle;

typedef struct str_champs {
    int nb_mines;
    int nb_erreurs_restant;
    int nb_lignes; /*nombre de lignes*/
    int nb_colonnes; /*nombre de colonnes*/
    parcelle *tab; /*un tableau de nb_lignes * nb_colonnes de parcelles*/
} str_champs;

typedef str_champs *champs;

int indice(champs C, int i, int j);

int getX(champs C, int k);

int getY(champs C, int k);

void genererMine(champs C);

champs init(int nb_lignes, int nb_colonnes, int nb_mines);

int nb_mines_voisins(champs C, int x, int y);

void affichage (champs C);

void affichage_triche(champs C);

int joueur(champs C, int x, int y);

void recupXY(char* chaine, int* x, int* y);

void menu(champs C);

int sauvegarder(char *fichier, champs C);

int code(int nombre1, int nombre2);

champs lire(char *fichier);

int fin_jeu(champs C);

int a_gagne(champs C);

champs menuDefaut();

void commencerJeu(champs C);

#endif	/* _UTILS_H */