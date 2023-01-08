//
// Created by hilla on 1/7/2023.
//
#include "worldcup23a2.h"
#include <string>
#include <iostream>

using namespace std;

// Helpers
static const char *StatusTypeStr[] =
        {
                "SUCCESS",
                "ALLOCATION_ERROR",
                "INVALID_INPUT",
                "FAILURE"
        };

void print(string cmd, StatusType res) {
    cout << cmd << ": " << StatusTypeStr[(int) res] << endl;
}

template<typename T>
void print(string cmd, output_t<T> res) {
    if (res.status() == StatusType::SUCCESS) {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << ", " << res.ans() << endl;
    } else {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
    }
}

int main() {
    world_cup_t wc;

    print("add_team", wc.add_team(1));
    print("add_team", wc.add_team(2));
    print("add_team", wc.add_team(3));
    print("remove_team", wc.remove_team(3));
    permutation_t p1 = permutation_t::read("2,3,4,5,1");
    permutation_t p2 = permutation_t::read("4,3,5,2,1");
    print("add_player", wc.add_player(1001, 1, p1, 0, 15, 2, true));
    print("add_player", wc.add_player(2001, 2, p2, 0, 17, 1, true));
    print("play_match", wc.play_match(1, 2));
    print("num_played_games_for_player", wc.num_played_games_for_player(1001));
    print("add_player_cards", wc.add_player_cards(2001, 2));
    print("get_player_cards",wc.get_player_cards(2001));
    print("get_team_points" ,wc.get_team_points(1));
    print("get_ith_pointless_ability", wc.get_ith_pointless_ability(0));
    print("get_partial_spirit" ,wc.get_partial_spirit(1001));
    print("buy_team" ,wc.buy_team(1, 2));

    return 0;
};