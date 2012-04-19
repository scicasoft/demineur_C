#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include "utils.h"

int main(int argc, char** argv) {
    char choix;
    int arg1, arg2, arg3, arg4, recommence=0;
    champs C = (champs) malloc(sizeof (str_champs));
    time_t t1, t2;
    double dt;
    //initialisation de rand() pour pouvoir generer correctement
    srand((unsigned int) time(NULL));
    initscr(); /* Start curses mode     */

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_WHITE);
    init_pair(2, COLOR_GREEN, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    bkgd(COLOR_PAIR(1));

    // Demarrage du jeu de plusieurs manieres

    switch (argc) { // execution en fonction des arguments envoyés
        case 3: // a partir d un fichier de sauvegarde
            if (argv[1] == "-f")
                C = lire(argv[2]);
            else {
                printw("erreur lors de la lecture du fichier");
                getch();
                exit(1);
            }
            break;

        case 4: //qui initialise le jeu de la maniere suivante demineur nbLignes nbColonnes nbMines
            arg1 = atoi(argv[1]);
            arg2 = atoi(argv[2]);
            arg3 = atoi(argv[3]);
            if (arg3 > (arg1 * arg2)) {
                printw("Le nombre de mines est superieur au nombre de cases qui est : %d", arg1 * arg2);
                getch();
                exit(1);
            }
            C = init(arg1, arg2, arg3);
            C->nb_erreurs_restant = 0;
            break;
        case 5: //qui initialise le jeu de la maniere suivante demineur nbLignes nbColonnes nbMines nbErreurs
            arg1 = atoi(argv[1]);
            arg2 = atoi(argv[2]);
            arg3 = atoi(argv[3]);
            arg4 = atoi(argv[4]);
            if (arg3 > (arg1 * arg2)) {
                printf("Le nombre de mines est superieur au nombre de cases qui est : %d", arg1 * arg2);
                exit(1);
            } else
                C = init(arg1, arg2, arg3);
            if (arg4 < 0) {
                printf("Entrer un nombre d'erreurs positifs s'il vous plait");
                exit(1);
            } else
                C->nb_erreurs_restant = arg4;
            break;

        default:
            C = menuDefaut();
    }
    do {
        if (recommence)
            C = menuDefaut();
        t1 = time(NULL);
        commencerJeu(C);
        t2 = time(NULL);
        erase();
        //affichage(C);
        if (fin_jeu(C)) {
            if (a_gagne(C)) {
                printw("C'est boz!!! nice cool\n");
            } else {
                printw("Desole, goorgoorloul boubakh pour la prochaine fois\n");
            }
            dt = difftime(t2, t1);
            printw("\nDuree : %lf secondes\n", dt);
        }
        printw("Voulez vous recommencer [O/N] ?\n");
        refresh();
        do {
            choix = getchar();
        } while(!(choix=='N' || choix=='O' || choix=='o' || choix=='o'));
        recommence=1;
    } while(!(choix=='N' || choix=='n'));
    endwin();
    return (EXIT_SUCCESS);
}