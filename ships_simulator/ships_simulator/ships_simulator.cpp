#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#define CAR 5
#define BAT 4
#define CRU 3
#define DES 2

using namespace std;

struct ship
{
    int ind, x1, y1, x2, y2, type, amount, can[2], eng[2], moves, pattern[5] = { 1 }, pattern_set = 0, int_pattern, shots;
    char dir, chartype[3];
};

struct fleet
{
    int CARs[10], BATs[10], CRUs[10], DESs[10];
    ship* CAR_ships[10], * BAT_ships[10], * CRU_ships[10], * DES_ships[10];
};

struct init
{
    int x1, y1, x2, y2;
};

struct board_size
{
    int MAX_X, MAX_Y;
};

fleet* make_fleet(int CARs, int BATs, int CRUs, int DESs);

ship* make_ship(fleet* player_fleet, char place_type, int* ship_parts, char player, board_size cor);

char** make_board();

void shoot(int x, int y, char** board, int* parts_count, board_size cor);

void set_board(char** board, board_size cor);

void delete_ship(char** board, fleet* player_fleet, char type[3], int ind, board_size cor);

void add_ship(char** board, fleet* player_fleet, char type[3], int ind, board_size cor);

int check_in_board(ship new_ship, char player, init player_pos);

int move_in_board(ship new_ship, board_size cor);

int reef_on_board(int x, int y, board_size cor);

int check_reef(char** board, ship new_ship, board_size cor);

void make_init(init* player_init, int x1, int y1, int x2, int y2);

init* default_pos(char player, board_size cor);

init* make_pos(char player);

int n_head_check(char** board, ship new_ship, int i, board_size cor);

int check_surround(char** board, ship new_ship, board_size cor);

int n_back_check(char** board, ship new_ship, int i, board_size cor);

int n_s_side_check(char** board, ship new_ship, int i, board_size cor);

int w_head_check(char** board, ship new_ship, int i, board_size cor);

int w_back_check(char** board, ship new_ship, int i, board_size cor);

int w_e_side_check(char** board, ship new_ship, int i, board_size cor);

board_size* make_cor();

void update_ship(ship* new_ship);

int check_ship_moves(ship new_ship, char side);

void erase_moves(ship* new_ship);

void erase_shots(ship* new_ship);

int check_engine(char** board, ship new_ship, board_size cor);

void print_board(char** board, board_size cor);

void extended_shoot(int x, int y, char** board, int* parts_count, board_size cor, ship* new_ship);

int dist(int x1, int y1, int x2, int y2);

int check_can(char** board, ship new_ship);

void fleet_add(fleet* player_fleet, ship new_ship, int CARs, int BATs, int CRUs, int DESs);

int check_fleet(int CARs, int BATs, int CRUs, int DESs, fleet player_fleet);

void move(ship* new_ship);

void rotate(ship* new_ship, char side);

int move_ship(char** board, fleet* player_fleet, char type[3], char side, int ind, char player, board_size cor);

void set_ship(char** board, char player, ship new_ship, char command[15], int* inv, fleet player_fleet, init player_pos, board_size cor);


