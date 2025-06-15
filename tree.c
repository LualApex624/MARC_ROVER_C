#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include <time.h>

// Fonction pour créer un nœud de l'arbre
p_node createNode(t_localisation loc, t_soil soil_type, int cost, int nbSons, const t_move *available_moves, int nbMoves, int depth, p_node parent, t_move move_to_do) {
    // Allocation dynamique pour un nouveau nœud
    p_node node = (p_node) malloc(sizeof(t_node));
    if (node == NULL) {
        printf("Allocation failed for node\n");
        return NULL; // Si l'allocation échoue, retourner NULL
    }

    // Initialisation des propriétés du nœud
    node->loc = loc;
    node->soil_type = soil_type;
    node->cost = cost;
    node->depth = depth;
    node->nbSons = nbSons;
    node->parent = parent;
    node->move_from_parent = move_to_do;
    node->nbMoves = nbMoves;

    // Allocation pour les mouvements disponibles
    if (nbMoves > 0) {
        node->moves_available = (t_move *) malloc(nbMoves * sizeof(t_move));
        if (node->moves_available == NULL) {
            free(node);
            printf("Moves allocation failed\n");
            return NULL;
        }
        for (int i = 0; i < nbMoves; i++) {
            node->moves_available[i] = available_moves[i];
        }
    } else {
        node->moves_available = NULL;
    }

    // Allocation pour les fils
    if (nbSons > 0) {
        node->sons = (p_node *) malloc(nbSons * sizeof(p_node));
        if (node->sons == NULL) {
            free(node->moves_available);
            free(node);
            printf("Sons allocation failed\n");
            return NULL;
        }
        for (int i = 0; i < nbSons; i++) {
            node->sons[i] = NULL;
        }
    } else {
        node->sons = NULL;
    }

    return node; // Retourner le nœud créé
}

// Fonction pour copier les mouvements disponibles tout en excluant un mouvement donné
void copyAvailableMoves(t_move *source, t_move **destination, int nbMoves, t_move move_to_exclude) {
    int i, j = 0;
    int occurrenceSautee = 0;

    for (i = 0; i < nbMoves; i++) {
        // Exclure la première occurrence du mouvement à exclure
        if (source[i] == move_to_exclude && !occurrenceSautee) {
            occurrenceSautee = 1;
            continue;
        }
        (*destination)[j++] = source[i];
    }
}

// Fonction récursive pour créer un arbre de décision
void createTreeRecursivity(p_node parent, t_map map, int k) {
    // Condition d'arrêt pour éviter une récursion excessive ou des positions invalides
    if (parent == NULL || parent->depth >= k || !isValidLocalisation(parent->loc.pos, map.x_max, map.y_max)) {
        return;
    }

    // Si le terrain est une crevasse ou le coût est trop élevé, arrêter la création de fils
    if (map.soils[parent->loc.pos.y][parent->loc.pos.x] == CREVASSE || map.costs[parent->loc.pos.y][parent->loc.pos.x] > 10000) {
        return;
    }

    // Créer des enfants pour chaque mouvement disponible
    for (int i = 0; i < parent->nbMoves; i++) {
        t_localisation son_loc = move(parent->loc, parent->moves_available[i]);
        if (!isValidLocalisation(son_loc.pos, map.x_max, map.y_max)) {
            continue; // Ignorer les localisations invalides
        }

        // Créer un tableau de mouvements mis à jour en excluant le mouvement actuel
        t_move * updated_moves = malloc(((parent->nbMoves) - 1) * sizeof(t_move));
        copyAvailableMoves(parent->moves_available, &updated_moves, parent->nbMoves, parent->moves_available[i]);

        // Créer un nœud enfant
        p_node son = createNode(son_loc,
                                map.soils[son_loc.pos.y][son_loc.pos.x],
                                map.costs[son_loc.pos.y][son_loc.pos.x],
                                parent->nbSons - 1,
                                updated_moves,
                                parent->nbMoves - 1,
                                parent->depth + 1,
                                parent,
                                parent->moves_available[i]);

        if (son == NULL) {
            continue; // Si l'allocation échoue, ignorer cette branche
        }

        parent->sons[i] = son; // Ajouter l'enfant au parent
        createTreeRecursivity(son, map, k); // Appel récursif pour créer les sous-arbres
        free(updated_moves); // Libérer la mémoire des mouvements mis à jour
    }
}

