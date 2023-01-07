//
// Created by hilla on 1/7/2023.
//
#include "worldcup23a2.h"

int main(){
    world_cup_t wc;
    wc.add_team(1);
    wc.add_team(2);
    wc.add_team(3);
    wc.remove_team(3);
    int p1[5] = {2,3,4,5,1};
    int p2[5] = {4,3,5,2,1};
    wc.add_player(1001, 1, p1, 0, 15, 2, true);
    wc.add_player(2001, 2, p2, 0, 17, 1, true);
    wc.play_match(1, 2);
    wc.num_played_games_for_player(1001);
    wc.add_player_cards(2001, 2);
    wc.get_player_cards(2001);
    wc.get_team_points(1);
    output_t<int> res = wc.get_ith_pointless_ability(0);
    wc.get_partial_spirit(1001);
    wc.buy_team(1, 2);

    return 0;
};