int main()
{
    char turn, command[15];
    int A_quit_turn = 0, B_quit_turn = 0, shipsA_count = 0, shipsB_count = 0, A_parts = 0, B_parts = 0, state_count = 0, inv = 0, fleet_set = 0;
    int A_CARs, A_BATs, A_CRUs, A_DESs, B_CARs, B_BATs, B_CRUs, B_DESs, ships_not_placed = 0, A_turns = 0, B_turns = 0, wrong_player_end = 0;
    int max_x = 21, max_y = 21, board_set = 0, Apos_set = 0, Bpos_set = 0, extended = 0;
    ship* shipsA[100];
    ship* shipsB[100];
    fleet* fleetA = make_fleet(1, 2, 3, 4);
    fleet* fleetB = make_fleet(1, 2, 3, 4);
    board_size* cor = make_cor();
    init* posA = make_pos('A');
    init* posB = make_pos('B');
    char** board = make_board();


    fleet_set = 0;
    while (cin >> command) {
        inv = 0;
        if ((command[0] == '[' && command[7] == 'B' && B_quit_turn == 1) || (command[0] == '[' && command[7] == 'A' && A_quit_turn == 1)) {
            wrong_player_end += 1;
        }
        state_count = 0;
        if (command[0] == '[' && command[1] == 's' && state_count % 2 != 1) {
            cin >> command;
            while (command[0] != '[' && command[1] != 's') {
                if (command[0] == 'B' && command[1] == 'O') {
                    cin >> max_x >> max_y;
                    board = (char**)malloc(max_x * sizeof(char*));
                    for (int i = 0; i < max_x; i++) {
                        board[i] = (char*)malloc(max_y * sizeof(char));
                    }
                    cor->MAX_X = max_x;
                    cor->MAX_Y = max_y;
                    board_set = 1;
                    set_board(board, *cor);
                }
                if (board_set == 0) {
                    board = (char**)malloc(max_x * sizeof(char*));
                    for (int i = 0; i < max_x; i++) {
                        board[i] = (char*)malloc(max_y * sizeof(char));
                    }
                    set_board(board, *cor);
                }
                if (command[0] == 'P' && command[1] == 'R') {
                    if (!inv) {
                        print_board(board, *cor);
                        cout << "PARTS REMAINING:: A : " << A_parts << " B : " << B_parts << endl;
                    }
                }
                if (command[0] == 'S' && command[1] == 'E') {
                    if (board_set == 0) {
                        set_board(board, *cor);
                        board_set = 1;
                    }
                    char player;
                    cin >> player;
                    if (player == 'A') {
                        cin >> A_CARs >> A_BATs >> A_CRUs >> A_DESs;
                        fleet* fleetA = make_fleet(A_CARs, A_BATs, A_CRUs, A_DESs);
                    }
                    if (player == 'B') {
                        cin >> B_CARs >> B_BATs >> B_CRUs >> B_DESs;
                        fleet* fleetB = make_fleet(B_CARs, B_BATs, B_CRUs, B_DESs);
                    }
                    fleet_set = 1;
                }
                if (command[0] == 'E') {
                    extended = 1;
                }
                if (command[0] == 'R') {
                    if (board_set == 0) {
                        set_board(board, *cor);
                        board_set = 1;
                    }
                    int x, y;
                    cin >> x >> y;
                    if (reef_on_board(x, y, *cor)) {
                        board[x][y] = '#';
                    }
                    else {
                        cout << "INVALID OPERATION REEF" << x << ' ' << y << "REEF IS NOT PLACED ON BOARD" << endl;
                        exit(1);
                    }
                }
                if (command[0] == 'N') {
                    char turn;
                    cin >> turn;
                    if (turn == 'A') {
                        B_quit_turn = 1;
                    }
                    else {
                        A_quit_turn = 1;
                    }
                }
                if (command[0] == 'I') {
                    if (board_set == 0) {
                        set_board(board, *cor);
                        board_set = 1;
                    }
                    char player;
                    int x1, y1, x2, y2;
                    cin >> player >> x1 >> y1 >> x2 >> y2;
                    if (player == 'A') {
                        make_init(posA, x1, y1, x2, y2);
                        Apos_set = 1;
                    }
                    else {
                        make_init(posB, x1, y1, x2, y2);
                        Bpos_set = 1;
                    }
                }

                if (command[0] == 'S' && command[1] == 'H') {
                    if (board_set == 0) {
                        set_board(board, *cor);
                        board_set = 1;
                    }
                    if (!fleet_set) {
                        A_CARs = B_CARs = 1;
                        A_BATs = B_BATs = 2;
                        A_CRUs = B_CRUs = 3;
                        A_DESs = B_DESs = 4;
                    }
                    char player;
                    cin >> player;
                    if (player == 'A') {
                        ship* new_ship = make_ship(fleetA, 's', &A_parts, player, *cor);
                        shipsA[shipsA_count] = new_ship;
                        shipsA_count++;
                        set_ship(board, 'A', *new_ship, command, &inv, *fleetA, *posA, *cor);
                        fleet_add(fleetA, *new_ship, A_CARs, A_BATs, A_CRUs, A_DESs);
                    }
                    else {
                        ship* new_ship = make_ship(fleetB, 's', &B_parts, player, *cor);
                        shipsB[shipsB_count] = new_ship;
                        shipsB_count++;
                        set_ship(board, 'B', *new_ship, command, &inv, *fleetB, *posB, *cor);
                        fleet_add(fleetB, *new_ship, B_CARs, B_BATs, B_CRUs, B_DESs);
                    }
                }

                cin >> command;
            }
            if (Apos_set == 0) {
                posA = default_pos('A', *cor);
            }
            if (Bpos_set == 0) {
                posB = default_pos('B', *cor);
            }

            state_count += 1;
        }
        if (!fleet_set) {
            A_CARs = B_CARs = 1;
            A_BATs = B_BATs = 2;
            A_CRUs = B_CRUs = 3;
            A_DESs = B_DESs = 4;
        }
        if (board_set == 0) {
            set_board(board, *cor);
            board_set = 1;
        }
        if ((command[0] == '[' && command[7] == 'A') || (command[0] == '[' && command[7] == 'B')) {

            if (command[0] == '[' && command[7] == 'A' && A_quit_turn == 0) {
                B_quit_turn = 0;
                turn = 'A';
                while (!A_quit_turn) {
                    int shot_count = 0;
                    cin >> command;
                    if (command[0] == 'P' && command[1] == 'L') {
                        ship* new_ship = make_ship(fleetA, 'd', &A_parts, turn, *cor);
                        shipsA[shipsA_count] = new_ship;
                        shipsA_count++;
                        set_ship(board, turn, *new_ship, command, &inv, *fleetA, *posA, *cor);
                        fleet_add(fleetA, *new_ship, A_CARs, A_BATs, A_CRUs, A_DESs);


                    }
                    if (command[0] == 'S' && command[1] == 'H') {
                        if (extended == 0 && shot_count == 0) {
                            int shoot_x, shoot_y;
                            cin >> shoot_x >> shoot_y;
                            if (check_fleet(B_CARs, B_BATs, B_CRUs, B_DESs, *fleetB)) {
                                shoot(shoot_x, shoot_y, board, &B_parts, *cor);
                                shot_count = 1;
                            }
                            else {
                                cout << "INVALID OPERATION \"SHOOT " << shoot_x << " " << shoot_y << "\": NOT ALL SHIPS PLACED" << endl;
                                exit(1);
                            }
                        }
                        else if (extended == 1) {
                            int shoot_x, shoot_y, ind;
                            char type[3];
                            cin >> ind >> type >> shoot_x >> shoot_y;
                            if (check_fleet(B_CARs, B_BATs, B_CRUs, B_DESs, *fleetB)) {
                                ship* shooting_ship;
                                if (type[0] == 'C' && type[1] == 'A') {
                                    extended_shoot(shoot_x, shoot_y, board, &B_parts, *cor, fleetA->CAR_ships[ind]);
                                }
                                else if (type[0] == 'B' && type[1] == 'A') {
                                    extended_shoot(shoot_x, shoot_y, board, &B_parts, *cor, fleetA->BAT_ships[ind]);
                                }
                                else if (type[0] == 'C' && type[1] == 'R') {
                                    extended_shoot(shoot_x, shoot_y, board, &B_parts, *cor, fleetA->CRU_ships[ind]);
                                }
                                else if (type[0] == 'D' && type[1] == 'E') {
                                    extended_shoot(shoot_x, shoot_y, board, &B_parts, *cor, fleetA->DES_ships[ind]);
                                }

                                shot_count = 1;
                            }
                            else {
                                cout << "INVALID OPERATION \"SHOOT " << shoot_x << " " << shoot_y << "\": NOT ALL SHIPS PLACED" << endl;
                                exit(1);
                            }
                        }

                    }
                    if (command[0] == 'M' && command[1] == 'O') {
                        char type[3], side;
                        int ind;
                        cin >> ind >> type >> side;
                        move_ship(board, fleetA, type, side, ind, turn, *cor);

                    }
                    if (command[0] == '[' && command[7] == turn) {
                        A_quit_turn = 1;
                        for (int i = 0; i <= shipsA_count - 1; i++) {
                            erase_moves(shipsA[i]);
                            erase_shots(shipsA[i]);
                        }
                    }
                }


            }
            else if (command[0] == '[' && command[7] == 'B' && B_quit_turn == 0) {
                A_quit_turn = 0;
                turn = 'B';
                while (!B_quit_turn) {
                    int shot_count = 0;
                    cin >> command;
                    if (command[0] == 'P' && command[1] == 'L') {
                        ship* new_ship = make_ship(fleetB, 'd', &B_parts, turn, *cor);
                        shipsB[shipsB_count] = new_ship;
                        shipsB_count++;
                        set_ship(board, turn, *new_ship, command, &inv, *fleetB, *posB, *cor);
                        fleet_add(fleetB, *new_ship, B_CARs, B_BATs, B_CRUs, B_DESs);



                    }
                    if (command[0] == 'S' && command[1] == 'H') {
                        if (extended == 0 && shot_count == 0) {
                            int shoot_x, shoot_y;
                            cin >> shoot_x >> shoot_y;
                            if (check_fleet(A_CARs, A_BATs, A_CRUs, A_DESs, *fleetA)) {
                                shoot(shoot_x, shoot_y, board, &A_parts, *cor);
                                shot_count = 1;
                            }
                            else {
                                cout << "INVALID OPERATION \"SHOOT " << shoot_x << " " << shoot_y << "\": NOT ALL SHIPS PLACED" << endl;
                                exit(1);
                            }
                        }
                        else if (extended == 1) {
                            int shoot_x, shoot_y, ind;
                            char type[3];
                            cin >> ind >> type >> shoot_x >> shoot_y;
                            if (check_fleet(A_CARs, A_BATs, A_CRUs, A_DESs, *fleetA)) {
                                if (type[0] == 'C' && type[1] == 'A') {
                                    extended_shoot(shoot_x, shoot_y, board, &A_parts, *cor, fleetB->CAR_ships[ind]);
                                }
                                else if (type[0] == 'B' && type[1] == 'A') {
                                    extended_shoot(shoot_x, shoot_y, board, &A_parts, *cor, fleetB->BAT_ships[ind]);
                                }
                                else if (type[0] == 'C' && type[1] == 'R') {
                                    extended_shoot(shoot_x, shoot_y, board, &A_parts, *cor, fleetB->CRU_ships[ind]);
                                }
                                else if (type[0] == 'D' && type[1] == 'E') {
                                    extended_shoot(shoot_x, shoot_y, board, &A_parts, *cor, fleetB->DES_ships[ind]);
                                }

                                shot_count = 1;
                            }
                            else {
                                cout << "INVALID OPERATION \"SHOOT " << shoot_x << " " << shoot_y << "\": NOT ALL SHIPS PLACED" << endl;
                                exit(1);
                            }
                        }

                    }
                    if (command[0] == 'M' && command[1] == 'O') {
                        char type[3], side;
                        int ind;
                        cin >> ind >> type >> side;
                        move_ship(board, fleetB, type, side, ind, turn, *cor);

                    }
                    if (command[0] == '[' && command[7] == turn) {
                        B_quit_turn = 1;
                        for (int i = 0; i <= shipsB_count - 1; i++) {
                            erase_moves(shipsB[i]);
                            erase_shots(shipsB[i]);
                        }

                    }
                }

            }

            if ((A_parts == 0 && shipsA_count != 0) || (B_parts == 0 && shipsB_count != 0)) {
                if (A_parts == 0) {
                    cout << "B won" << endl;
                }
                else if (B_parts == 0) {
                    cout << "A won" << endl;
                }
                break;
            }
            if (wrong_player_end == 2) {
                cout << "INVALID OPERATION \"[player" << command[7] << "] \": THE OTHER PLAYER EXPECTED" << endl;
                exit(1);
            }
        }

        if (wrong_player_end == 2) {
            exit(1);
        }
        if (ships_not_placed) {
            exit(1);
        }
        if (inv == 1) {
            exit(1);
        }
        if (command[0] == '\0') {
            exit(1);
        }
    }
    exit(0);
    return 0;
}

