#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include "utils.h"

//retourne l'indice de la parcelle en lui donnant les coordonnees de la parcelle

int indice(champs C, int i, int j) {
    if (i == 0)
        return j;
    else
        return i * C->nb_colonnes + j;
}

//retourne le numero de la colonne ou se trouve la parcelle a l'indice k

int getX(champs C, int k) {
    return (k % C->nb_colonnes);
}

//retourne le numero de la ligne ou se trouve la parcelle a l'indice k

int getY(champs C, int k) {
    return (int) (k / C->nb_colonnes);
}

//cette procedure permet de generer les mines

void genererMine(champs C) {
    //selection aléatoire d'un indice
    int indice, nb_mines = C->nb_mines, taille = C->nb_colonnes * C->nb_lignes;
    while (nb_mines > 0) {
        indice = rand() % (taille);
        if (C->tab[indice].mine == 0) {
            C->tab[indice].mine = 1;
            nb_mines--;
        }
    }
}

//cree un champ pour le démineur

champs init(int nb_lignes, int nb_colonnes, int nb_mines) {
    //déclaration des variables
    int i, j, taille = nb_colonnes * nb_lignes;
    champs vChamps = (champs) malloc(sizeof (str_champs));

    //remplissage des attributs du champs
    vChamps->nb_mines = nb_mines;
    vChamps->nb_colonnes = nb_colonnes;
    vChamps->nb_lignes = nb_lignes;
    vChamps->tab = malloc(taille * sizeof (parcelle));

    //initialisation des parcelles
    for (i = 0; i < taille; i++) {
        vChamps->tab[i].status = PASVU;
        vChamps->tab[i].mine = 0;
    }

    //generation des mines
    genererMine(vChamps);

    //on retourne le champs
    return vChamps;
}

//calcul le nombre de mines qui entoure une parcelle

int nb_mines_voisins(champs C, int x, int y) {
    int i, j, nb_mines = 0;

    for (i = x - 1; i <= x + 1; i++)
        for (j = y - 1; j <= y + 1; j++) {

            //on verifie si la parcelle existe
            if (i >= 0 && j >= 0 && i < C->nb_colonnes && j < C->nb_lignes)

                //on verifie si la parcelle est differente de celle qu'on doit trouver le nombre de mines voisins et que si la parcelle a une bombe :D
                if ((indice(C, j, i) != indice(C, y, x)) && (C->tab[indice(C, j, i)].mine == 1))

                    //on incrémente le nombre de mines trouvées
                    nb_mines++;

        }

    //on retourne le nombre de mines trouvées
    return nb_mines;
}

//affichage de l'état du champs C

void affichage(champs C) {
    int i, j, ind, debut_x = 2, debut_y = 1;

    erase();

    //affichage du champs

    for (i = 0; i < C->nb_lignes; i++) {

        //on affiche d'abord les numeros de ligne

        move(debut_y + 3 + i, debut_x + 1);
        printw("%d", i % 10); //affichage des unites pour les numeros de ligne
        move(debut_y + 3 + i, debut_x + 2);
        printw("|");
        if (i % 10 == 0) { //affichage des dizaines pour les numeros de ligne
            move(debut_y + 3 + i, debut_x);
            printw("%d", i / 10);
        }

        for (j = 0; j < C->nb_colonnes; j++) {

            //on affiche les numeros de colonne
            move(debut_y + 1, debut_x + 4 + 2 * j);
            printw("%d", j % 10); //affichage des unites pour les numeros de colonne
            move(debut_y + 2, debut_x + 3 + 2 * j);
            printw("-");
            move(debut_y + 2, debut_x + 4 + 2 * j);
            printw("-");
            if (j % 10 == 0) { //affichage des dizaines pour les numeros de colonne
                move(debut_y, debut_x + 4 + 2 * j);
                printw("%d", j / 10);
            }

            //on recupere l'indice de la parcelle
            ind = indice(C, i, j);

            move(debut_y + 3 + i, debut_x + 4 + 2 * j);
            if (C->tab[ind].status == PASVU) {
                attron(COLOR_PAIR(2));
                addstr("x");
                attroff(COLOR_PAIR(2));
            } else {
                if (C->tab[ind].mine == 1) {
                    attron(COLOR_PAIR(3));
                    addstr("M");
                    attroff(COLOR_PAIR(3));
                } else
                    printw("%d", nb_mines_voisins(C, j, i));
            }
            /*
                        move(debut_y + 3 + i, debut_x + 4 + 2 * j);
                        printw("%d", nb_mines_voisins(C, j, i));
                        move(debut_y + 3 + i, debut_x + 4 + 2 * j);
                        if (C->tab[ind].mine == 1)
                            printw("*");
             */
        }
    }

    move(debut_y + C->nb_lignes + 5, 2);
    printw("Il reste %d parcelle(s) non minee(s) a trouver\n  Tu as encore droit a %d erreur(s)", reste_parcelle_nonMinee(C), C->nb_erreurs_restant);
    move(debut_y + C->nb_lignes + 8, 2);
    printw("Q ou q pour quitter\n  S ou s pour sauvegarder\n  Coordonnees du points => l,c :                               ");
    refresh();
}

