/**
 * Fichier permettant de définir les fonctions de gestion des signaux.
 * Ces fonctions doivent être implémentées dans des fichiers séparés
 *   pour la gestion des signaux dans le server et le client.
 */

#ifndef SIGNAL_H
#define SIGNAL_H

/**
 * Permet de signaler que le programme doit se termnier
 * Post-condition : 
 *   - isDone() == 1;
 */
void done();

/**
 * Permet de savoir si le programme doir se terminer.
 */
int isDone();

/**
 * Permet d'installer le gestionnaire de signaux.
 */
void setSignals();

#endif