fleet* make_fleet(int CARs, int BATs, int CRUs, int DESs) {
    fleet* new_fleet = (fleet*)malloc(sizeof(fleet));
    for (int i = 0; i < CARs; i++) {
        new_fleet->CARs[i] = 0;
    }
    for (int i = 0; i < BATs; i++) {
        new_fleet->BATs[i] = 0;
    }
    for (int i = 0; i < CRUs; i++) {
        new_fleet->CRUs[i] = 0;
    }
    for (int i = 0; i < DESs; i++) {
        new_fleet->DESs[i] = 0;
    }

    return new_fleet;
}

ship* make_ship(fleet* player_fleet, char place_type, int* ship_parts, char player, board_size cor) {
    ship* new_ship = (ship*)malloc(sizeof(ship));
    int x, y, ind, pattern = 0;
    char dir, type[4], char_pattern[6];
    if (place_type == 'd') {
        cin >> x >> y >> dir >> ind >> type;
    }
    else {
        cin >> x >> y >> dir >> ind >> type >> pattern;
    }

    new_ship->x1 = x;
    new_ship->y1 = y;
    new_ship->dir = dir;
    new_ship->ind = ind;
    new_ship->moves = 0;
    if (type[0] == 'C' && type[1] == 'A' && type[2] == 'R') {
        new_ship->type = CAR;
        new_ship->amount = 1;

    }
    else if (type[0] == 'B' && type[1] == 'A' && type[2] == 'T') {
        new_ship->type = BAT;
        new_ship->amount = 2;

    }
    else if (type[0] == 'C' && type[1] == 'R' && type[2] == 'U') {
        new_ship->type = CRU;
        new_ship->amount = 3;

    }
    else if (type[0] == 'D' && type[1] == 'E' && type[2] == 'S') {
        new_ship->type = DES;
        new_ship->amount = 4;

    }
    new_ship->chartype[0] = type[0];
    new_ship->chartype[1] = type[1];
    new_ship->chartype[2] = type[2];
    new_ship->shots = 0;
    if (new_ship->type) {
        if (dir == 'N') {
            new_ship->x2 = new_ship->eng[0] = x + (new_ship->type - 1);
            new_ship->y2 = new_ship->eng[1] = y;
            new_ship->can[0] = x + 1;
            new_ship->can[1] = y;
        }
        else if (dir == 'S') {
            new_ship->x2 = new_ship->eng[0] = x - (new_ship->type - 1);
            new_ship->y2 = new_ship->eng[1] = y;
            new_ship->can[0] = x - 1;
            new_ship->can[1] = y;
        }
        else if (dir == 'E') {
            new_ship->x2 = new_ship->eng[0] = x;
            new_ship->y2 = new_ship->eng[1] = y - (new_ship->type - 1);
            new_ship->can[0] = x;
            new_ship->can[1] = y - 1;

        }
        else if (dir == 'W') {
            new_ship->x2 = new_ship->eng[0] = x;
            new_ship->y2 = new_ship->eng[1] = y + (new_ship->type - 1);
            new_ship->can[0] = x;
            new_ship->can[1] = y + 1;
        }
    }
    if (place_type != 'd') {
        sprintf_s(char_pattern, "%ld", pattern);
        for (int i = 0; i < new_ship->type; i++) {
            new_ship->pattern[i] = int(char_pattern[i] - '0');
            if (int(char_pattern[i] - '0') == 1) {
                *ship_parts += 1;
            }
        }
        new_ship->int_pattern = pattern;
        new_ship->pattern_set = 1;
    }
    else {
        for (int i = 0; i < new_ship->type; i++) {
            new_ship->pattern[i] = 1;
            pattern += pow(10, i);
        }
        new_ship->int_pattern = pattern;
        *ship_parts += new_ship->type;
        new_ship->pattern_set = 0;
    }
    if (new_ship->type == CAR) {
        player_fleet->CAR_ships[new_ship->ind] = new_ship;
    }
    else if (new_ship->type == BAT) {
        player_fleet->BAT_ships[new_ship->ind] = new_ship;
    }
    else if (new_ship->type == CRU) {
        player_fleet->CRU_ships[new_ship->ind] = new_ship;
    }
    else if (new_ship->type == DES) {
        player_fleet->DES_ships[new_ship->ind] = new_ship;
    }

    if (x > cor.MAX_X || y > cor.MAX_Y) {
        if (new_ship->pattern_set == 1) {
            cout << "INVALID OPERATION \"" << "SHIP" << ' ' << player << " " << new_ship->x1 << " " << new_ship->y1 << " " << new_ship->dir << " " << new_ship->ind << " " << type[0] << type[1] << type[2] << new_ship->int_pattern << "\": NOT IN STARTING POSITION" << endl;
            exit(1);
        }
        else {
            cout << "INVALID OPERATION \"" << "PLACE_SHIP" << " " << new_ship->x1 << " " << new_ship->y1 << " " << new_ship->dir << " " << new_ship->ind << " " << type[0] << type[1] << type[2] << "\": NOT IN STARTING POSITION" << endl;
            exit(1);
        }

    }
    return new_ship;

}