void affichage_triche(champs C) {
    int i, j, ind, debut_x = 2, debut_y = 1;

    //on efface l'ecran

    erase();

    //affichage du champs

    for (i = 0; i < C->nb_lignes; i++) {

        //on affiche d'abord les numeros de ligne

        move(debut_y + 3 + i, debut_x + 1);
        printw("%d", i % 10); //affichage des unites pour les numeros de ligne
        move(debut_y + 3 + i, debut_x + 2);
        printw("|");
        if (i % 10 == 0) { //affichage des dizaines pour les numeros de ligne
            move(debut_y + 3 + i, debut_x);
            printw("%d", i / 10);
        }

        for (j = 0; j < C->nb_colonnes; j++) {

            //on affiche les numeros de colonne
            move(debut_y + 1, debut_x + 4 + 2 * j);
            printw("%d", j % 10); //affichage des unites pour les numeros de colonne
            move(debut_y + 2, debut_x + 3 + 2 * j);
            printw("-");
            move(debut_y + 2, debut_x + 4 + 2 * j);
            printw("-");
            if (j % 10 == 0) { //affichage des dizaines pour les numeros de colonne
                move(debut_y, debut_x + 4 + 2 * j);
                printw("%d", j / 10);
            }

            //on recupere l'indice de la parcelle
            ind = indice(C, i, j);

            move(debut_y + 3 + i, debut_x + 4 + 2 * j);

            /*
                        if (C->tab[ind].mine == 1) {
                            attron(COLOR_PAIR(3));
                            addstr("M");
                            attroff(COLOR_PAIR(3));
                        } else
                            printw("%d", nb_mines_voisins(C, j, i));
             */

            move(debut_y + 3 + i, debut_x + 4 + 2 * j);
            printw("%d", nb_mines_voisins(C, j, i));
            move(debut_y + 3 + i, debut_x + 4 + 2 * j);
            if (C->tab[ind].mine == 1)
                printw("*");
        }
    }

    move(debut_y + C->nb_lignes + 5, 2);
    printw("Il reste %d parcelle(s) non minee(s) a trouver\n  Tu as encore droit a %d erreur(s)", reste_parcelle_nonMinee(C), C->nb_erreurs_restant);
    move(debut_y + C->nb_lignes + 8, 2);
    printw("Q ou q pour quitter\n  S ou s pour sauvegarder\n  Coordonnees du points => l,c :                               ");
    refresh();
}

int reste_parcelle_nonMinee(champs C) {
    int i, pasvu = 0, taille = C->nb_colonnes * C->nb_lignes;
    for (i = 0; i < taille; i++) {
        if ((C->tab[i].status == PASVU) && (C->tab[i].mine == 0))
            pasvu++;
    }
    return pasvu;
}

/* sauvegarde du jeu dans un fichier*/

int joueur(champs C, int x, int y) {
    int ind = indice(C, y, x);

    //on vérifie si la parcelle existe

    if (!(x >= 0 && y >= 0 && x < C->nb_colonnes && y < C->nb_lignes))
        //si elle n existe pas on retourne -1
        return -1;
    if (C->tab[ind].status == PASVU) {
        C->tab[ind].status = DEJAVU;
        if (C->tab[ind].mine == 0) {
            if (nb_mines_voisins(C, x, y) == 0) {
                int i, j;

                //si le nombre de mines voisins est nul alors on active les parcelles voisines

                for (i = x - 1; i <= x + 1; i++)
                    for (j = y - 1; j <= y + 1; j++)
                        joueur(C, i, j);
            }
            //on retourne 0 si la parcelle n'etait pas encore vu et qu'elle n'ait pas une mine
            return 0;
        } else
            //on retourne 1 si la parcelle n'etait pas encore vu et qu'elle ait une mine
            return 1;
    } else
        //on retourne 2 si la parcelle est deja vu
        return 2;
}

void recupXY(char* chaine, int* x, int* y) {
    char* l = malloc(1);
    char* c = malloc(1);
    int i = 0, j = 0;
    while (chaine[i] != ',') {
        l[i] = chaine[i];
        i++;
    }
    i++;
    while (chaine[i] != '\0') {
        c[j] = chaine[i];
        j++;
        i++;
    }
    *x = atoi(c);
    *y = atoi(l);
}

void menu(champs C) {
    erase();
    printw("MENU");
    refresh();
}

