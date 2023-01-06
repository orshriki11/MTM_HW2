#include "worldcup23a2.h"

world_cup_t::world_cup_t() : numberOfActiveTeams(0), numberOfAllTeams(0) {
}

StatusType world_cup_t::add_team(int teamId) {
    //TODO: Add team to HashTable and link it with it's UF.
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (teamsTreeById.contains(teamId)) {
        return StatusType::FAILURE;
    }

    try {
        std::shared_ptr<Team> new_team = std::make_shared<Team>(teamId);
        teamsTreeById.insert(teamId, new_team);
        teamsTreeByAbility.insert(*new_team, new_team);
    }
    catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }

    numberOfActiveTeams++;
    numberOfAllTeams++;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId) {

    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if(!teamsTreeById.contains(teamId)){
        return StatusType::FAILURE;
    }

    std::shared_ptr<Team> found_team;
    AVLTreeResult res_by_id = teamsTreeById.find(teamId, &found_team);
    assert(res_by_id == AVL_TREE_SUCCESS);
    AVLTreeResult res_by_ability = teamsTreeByAbility.remove(*found_team);
    AVLTreeResult removed_by_id = teamsTreeById.remove(teamId);
    assert(res_by_ability == AVL_TREE_SUCCESS && removed_by_id == AVL_TREE_SUCCESS);

    found_team->isRemoved = true;

    numberOfActiveTeams--;

    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper) {

    if (playerId <= 0 || teamId <= 0 || !spirit.isvalid() || gamesPlayed < 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }
//    if (gamesPlayed == 0 && (cards > 0)) {
//        return StatusType::INVALID_INPUT;
//    }
    if(!teamsTreeById.contains(teamId)){
        return StatusType::FAILURE;
    }

    if(!playersHash.search(playerId)){
        return StatusType::FAILURE;
    }
    Team *team_of_Player = teamsHash.search(teamId);
    //UnionFindNode<Team*,Player*>* player_node = playersHash.search(playerId);
    if (playersHash.search(playerId) != nullptr || team_of_Player == nullptr) {
        return StatusType::FAILURE;
    }

    try {
        Player *new_player = new Player(playerId, teamId, spirit, gamesPlayed, ability, cards, goalKeeper);


        UnionFindNode<Team *, Player *> *player_node = new UnionFindNode<Team *, Player *>(new_player);
        team_of_Player->UF_Team->insert(player_node);
        player_node->data->partialSpirit = team_of_Player->teamSpirit * spirit;
        player_node->linkSpirit = player_node->data->partialSpirit;
        player_node->data->teamGamesPlayed_preAdd = team_of_Player->gamesPlayed;
    }
    catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }
    // TODO: Add Player as a UpTreeNode and link pointer to hashtable and UF
    team_of_Player->gksCount += goalKeeper;
    team_of_Player->playersCount++;
    team_of_Player->totalAbility += ability;
    team_of_Player->totalCards += cards;
    team_of_Player->teamSpirit = team_of_Player->teamSpirit * spirit;

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }
    Team *Team1 = teamsHash.search(teamId1);
    Team *Team2 = teamsHash.search(teamId1);
    if (Team1 == nullptr || Team2 == nullptr) {
        return StatusType::FAILURE;
    }
    int team1_score = Team1->totalAbility + Team1->points;
    int team2_score = Team2->totalAbility + Team2->points;
    if (team1_score > team2_score) {
        Team1->points += 3;
    } else if (team2_score > team1_score) {
        Team2->points += 3;
    } else if (team2_score == team1_score) {
        int team1_strength = Team1->teamSpirit.strength();
        int team2_strength = Team2->teamSpirit.strength();
        if (team1_strength > team2_strength) {
            Team1->points += 3;
        } else if (team1_strength < team2_strength) {
            Team2->points += 3;
        } else {
            Team1->points++;
            Team2->points++;
        }
    }

    Team1->gamesPlayed++;
    Team2->gamesPlayed++;
    // TODO: Your code goes here
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId) {

    // TODO: Correctly Fetch player data
    UnionFindNode<Team *, Player *> *player_node = playersHash.search(playerId);
    Player *player = player_node->data;


    //int played_games = player->gamesPlayed +
    //int gamesPlayed = player.gamesPlayed + (player.teamP.lock()->gamesPlayed - player.teamGamesPlayed_preAdd);
    return 22;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards) {
    if (playerId <= 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }
    UnionFindNode<Team *, Player *> *player_node = playersHash.search(playerId);
    if (player_node == nullptr) {
        return StatusType::FAILURE;
    }
    Team *team = player_node->Find();

    team->totalCards += cards;
    player_node->data->cards += cards;

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId) {
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    UnionFindNode<Team *, Player *> *player_node = playersHash.search(playerId);
    if (player_node == nullptr) {
        return StatusType::FAILURE;
    }

    int cards = player_node->data->cards;
    return cards;
}

output_t<int> world_cup_t::get_team_points(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    // TODO: Correctly Fetch Team data
    Team *team = teamsHash.search(teamId);

    if (team == nullptr) {
        return StatusType::FAILURE;
    }

    int points = team->points;
    return points;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i) {
    if (i < 0 || i >= numberOfActiveTeams) {
        return StatusType::FAILURE;
    }


    // TODO: Your code goes here
    return 12345;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId) {
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    // TODO: Need to figure out how to accumulate spirit from continuous bought teams.
    //return permutation_t();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    Team *Team1 = teamsHash.search(teamId1);
    Team *Team2 = teamsHash.search(teamId2);

    if (Team1 == nullptr || Team2 == nullptr) {
        return StatusType::FAILURE;
    }

    Team1->points += Team2->points;
    Team1->teamSpirit = Team2->teamSpirit * Team1->teamSpirit;
    Team1->totalAbility += Team2->totalAbility;
    Team1->gksCount += Team2->gksCount;
    Team1->totalCards += Team2->totalCards;
    Team1->playersCount += Team2->playersCount;
    Team1->UF_Team->Unite(Team2->UF_Team);





    // TODO: Need to properly modify player's part_spirit when teams are bought continuesly.
    return StatusType::SUCCESS;
}