char** make_board() {
    char** board = (char**)malloc(21 * sizeof(char*));
    for (int i = 0; i < 21; i++) {
        board[i] = (char*)malloc(10 * sizeof(char));
    }
    return board;
}

int reef_on_board(int x, int y, board_size cor) {
    int min_y = 0, max_y = cor.MAX_X - 1, min_x = 0, max_x = cor.MAX_Y;
    if (x <= max_y && x >= min_y && y <= max_x && y >= min_x) {
        return 1;
    }
    else {
        return 0;
    }
}

void make_init(init* player_init, int x1, int y1, int x2, int y2) {
    player_init->x1 = x1;
    player_init->y1 = y1;
    player_init->x2 = x2;
    player_init->y2 = y2;
}

void delete_ship(char** board, fleet* player_fleet, char type[3], int ind, board_size cor) {
    ship old_ship;
    if (type[0] == 'C' && type[1] == 'A') {
        old_ship = *player_fleet->CAR_ships[ind];
    }
    else if (type[0] == 'B' && type[1] == 'A') {
        old_ship = *player_fleet->BAT_ships[ind];
    }
    else if (type[0] == 'C' && type[1] == 'R') {
        old_ship = *player_fleet->CRU_ships[ind];
    }
    else if (type[0] == 'D' && type[1] == 'E') {
        old_ship = *player_fleet->DES_ships[ind];
    }
    if (old_ship.dir == 'N') {
        for (int i = old_ship.x1; i <= old_ship.x2; i++) {
            board[i][old_ship.y1] = ' ';
        }
    }
    else if (old_ship.dir == 'W') {
        for (int i = old_ship.y1; i <= old_ship.y2; i++) {
            board[old_ship.x1][i] = ' ';
        }
    }
    else if (old_ship.dir == 'E') {
        for (int i = old_ship.y1; i >= old_ship.y2; i--) {
            board[old_ship.x1][i] = ' ';
        }
    }
    else if (old_ship.dir == 'S') {
        for (int i = old_ship.x1; i >= old_ship.x2; i--) {
            board[i][old_ship.y1] = ' ';
        }
    }
}

void add_ship(char** board, fleet* player_fleet, char type[3], int ind, board_size cor) {
    ship new_ship;
    if (type[0] == 'C' && type[1] == 'A') {
        new_ship = *player_fleet->CAR_ships[ind];
    }
    else if (type[0] == 'B' && type[1] == 'A') {
        new_ship = *player_fleet->BAT_ships[ind];
    }
    else if (type[0] == 'C' && type[1] == 'R') {
        new_ship = *player_fleet->CRU_ships[ind];
    }
    else if (type[0] == 'D' && type[1] == 'E') {
        new_ship = *player_fleet->DES_ships[ind];
    }
    if (new_ship.dir == 'N') {
        for (int i = new_ship.x1; i <= new_ship.x2; i++) {
            board[i][new_ship.y1] = '+';
        }
    }
    else if (new_ship.dir == 'W') {
        for (int i = new_ship.y1; i <= new_ship.y2; i++) {
            board[new_ship.x1][i] = '+';
        }
    }
    else if (new_ship.dir == 'E') {
        for (int i = new_ship.y1; i >= new_ship.y2; i--) {
            board[new_ship.x1][i] = '+';
        }
    }
    else if (new_ship.dir == 'S') {
        for (int i = new_ship.x1; i >= new_ship.x2; i--) {
            board[i][new_ship.y1] = '+';
        }
    }
}

void shoot(int x, int y, char** board, int* parts_count, board_size cor) {
    int min_y = 0, max_y = cor.MAX_X - 1, min_x = 0, max_x = cor.MAX_Y;
    if (x <= max_y && x >= min_y && y >= min_x && y <= max_x && board[x][y] == '+') {
        if (board[x][y] == '+') {
            board[x][y] = 'x';
            *parts_count = *parts_count - 1;
        }


    }
    else if (x > max_y || x < 0 || y < min_x || y > max_x) {
        cout << "INVALID OPERATION \"SHOOT " << x << " " << y << "\": FIELD DOES NOT EXIST" << endl;
    }
}

void extended_shoot(int x, int y, char** board, int* parts_count, board_size cor, ship* new_ship) {
    int min_y = 0, max_y = cor.MAX_X - 1, min_x = 0, max_x = cor.MAX_Y;
    if (x <= max_y && x >= min_y && y >= min_x && y <= max_x && dist(new_ship->can[0], new_ship->can[1], x, y) <= new_ship->type && check_can(board, *new_ship) && new_ship->shots < new_ship->type) {
        if (board[x][y] == '+') {
            board[x][y] = 'x';
            *parts_count = *parts_count - 1;
        }
        new_ship->shots += 1;

    }
    else if (x > max_y || x < 0 || y < min_x || y > max_x) {
        cout << "INVALID OPERATION \"SHOOT " << new_ship->ind << ' ' << new_ship->chartype[0] << new_ship->chartype[1] << new_ship->chartype[2] << ' ' << x << ' ' << y << "\": FIELD DOES NOT EXIST" << endl;
        exit(1);
    }
    else if (!check_can(board, *new_ship)) {
        cout << "INVALID OPERATION \"SHOOT " << new_ship->ind << ' ' << new_ship->chartype[0] << new_ship->chartype[1] << new_ship->chartype[2] << ' ' << x << ' ' << y << "\": SHIP CANNOT SHOOT" << endl;
        exit(1);
    }
    else if (dist(new_ship->can[0], new_ship->can[1], x, y) > new_ship->type) {
        cout << "INVALID OPERATION \"SHOOT " << new_ship->ind << ' ' << new_ship->chartype[0] << new_ship->chartype[1] << new_ship->chartype[2] << ' ' << x << ' ' << y << "\": SHOOTING TOO FAR" << endl;
        exit(1);
    }
    else if (new_ship->shots == new_ship->type) {
        cout << "INVALID OPERATION \"SHOOT " << new_ship->ind << ' ' << new_ship->chartype[0] << new_ship->chartype[1] << new_ship->chartype[2] << ' ' << x << ' ' << y << "\": TOO MANY SHOOTS" << endl;
        exit(1);
    }
}