// Fonction pour libérer la mémoire de l'arbre
void freeTree(p_node node) {
    if (node == NULL) return;

    // Libérer les sous-arbres
    for (int i = 0; i < node->nbSons; i++) {
        freeTree(node->sons[i]);
    }

    // Libérer les fils et les mouvements disponibles
    if (node->sons != NULL) {
        free(node->sons);
    }

    if (node->moves_available != NULL) {
        free(node->moves_available);
    }

    // Libérer le nœud lui-même
    free(node);
}

// Fonction récursive pour trouver la feuille ayant le coût minimal dans un nœud
void findMinCostLeafInNode(p_node node, p_node * leaf, int k) {
    if (node == NULL)
        return;

    // Si le nœud est une feuille, vérifier s'il a un coût inférieur au coût de la feuille actuelle
    if(node->nbSons == 9 - k) {
        if (*leaf == NULL || node->cost < (*leaf)->cost) {
            *leaf = node;
        }
    }

    // Parcourir récursivement les fils du nœud
    for (int i = 0; i < node->nbSons; i++) {
        findMinCostLeafInNode(node->sons[i], leaf, k);
    }
}

// Fonction pour trouver la feuille de coût minimal à partir de la racine
void findMinCostLeaf(p_node root, p_node *leaf, int k) {
    clock_t start = clock();
    if (root == NULL)
        return;

    findMinCostLeafInNode(root, leaf, k);
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Temps pour trouver le chemin vers la feuille de plus bas coût : %f secondes\n", elapsed_time);
}

// Fonction pour imprimer l'arbre en utilisant une structure hiérarchique
void printTree(p_node root, int level) {
    if (root == NULL)
        return;

    printf("   ");

    for (int i = 0; i <= level; i++) printf("  ");
    printf(" |---Position (%d, %d), Orientation %d, Cost: %d, move = %s, nb = %d\n", root->loc.pos.x, root->loc.pos.y, root->loc.ori, root->cost,
           getMoveAsString(root->move_from_parent), root->nbSons );

    // Appel récursif pour chaque fils
    for (int i = 0; i < root->nbSons; i++) {
        printTree(root->sons[i], level + 1);
    }
}

// Fonction pour imprimer le chemin de la feuille à la racine
void printPathToRoot(p_node leaf) {
    if (leaf == NULL) return;

    if (leaf->parent != NULL) {
        printPathToRoot(leaf->parent);
        printf(" -> %s", getMoveAsString(leaf->move_from_parent));
    } else {
        printf("START");
    }
}

// Fonction pour vérifier si une position est celle de la base
int hasReachedBase(t_map map, t_position pos) {
    t_position base = getBaseStationPosition(map);
    if(pos.x == base.x && pos.y == base.y)
        return 1;
    return 0;
}

// Fonction pour exécuter la phase initiale et construire l'arbre
p_node phase(int x, int y, t_orientation ori, t_map map) {
    t_localisation start_loc = loc_init(x, y, ori);
    t_move * moves = getRandomMoves(9);
    // Afficher les mouvements aléatoires
    for(int i = 0; i < 9; i++) {
        printf(" %s ", getMoveAsString(moves[i]));
    }
    printf("\n");
    p_node root = createNode(start_loc, map.soils[start_loc.pos.y][start_loc.pos.x], map.costs[start_loc.pos.y][start_loc.pos.x], 9, moves, 9, 0, NULL, START);
    int k = 5;
    if(map.soils[y][x] == 3) {
        printf("sol reg\n");
        k = 4;
    }
    clock_t start = clock();
    createTreeRecursivity(root, map, k);
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Temps pour construire l'arbre : %f secondes\n", elapsed_time);

    p_node minLeaf = NULL;
    findMinCostLeaf(root, &minLeaf, k);
    if(minLeaf != NULL) {
        printf("\nFeuille de cout minimal : %d\n", minLeaf->cost);
        printPathToRoot(minLeaf);
    }
    return minLeaf;
}

// Fonction pour trouver le chemin vers la base en utilisant plusieurs phases successives
void pathToBase(int x, int y, t_orientation ori, t_map map) {
    clock_t start = clock();
    printf("position initiale %d %d %d\n", x, y, ori);
    p_node temp;
    temp = phase(x, y, ori, map);
    printf("\nposition suivante %d %d %d\n", temp->loc.pos.x, temp->loc.pos.y, temp->loc.ori);

    // Vérifier si la position actuelle est celle de la base
    while(temp->cost != 0) {
        temp = phase(temp->loc.pos.x, temp->loc.pos.y, temp->loc.ori, map);
        printf("\nposition suivante %d %d %d\n", temp->loc.pos.x, temp->loc.pos.y, temp->loc.ori);
    }

    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Temps pour arriver à la base : %f secondes\n", elapsed_time);
}
