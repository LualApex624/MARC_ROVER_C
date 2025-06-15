#include <stdio.h>
#include "map.h"
#include "loc.h"
#include "moves.h"
#include "tree.h"


void testFunctions();
void quickTest(t_map map);

int main() {

    testFunctions();

    return 0;
}



// Fonction de test pour vérifier toutes les fonctionnalités
void testFunctions() {
    t_map map;

    // The following preprocessor directive checks if the code is being compiled on a Windows system.
    // If either _WIN32 or _WIN64 is defined, it means we are on a Windows platform.
    // On Windows, file paths use backslashes (\), hence we use the appropriate file path for Windows.
#if defined(_WIN32) || defined(_WIN64)
    map = createMapFromFile("..\\maps\\map2.map");
#else
    map = createMapFromFile("../maps/example1.map");
#endif

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    //displayMap(map);


    // Vérification de la station de base
    /* t_position base_pos = getBaseStationPosition(map);
     printf("Base Station Position: (%d, %d)\n", base_pos.x, base_pos.y);

     // Test de la localisation initiale
     t_localisation init = loc_init(4, 6, NORTH);
     printf("Initial localisation: Position (%d, %d), Orientation %d\n", init.pos.x, init.pos.y, init.ori);

     // Test de validation de localisation
     t_position pos = {4, 6};
     printf("Is position (%d, %d) valid? %d\n", pos.x, pos.y, isValidLocalisation(pos, map.x_max, map.y_max));

     // Tests de mouvements individuels
     printf("\n--- Testing Moves ---\n");
     t_localisation loc = init;

     t_move moves[] = {F_10, T_RIGHT, F_20, T_LEFT, B_10, U_TURN};
     for (int i = 0; i < 6; i++) {
         loc = move(loc, moves[i]);
         printf("Move %s: New Position (%d, %d), Orientation %d\n", getMoveAsString(moves[i]), loc.pos.x, loc.pos.y, loc.ori);
     }
 printf("\n__________________________________________\n");

 */

    quickTest(map);


}


void quickTest(t_map map){

    t_localisation start_loc = { {  6 ,0 }, SOUTH };
    pathToBase(start_loc.pos.x, start_loc.pos.y,start_loc.ori, map);
}