int check_can(char** board, ship new_ship) {
    if (new_ship.type != CAR) {
        if (board[new_ship.can[0]][new_ship.can[1]] == 'x') {
            return 0;
        }
    }
    return 1;
}

int dist(int x1, int y1, int x2, int y2) {
    return(sqrt(pow(y1 - y2, 2) + pow(x1 - x2, 2)));
}

void set_board(char** board, board_size cor) {
    for (int i = 0; i < cor.MAX_X; i++) {
        for (int j = 0; j < cor.MAX_Y; j++) {
            board[i][j] = ' ';
        }
    }
}

init* make_pos(char player) {
    init* new_pos = (init*)malloc(sizeof(init));
    if (player == 'A') {
        new_pos->x1 = 0;
        new_pos->x2 = 21 / 2 - 1;
    }
    else {
        new_pos->x1 = 21 / 2 + 1;
        new_pos->x2 = 21 - 1;
    }
    new_pos->y1 = 0;
    new_pos->y2 = 10;
    return new_pos;
}

void print_board(char** board, board_size cor) {
    for (int i = 0; i < cor.MAX_X; i++) {
        for (int j = 0; j < cor.MAX_Y; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

board_size* make_cor() {
    board_size* cor = (board_size*)malloc(sizeof(board_size));
    cor->MAX_X = 21;
    cor->MAX_Y = 10;
    return cor;
}

init* default_pos(char player, board_size cor) {
    init* new_pos = (init*)malloc(sizeof(init));
    if (player == 'A') {
        new_pos->x1 = 0;
        new_pos->x2 = cor.MAX_X / 2 - 1;
    }
    else {
        new_pos->x1 = cor.MAX_X / 2 + 1;
        new_pos->x2 = cor.MAX_X - 1;
    }
    new_pos->y1 = 0;
    new_pos->y2 = cor.MAX_Y;

    return new_pos;
}

void fleet_add(fleet* player_fleet, ship new_ship, int CARs, int BATs, int CRUs, int DESs) {
    if (new_ship.type == CAR) {
        player_fleet->CARs[new_ship.ind] = 1;
    }
    else if (new_ship.type == BAT) {
        player_fleet->BATs[new_ship.ind] = 1;
    }
    else if (new_ship.type == CRU) {
        player_fleet->CRUs[new_ship.ind] = 1;
    }
    else if (new_ship.type == DES) {
        player_fleet->DESs[new_ship.ind] = 1;
    }
    if (CARs == 0) {
        player_fleet->CARs[9] = 1;
    }
    if (BATs == 0) {
        player_fleet->BATs[9] = 1;
    }
    if (CRUs == 0) {
        player_fleet->CRUs[9] = 1;
    }
    if (DESs == 0) {
        player_fleet->DESs[9] = 1;
    }
}

int check_fleet(int CARs, int BATs, int CRUs, int DESs, fleet player_fleet) {
    int CAR_check = 0, BAT_check = 0, CRU_check = 0, DES_check = 0;
    for (int i = 0; i <= CARs; i++) {
        if (player_fleet.CARs[i] == 1) {
            CAR_check += 1;
        }
    }
    for (int i = 0; i <= BATs; i++) {
        if (player_fleet.BATs[i] == 1) {
            BAT_check += 1;
        }
    }
    for (int i = 0; i <= CRUs; i++) {
        if (player_fleet.CRUs[i] == 1) {
            CRU_check += 1;
        }
    }
    for (int i = 0; i <= DESs; i++) {
        if (player_fleet.DESs[i] == 1) {
            DES_check += 1;
        }
    }
    if (CAR_check == CARs && BAT_check == BATs && CRU_check == CRUs && DES_check == DESs) {
        return 1;
    }
    else {
        return 0;
    }
}

void move(ship* new_ship) {
    if (new_ship->dir == 'N') {
        new_ship->x1 -= 1;
        new_ship->x2 -= 1;
    }
    else if (new_ship->dir == 'E') {
        new_ship->y1 += 1;
        new_ship->y2 += 1;
    }
    else if (new_ship->dir == 'W') {
        new_ship->y1 -= 1;
        new_ship->y2 -= 1;
    }
    else {
        new_ship->x1 += 1;
        new_ship->x2 += 1;
    }
}

void rotate(ship* new_ship, char side) {
    move(new_ship);
    if (new_ship->dir == 'N') {
        new_ship->x2 = new_ship->x1;
        if (side == 'R') {
            new_ship->y1 += (new_ship->type - 1);
            new_ship->dir = 'E';
        }
        else {
            new_ship->y1 -= (new_ship->type - 1);
            new_ship->dir = 'W';
        }
    }
    else if (new_ship->dir == 'E') {
        new_ship->y2 = new_ship->y1;
        if (side == 'R') {
            new_ship->x1 += (new_ship->type - 1);
            new_ship->dir = 'S';
        }
        else {
            new_ship->x1 -= (new_ship->type - 1);
            new_ship->dir = 'N';
        }
    }
    else if (new_ship->dir == 'W') {
        new_ship->y2 = new_ship->y1;
        if (side == 'R') {
            new_ship->x1 -= (new_ship->type - 1);
            new_ship->dir = 'N';
        }
        else {
            new_ship->x1 += (new_ship->type - 1);
            new_ship->dir = 'S';
        }
    }
    else {
        new_ship->x2 = new_ship->x1;
        if (side == 'R') {
            new_ship->y1 -= (new_ship->type - 1);
            new_ship->dir = 'W';
        }
        else {
            new_ship->y1 += (new_ship->type - 1);
            new_ship->dir = 'E';
        }
    }
}

int move_ship(char** board, fleet* player_fleet, char type[3], char side, int ind, char player, board_size cor) {
    delete_ship(board, player_fleet, type, ind, cor);
    int correct = 0;
    if (side == 'F') {
        if (type[0] == 'C' && type[1] == 'A') {
            move(player_fleet->CAR_ships[ind]);
        }
        else if (type[0] == 'B' && type[1] == 'A') {
            move(player_fleet->BAT_ships[ind]);
        }
        else if (type[0] == 'C' && type[1] == 'R') {
            move(player_fleet->CRU_ships[ind]);
        }
        else if (type[0] == 'D' && type[1] == 'E') {
            move(player_fleet->DES_ships[ind]);
        }
    }
    else {
        if (type[0] == 'C' && type[1] == 'A') {
            rotate(player_fleet->CAR_ships[ind], side);
        }
        else if (type[0] == 'B' && type[1] == 'A') {
            rotate(player_fleet->BAT_ships[ind], side);
        }
        else if (type[0] == 'C' && type[1] == 'R') {
            rotate(player_fleet->CRU_ships[ind], side);
        }
        else if (type[0] == 'D' && type[1] == 'E') {
            rotate(player_fleet->DES_ships[ind], side);
        }
    }
    if (type[0] == 'C' && type[1] == 'A') {
        if (move_in_board(*player_fleet->CAR_ships[ind], cor) && check_reef(board, *player_fleet->CAR_ships[ind], cor) && check_surround(board, *player_fleet->CAR_ships[ind], cor) && check_engine(board, *player_fleet->CAR_ships[ind], cor)) {
            check_ship_moves(*player_fleet->CAR_ships[ind], side);
            correct = 1;
        }
    }
    else if (type[0] == 'B' && type[1] == 'A') {
        if (move_in_board(*player_fleet->BAT_ships[ind], cor) && check_reef(board, *player_fleet->BAT_ships[ind], cor) && check_surround(board, *player_fleet->BAT_ships[ind], cor) && check_engine(board, *player_fleet->BAT_ships[ind], cor)) {
            check_ship_moves(*player_fleet->BAT_ships[ind], side);
            correct = 1;
        }
    }
    else if (type[0] == 'C' && type[1] == 'R') {
        if (move_in_board(*player_fleet->CRU_ships[ind], cor) && check_reef(board, *player_fleet->CRU_ships[ind], cor) && check_surround(board, *player_fleet->CRU_ships[ind], cor) && check_engine(board, *player_fleet->CRU_ships[ind], cor)) {
            check_ship_moves(*player_fleet->CRU_ships[ind], side);
            correct = 1;
        }
    }
    else if (type[0] == 'D' && type[1] == 'E') {
        if (move_in_board(*player_fleet->DES_ships[ind], cor) && check_reef(board, *player_fleet->DES_ships[ind], cor) && check_surround(board, *player_fleet->DES_ships[ind], cor) && check_engine(board, *player_fleet->DES_ships[ind], cor)) {
            check_ship_moves(*player_fleet->DES_ships[ind], side);
            correct = 1;
        }
    }
    if (correct) {
        add_ship(board, player_fleet, type, ind, cor);
        if (type[0] == 'C' && type[1] == 'A') {
            update_ship(player_fleet->CAR_ships[ind]);
            player_fleet->CAR_ships[ind]->moves += 1;
        }
        else if (type[0] == 'B' && type[1] == 'A') {
            update_ship(player_fleet->BAT_ships[ind]);
            player_fleet->BAT_ships[ind]->moves += 1;
        }
        else if (type[0] == 'C' && type[1] == 'R') {
            update_ship(player_fleet->CRU_ships[ind]);
            player_fleet->CRU_ships[ind]->moves += 1;
        }
        else if (type[0] == 'D' && type[1] == 'E') {
            update_ship(player_fleet->DES_ships[ind]);
            player_fleet->DES_ships[ind]->moves += 1;
        }
        return 1;
    }
    else {
        if (type[0] == 'C' && type[1] == 'A') {
            if (!check_surround(board, *player_fleet->CAR_ships[ind], cor)) {
                cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
                exit(1);
            }
            else if (!check_engine(board, *player_fleet->CAR_ships[ind], cor)) {
                cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": SHIP CANNOT MOVE" << endl;
                exit(1);
            }
        }
        else if (type[0] == 'B' && type[1] == 'A') {
            if (!check_surround(board, *player_fleet->BAT_ships[ind], cor)) {
                cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
                exit(1);
            }
            else if (!check_engine(board, *player_fleet->BAT_ships[ind], cor)) {
                cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": SHIP CANNOT MOVE" << endl;
                exit(1);
            }
        }
        else if (type[0] == 'C' && type[1] == 'R') {
            if (!check_surround(board, *player_fleet->CRU_ships[ind], cor)) {
                cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
                exit(1);
            }
            else if (!check_engine(board, *player_fleet->CRU_ships[ind], cor)) {
                cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": SHIP CANNOT MOVE" << endl;
                exit(1);
            }
        }
        else if (type[0] == 'D' && type[1] == 'E') {
            if (!check_surround(board, *player_fleet->DES_ships[ind], cor)) {
                cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
                exit(1);
            }
            else if (!check_engine(board, *player_fleet->DES_ships[ind], cor)) {
                cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": SHIP CANNOT MOVE" << endl;
                exit(1);
            }
        }
        cout << "INVALID OPERATION \"MOVE " << ind << ' ' << type << ' ' << side << "\": SHIP WENT FROM BOARD" << endl;
        exit(1);
        return 0;
    }


}

void update_ship(ship* new_ship) {
    if (new_ship->type) {
        if (new_ship->dir == 'N') {
            new_ship->eng[0] = new_ship->x2;
            new_ship->eng[1] = new_ship->y2;
            new_ship->can[0] = new_ship->x1 + 1;
            new_ship->can[1] = new_ship->y1;
        }
        else if (new_ship->dir == 'S') {
            new_ship->eng[0] = new_ship->x2;
            new_ship->eng[1] = new_ship->y2;
            new_ship->can[0] = new_ship->x1 - 1;
            new_ship->can[1] = new_ship->y1;
        }
        else if (new_ship->dir == 'E') {
            new_ship->eng[0] = new_ship->x2;
            new_ship->eng[1] = new_ship->y2;
            new_ship->can[0] = new_ship->x1;
            new_ship->can[1] = new_ship->y1 - 1;

        }
        else if (new_ship->dir == 'W') {
            new_ship->eng[0] = new_ship->x2;
            new_ship->eng[1] = new_ship->y2;
            new_ship->can[0] = new_ship->x1;
            new_ship->can[1] = new_ship->y1 + 1;
        }
    }
}

int check_engine(char** board, ship new_ship, board_size cor) {
    if (board[new_ship.eng[0]][new_ship.eng[1]] == 'x') {
        return 0;
    }
    return 1;
}

int check_ship_moves(ship new_ship, char side) {
    if (new_ship.type == CAR) {
        if (new_ship.moves == 2) {
            cout << "INVALID OPERATION \"" << "MOVE" << " " << new_ship.ind << " " << "CAR" << ' ' << side << "\": SHIP MOVED ALREADY" << endl;
            exit(1);
        }
    }
    else {
        if (new_ship.moves == 3) {
            if (new_ship.type == BAT) {
                cout << "INVALID OPERATION \"" << "MOVE" << " " << new_ship.ind << " " << "BAT" << ' ' << side << "\": SHIP MOVED ALREADY" << endl;
                exit(1);
            }
            else if (new_ship.type == CRU) {
                cout << "INVALID OPERATION \"" << "MOVE" << " " << new_ship.ind << " " << "CRU" << ' ' << side << "\": SHIP MOVED ALREADY" << endl;
                exit(1);
            }
            if (new_ship.type == DES) {
                cout << "INVALID OPERATION \"" << "MOVE" << " " << new_ship.ind << " " << "DES" << ' ' << side << "\": SHIP MOVED ALREADY" << endl;
                exit(1);
            }
        }
    }
    return 1;
}

void erase_moves(ship* new_ship) {
    new_ship->moves = 0;
}

void erase_shots(ship* new_ship) {
    new_ship->shots = 0;
}

int check_surround(char** board, ship new_ship, board_size cor) {
    if (new_ship.dir == 'N') {
        for (int i = new_ship.x1; i <= new_ship.x2; i++) {
            if (i == new_ship.x1) {
                if (!n_head_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
            else if (i == new_ship.x2) {
                if (!n_back_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
            else {
                if (!n_s_side_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }

        }
    }
    else if (new_ship.dir == 'W') {
        for (int i = new_ship.y1; i <= new_ship.y2; i++) {
            if (i == new_ship.y1) {
                if (!w_head_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
            else if (i == new_ship.y2) {
                if (!w_back_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
            else {
                if (!w_e_side_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
        }
    }
    else if (new_ship.dir == 'E') {
        for (int i = new_ship.y1; i >= new_ship.y2; i--) {
            if (i == new_ship.y1) {
                if (!w_back_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
            else if (i == new_ship.y2) {
                if (!w_head_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
            else {
                if (!w_e_side_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
        }
    }
    else if (new_ship.dir == 'S') {
        for (int i = new_ship.x1; i >= new_ship.x2; i--) {
            if (i == new_ship.x1) {
                if (!n_back_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
            else if (i == new_ship.x2) {
                if (!n_head_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
            else {
                if (!n_s_side_check(board, new_ship, i, cor)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int n_head_check(char** board, ship new_ship, int i, board_size cor) {
    if (i != 0 && new_ship.y1 != 0 && new_ship.y1 != cor.MAX_Y - 1) {
        if (board[i - 1][new_ship.y1] == '+' || board[i][new_ship.y1 - 1] == '+' || board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == 0 && new_ship.y1 == 0) {
        if (board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == 0 && new_ship.y1 == cor.MAX_Y - 1) {
        if (board[i][new_ship.y1 - 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i != 0 && new_ship.y1 == 0) {
        if (board[i - 1][new_ship.y1] == '+' || board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i != 0 && new_ship.y1 == cor.MAX_Y - 1) {
        if (board[i - 1][new_ship.y1] == '+' || board[i][new_ship.y1 - 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == 0 && new_ship.y1 != 0 && new_ship.y1 != cor.MAX_Y - 1) {
        if (board[i][new_ship.y1 - 1] == '+' || board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
}

int n_back_check(char** board, ship new_ship, int i, board_size cor) {
    if (i != cor.MAX_X - 1 && new_ship.y1 != 0 && new_ship.y1 != cor.MAX_Y - 1) {
        if (board[i + 1][new_ship.y1] == '+' || board[i][new_ship.y1 - 1] == '+' || board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == cor.MAX_X - 1 && new_ship.y1 == 0) {
        if (board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == cor.MAX_X - 1 && new_ship.y1 == cor.MAX_Y - 1) {
        if (board[i][new_ship.y1 - 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i != cor.MAX_X - 1 && new_ship.y1 == 0) {
        if (board[i + 1][new_ship.y1] == '+' || board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i != cor.MAX_X - 1 && new_ship.y1 == cor.MAX_Y - 1) {
        if (board[i + 1][new_ship.y1] == '+' || board[i][new_ship.y1 - 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == cor.MAX_X - 1 && new_ship.y1 != 0 && new_ship.y1 != cor.MAX_Y - 1) {
        if (board[i][new_ship.y1 - 1] == '+' || board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
}

int n_s_side_check(char** board, ship new_ship, int i, board_size cor) {
    if (new_ship.y1 != 0 && new_ship.y1 != cor.MAX_Y - 1) {
        if (board[i][new_ship.y1 - 1] == '+' || board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (new_ship.y1 == 0) {
        if (board[i][new_ship.y1 + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (new_ship.y1 == cor.MAX_Y - 1) {
        if (board[i][new_ship.y1 - 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
}

int w_head_check(char** board, ship new_ship, int i, board_size cor) {
    if (i != 0 && new_ship.x1 != 0 && new_ship.x1 != cor.MAX_X - 1) {
        if (board[new_ship.x1 - 1][i] == '+' || board[new_ship.x1 + 1][i] == '+' || board[new_ship.x1][i - 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i != 0 && new_ship.x1 == 0) {
        if (board[new_ship.x1][i - 1] == '+' || board[new_ship.x1 + 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i != 0 && new_ship.x1 == cor.MAX_X - 1) {
        if (board[new_ship.x1][i - 1] == '+' || board[new_ship.x1 - 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == 0 && new_ship.x1 == cor.MAX_X - 1) {
        if (board[new_ship.x1 - 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == 0 && new_ship.x1 == 0) {
        if (board[new_ship.x1 + 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == 0 && new_ship.x1 != 0 && new_ship.x1 != cor.MAX_X - 1) {
        if (board[new_ship.x1 - 1][i] == '+' || board[new_ship.x1 + 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
}

int w_back_check(char** board, ship new_ship, int i, board_size cor) {
    if (i != cor.MAX_Y - 1 && new_ship.x1 != 0 && new_ship.x1 != cor.MAX_X - 1) {
        if (board[new_ship.x1 - 1][i] == '+' || board[new_ship.x1 + 1][i] == '+' || board[new_ship.x1][i + 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i != cor.MAX_Y - 1 && new_ship.x1 == 0) {
        if (board[new_ship.x1][i + 1] == '+' || board[new_ship.x1 + 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i != cor.MAX_Y - 1 && new_ship.x1 == cor.MAX_X - 1) {
        if (board[new_ship.x1][i + 1] == '+' || board[new_ship.x1 - 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == cor.MAX_Y - 1 && new_ship.x1 == cor.MAX_X - 1) {
        if (board[new_ship.x1 - 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == 0 && new_ship.x1 == 0) {
        if (board[new_ship.x1 + 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (i == cor.MAX_Y - 1 && new_ship.x1 != 0 && new_ship.x1 != cor.MAX_X - 1) {
        if (board[new_ship.x1 - 1][i] == '+' || board[new_ship.x1 + 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
}

int w_e_side_check(char** board, ship new_ship, int i, board_size cor) {
    if (new_ship.x1 != 0 && new_ship.x1 != cor.MAX_X - 1) {
        if (board[new_ship.x1 - 1][i] == '+' || board[new_ship.x1 + 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (new_ship.x1 == 0) {
        if (board[new_ship.x1 + 1][i] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (new_ship.x1 == cor.MAX_X - 1) {
        if (board[new_ship.x1][i - 1] == '+') {
            return 0;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
}

int check_in_board(ship new_ship, char player, init player_pos) {
    int min_y = player_pos.x1, max_y = player_pos.x2, min_x = player_pos.y1, max_x = player_pos.y2;
    if (new_ship.x1 <= max_y && new_ship.x1 >= min_y && new_ship.y1 <= max_x && new_ship.y1 >= min_x && new_ship.x2 >= min_x && new_ship.x2 <= max_y && new_ship.x2 >= min_y && new_ship.y2 <= max_x && new_ship.y2 >= min_x) {
        return 1;
    }
    else {
        return 0;
    }
}

int move_in_board(ship new_ship, board_size cor) {
    int min_y = 0, max_y = cor.MAX_X - 1, min_x = 0, max_x = cor.MAX_Y;
    if (new_ship.x1 <= max_y && new_ship.x1 >= min_y && new_ship.y1 <= max_x && new_ship.y1 >= min_x && new_ship.x2 >= min_x && new_ship.x2 <= max_y && new_ship.x2 >= min_y && new_ship.y2 <= max_x && new_ship.y2 >= min_x) {
        return 1;
    }
    else {
        return 0;
    }
}

int check_reef(char** board, ship new_ship, board_size cor) {
    if (new_ship.dir == 'N') {
        for (int i = new_ship.x1; i <= new_ship.x2; i++) {
            if (board[i][new_ship.y1] == '#') {
                return 0;
            }
        }
    }
    else if (new_ship.dir == 'W') {
        for (int i = new_ship.y1; i <= new_ship.y2; i++) {
            if (board[new_ship.x1][i] == '#') {
                return 0;
            }
        }
    }
    else if (new_ship.dir == 'E') {
        for (int i = new_ship.y1; i >= new_ship.y2; i--) {
            if (board[new_ship.x1][i] == '#') {
                return 0;
            }
        }
    }
    else if (new_ship.dir == 'S') {
        for (int i = new_ship.x1; i >= new_ship.x2; i--) {
            if (board[i][new_ship.y1] == '#') {
                return 0;
            }
        }
    }
    else if (!move_in_board(new_ship, cor)) {
        return 0;
    }
    return 1;
}

void set_ship(char** board, char player, ship new_ship, char command[15], int* inv, fleet player_fleet, init player_pos, board_size cor) {
    bool fleet_check;
    char type[3];

    if (new_ship.type == 5) {
        type[0] = 'C';
        type[1] = 'A';
        type[2] = 'R';
        fleet_check = player_fleet.CARs[new_ship.ind];
    }
    else if (new_ship.type == 4) {
        type[0] = 'B';
        type[1] = 'A';
        type[2] = 'T';
        fleet_check = player_fleet.BATs[new_ship.ind];
    }
    else if (new_ship.type == 3) {
        type[0] = 'C';
        type[1] = 'R';
        type[2] = 'U';
        fleet_check = player_fleet.CRUs[new_ship.ind];
    }
    else if (new_ship.type == 2) {
        type[0] = 'D';
        type[1] = 'E';
        type[2] = 'S';
        fleet_check = player_fleet.DESs[new_ship.ind];
    }
    if (check_in_board(new_ship, player, player_pos) && check_surround(board, new_ship, cor) && new_ship.ind < new_ship.amount && !fleet_check) {

        if (check_reef(board, new_ship, cor)) {
            int count = 0;
            if (new_ship.dir == 'N') {
                for (int i = new_ship.x1; i <= new_ship.x2; i++) {
                    if (new_ship.pattern[count] == 1) {
                        board[i][new_ship.y1] = '+';
                    }
                    else if (new_ship.pattern[count] == 0) {
                        board[i][new_ship.y1] = 'x';
                    }
                    else {
                        board[i][new_ship.y1] = 'O';
                    }
                    count += 1;
                }
            }
            else if (new_ship.dir == 'W') {
                for (int i = new_ship.y1; i <= new_ship.y2; i++) {
                    if (new_ship.pattern[count] == 1) {
                        board[new_ship.x1][i] = '+';
                    }
                    else if (new_ship.pattern[count] == 0) {
                        board[new_ship.x1][i] = 'x';
                    }
                    else {
                        board[new_ship.x1][i] = 'O';
                    }
                    count += 1;
                }
            }
            else if (new_ship.dir == 'E') {
                for (int i = new_ship.y1; i >= new_ship.y2; i--) {
                    if (new_ship.pattern[count] == 1) {
                        board[new_ship.x1][i] = '+';
                    }
                    else if (new_ship.pattern[count] == 0) {
                        board[new_ship.x1][i] = 'x';
                    }
                    else {
                        board[new_ship.x1][i] = 'O';
                    }
                    count += 1;
                }
            }
            else if (new_ship.dir == 'S') {
                for (int i = new_ship.x1; i >= new_ship.x2; i--) {
                    if (new_ship.pattern[count] == 1) {
                        board[i][new_ship.y1] = '+';
                    }
                    else if (new_ship.pattern[count] == 0) {
                        board[i][new_ship.y1] = 'x';
                    }
                    else {
                        board[i][new_ship.y1] = 'O';
                    }
                    count += 1;
                }
            }
        }
        else {
            if (new_ship.pattern_set == 1) {
                cout << "INVALID OPERATION \"" << command << " " << player << ' ' << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << ' ' << new_ship.int_pattern << "\": PLACING SHIP ON REEF" << endl;
                exit(1);
            }
            else {
                cout << "INVALID OPERATION \"" << command << " " << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << "\": PLACING SHIP ON REEF" << endl;
                exit(1);
            }

        }
    }
    else {
        if (new_ship.ind >= new_ship.amount) {
            if (new_ship.pattern_set == 1) {
                cout << "INVALID OPERATION \"" << command << " " << player << ' ' << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << ' ' << new_ship.int_pattern << "\": ALL SHIPS OF THE CLASS ALREADY SET" << endl;
                exit(1);
            }
            else
            {
                cout << "INVALID OPERATION \"" << command << " " << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << "\": ALL SHIPS OF THE CLASS ALREADY SET" << endl;
                exit(1);
            }

        }
        if (fleet_check) {
            if (new_ship.pattern_set == 1) {
                cout << "INVALID OPERATION \"" << command << " " << player << ' ' << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << ' ' << new_ship.int_pattern << "\": SHIP ALREADY PRESENT" << endl;
                exit(1);
            }
            else
            {
                cout << "INVALID OPERATION \"" << command << " " << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << "\": SHIP ALREADY PRESENT" << endl;
                exit(1);
            }

        }
        if (!check_surround(board, new_ship, cor)) {
            if (new_ship.pattern_set == 1) {
                cout << "INVALID OPERATION \"" << command << " " << player << ' ' << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << ' ' << new_ship.int_pattern << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
                exit(1);
            }
            else
            {
                cout << "INVALID OPERATION \"" << command << " " << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
                exit(1);
            }

        }
        else {
            if (new_ship.pattern_set == 1) {
                cout << "INVALID OPERATION \"" << command << " " << player << ' ' << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << ' ' << new_ship.int_pattern << "\": NOT IN STARTING POSITION" << endl;
                exit(1);
            }
            else
            {
                cout << "INVALID OPERATION \"" << command << " " << new_ship.x1 << " " << new_ship.y1 << " " << new_ship.dir << " " << new_ship.ind << " " << type[0] << type[1] << type[2] << "\": NOT IN STARTING POSITION" << endl;
                exit(1);
            }
        }
    }
}