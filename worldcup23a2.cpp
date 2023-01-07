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
    if(playersHash.search(playerId) != nullptr){
        return StatusType::FAILURE;
    }

    std::shared_ptr<Team> team_of_Player;
    teamsTreeById.find(teamId, &team_of_Player);

    try {
        std::shared_ptr<Player> new_player = std::make_shared<Player>(playerId, teamId, spirit, gamesPlayed, ability, cards, goalKeeper);
        UnionFindNode<std::shared_ptr<Team>, std::shared_ptr<Player>>* player_node = new UnionFindNode<std::shared_ptr<Team>, std::shared_ptr<Player>>(new_player);

        if(team_of_Player->isNew)
        {
            if(teamsHash.search(teamId) != nullptr){
                teamsHash.remove(teamId);
            }
            teamsHash.insert(teamId,team_of_Player);

            team_of_Player->UF_Team = player_node;
            player_node->master = team_of_Player;
            team_of_Player->isNew = false;
        }
        else
        {
            assert(team_of_Player->UF_Team != nullptr);
            team_of_Player->UF_Team->insert(player_node);
        }

        player_node->data->partialSpirit = team_of_Player->teamSpirit * spirit;
        player_node->linkSpirit = team_of_Player->teamSpirit;
        player_node->link_gamesPlayed = team_of_Player->gamesPlayed;

        //TODO make sure this works fine and doesnt lead to mem leaks
        playersHash.insert(playerId,*player_node);
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

    std::shared_ptr<Team> team1;
    std::shared_ptr<Team> team2;
    AVLTreeResult res1 = teamsTreeById.find(teamId1, &team1);
    AVLTreeResult res2 = teamsTreeById.find(teamId2, &team2);
    if (res1 != AVL_TREE_SUCCESS || res2 != AVL_TREE_SUCCESS || team1->gksCount < 1 || team2->gksCount < 1) {
        return StatusType::FAILURE;
    }

    int resultState;
    int team1_score = team1->totalAbility + team1->points;
    int team2_score = team2->totalAbility + team2->points;
    if (team1_score > team2_score) {
        team1->points += 3;
        resultState = 1;
    } else if (team2_score > team1_score) {
        team2->points += 3;
        resultState = 3;
    } else {
        int team1_strength = team1->teamSpirit.strength();
        int team2_strength = team2->teamSpirit.strength();
        if (team1_strength > team2_strength) {
            team1->points += 3;
            resultState = 2;
        } else if (team1_strength < team2_strength) {
            team2->points += 3;
            resultState = 4;
        } else {
            team1->points++;
            team2->points++;
            resultState = 0;
        }
    }

    team1->gamesPlayed++;
    team1->UF_Team->gamesPlayed_whenBought++;
    team2->gamesPlayed++;
    team2->UF_Team->gamesPlayed_whenBought++;

    return resultState;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId) {

    if(playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    UnionFindNode<std::shared_ptr<Team>, std::shared_ptr<Player>> *player_node = playersHash.search(playerId);
    if(player_node == nullptr)
    {
        return StatusType::FAILURE;
    }
    int gamesFromUF = playersHash.search(playerId)->FindGamesPlayed();
    //FIXME why is this unused? what is the logic?
    int gamesTotalPlayed = player_node->Find()->gamesPlayed;

    return gamesFromUF + player_node->data->gamesPlayed;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards) {
    if (playerId <= 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }
    UnionFindNode<std::shared_ptr<Team>, std::shared_ptr<Player>> *player_node = playersHash.search(playerId);
    if (player_node == nullptr) {
        return StatusType::FAILURE;
    }

    std::shared_ptr<Team> team = player_node->Find();
    if(team->isRemoved) {
        return StatusType::FAILURE;
    }

    team->totalCards += cards;
    player_node->data->cards += cards;

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId) {
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    UnionFindNode<std::shared_ptr<Team>, std::shared_ptr<Player>> *player_node = playersHash.search(playerId);
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

    std::shared_ptr<Team> team;
    AVLTreeResult res = teamsTreeById.find(teamId, &team);

    if (res != AVL_TREE_SUCCESS) {
        return StatusType::FAILURE;
    }

    return team->points;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i) {
    if (i < 0 || i >= numberOfActiveTeams) {
        return StatusType::FAILURE;
    }

    std::shared_ptr<Team> team;
    if(teamsTreeByAbility.get_ith_ranked_element(i, &team) != AVL_TREE_SUCCESS)
    {
        return StatusType::FAILURE;
    }

    // TODO: Your code goes here
    return team->teamId;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId) {
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    UnionFindNode<Team*,Player*> *playerNode = playersHash.search(playerId);
    if(playerNode == nullptr)
        return  StatusType::FAILURE;


    permutation_t playerSpirit = playerNode->data->spirit;

    playerNode->FindSpiritLinks(playerSpirit);

    // TODO: Need to figure out how to accumulate spirit from continuous bought teams.
    return permutation_t(playerSpirit);
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<Team> Team1 = *teamsHash.search(teamId1);
    std::shared_ptr<Team> Team2 = *teamsHash.search(teamId2);

    if (Team1 == nullptr || Team2 == nullptr) {
        return StatusType::FAILURE;
    }


    Team1->points += Team2->points;
    Team2->UF_Team->linkSpirit = Team1->teamSpirit;
    Team2->UF_Team->link_gamesPlayed = Team1->gamesPlayed;
    //Team2->UF_Team->gamesPlayed_whenBought = Team2->gamesPlayed;
    Team1->teamSpirit = Team2->teamSpirit * Team1->teamSpirit;
    Team1->totalAbility += Team2->totalAbility;
    Team1->gksCount += Team2->gksCount;
    Team1->totalCards += Team2->totalCards;
    Team1->playersCount += Team2->playersCount;
    Team1->UF_Team->Unite(Team2->UF_Team);
    Team2->teamId = -1;

    teamsHash.remove(teamId2);






    return StatusType::SUCCESS;
}