int sauvegarder(char *fichier, champs C) {
    FILE *sauvegarde;
    int i, taille = C->nb_colonnes * C->nb_lignes;
    if (sauvegarde = fopen(fichier, "w+")) {
        fprintf(sauvegarde, "%d\n", C->nb_lignes);
        fprintf(sauvegarde, "%d\n", C->nb_colonnes);
        fprintf(sauvegarde, "%d\n", C->nb_erreurs_restant);
        for (i = 0; i < taille; i++) {
            int mine = C->tab[i].mine;
            int status = C->tab[i].status;
            fprintf(sauvegarde, "%d\t%d\t", mine, status);
        }
        fclose(sauvegarde);
        if (sauvegarde = fopen("saves", "a")) {
            fprintf(sauvegarde, "%s\n", fichier);
        } else {
            sauvegarde = fopen("saves", "w+");
            fprintf(sauvegarde, "%s\n", fichier);
        }
        fclose(sauvegarde);
        return 1;
    } else
        return 0;
}

/* récupération du fichier sauvegardé*/
champs lire(char* fichier) {
    FILE* liresauve;
    champs C = malloc(sizeof (champs));

    int k = 0, nmine = 0;
    if (liresauve = fopen(fichier, "r")) {
        fscanf(liresauve, "%d\n", &(C->nb_lignes));
        fscanf(liresauve, "%d\n", &(C->nb_colonnes));
        fscanf(liresauve, "%d\n", &(C->nb_erreurs_restant));
        C->tab = malloc(C->nb_colonnes * C->nb_lignes * sizeof (parcelle));
        while (feof(liresauve) == 0) {
            fscanf(liresauve, "%d\t%d\t", &(C->tab[k].mine), &(C->tab[k].status));
            if (C->tab[k].mine == 1) {
                nmine++;
            }
            k++;
        }
        fclose(liresauve);
        C->nb_mines = nmine;
        return C;
    } else {
        printw("Erreur lors de l'ouverture du fichier !!! Veuillez verifier si le fichier existe");
        exit(0);

    }
}

/* Determine si le jeu s'est terminé */
int fin_jeu(champs C) {
    int i;
    for (i = 0; i < C->nb_lignes * C->nb_colonnes; i++) {
        if (C->tab[i].mine == 0 && C->tab[i].status == PASVU) {
            return 0;
        }
    }
    return 1;
}

/* Vérifie si le joueur a gagné*/
int a_gagne(champs C) {
    if (fin_jeu(C) && C->nb_erreurs_restant >= 0) {
        return 1;
    }
    return 0;
}

//lancement jeu par defaut

champs menuDefaut() {
    champs A = (champs) malloc(sizeof (str_champs));
    char choix;
    erase();
    printw("\n*******************************\n");
    printw("Choisissez un niveau\n");
    printw("\n 1 - facile");
    printw("\n 2 - moyen");
    printw("\n 3 - expert");
    printw("\n*******************************\n");
    refresh();
    do {
        choix = getchar();
        switch (choix) {
            case '1': A = init(5, 5, 5);
                A->nb_erreurs_restant = 1;
                break;
            case '2': A = init(10, 10, 20);
                A->nb_erreurs_restant = 1;
                break;
            case '3': A = init(15, 15, 60);
                A->nb_erreurs_restant = 1;
                break;
        }
    } while ((choix > '3' || choix < '1'));
    return A;
}

void commencerJeu(champs C) {
    char* choix = malloc(1);
    int triche = 0;
    while (C->nb_erreurs_restant >= 0 && !fin_jeu(C)) {
        int x, y;
        if (triche == 0)
            affichage(C);
        else {
            affichage_triche(C);
            triche = 0;
        }

        move(1 + C->nb_lignes + 10, 33);
        scanw("%s", choix);

        if (choix[0] == 'q' || choix[0] == 'Q')
            break;
        if (choix[0] == 's' || choix[0] == 'S') {
            char reponse;
            char *nom_fichier = malloc(0);
            int enregistre = 0;
            while (1) {
                printw("donner le nom sous lequel vous voulez enregistrer la partie");
                scanw("%s", nom_fichier);
                if (fopen(nom_fichier, "r") == NULL) {
                    sauvegarder(nom_fichier, C);
                    enregistre = 1;
                    break;
                } else {
                    printw("Le fichier existe déja. Voulez vous l'écraser o/n?");
                    scanw("%c", &reponse);
                    if (reponse == 'o' || reponse == 'O') {
                        sauvegarder(nom_fichier, C);
                        enregistre = 1;
                        break;
                    } else {
                        printw("Voulez vous choisir un autre nom o/n?");
                        scanw("%c", &reponse);
                        if (!(reponse == 'o' || reponse == 'O')) {
                            break;
                        }
                    }
                }
            }
            if (enregistre == 1) break;
        }

        if (choix[0] == 'a' || choix[0] == 'A')
            triche = 1;
        else {
            recupXY(choix, &x, &y);
            if (joueur(C, x, y) == 1)
                C->nb_erreurs_restant--;
        }
    }
}