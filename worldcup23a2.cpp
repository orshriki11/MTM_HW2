#include "worldcup23a2.h"

world_cup_t::world_cup_t() : numberOfActiveTeams(0), numberOfAllTeams(0) {
}

world_cup_t::~world_cup_t() {
    playersHash.deletePtrElements();
}

StatusType world_cup_t::add_team(int teamId) {

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
    if(teamsTreeById.find(teamId, &found_team) != AVL_TREE_SUCCESS)
        return StatusType::FAILURE;
    //assert(res_by_id == AVL_TREE_SUCCESS);
    AVLTreeResult res_by_ability = teamsTreeByAbility.remove(*found_team);
    AVLTreeResult removed_by_id = teamsTreeById.remove(teamId);
    //assert(res_by_ability == AVL_TREE_SUCCESS && removed_by_id == AVL_TREE_SUCCESS);

    if(res_by_ability != AVL_TREE_SUCCESS || removed_by_id != AVL_TREE_SUCCESS)
        return StatusType::ALLOCATION_ERROR;
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
        auto* player_node = new UnionFindNode<std::shared_ptr<Team>, std::shared_ptr<Player>>(new_player);
        if(team_of_Player->isNew)
        {
            player_node->master = team_of_Player;
            player_node->data->partialSpirit = team_of_Player->teamSpirit * spirit;

            team_of_Player->UF_Team = player_node;

            playersHash.insert(playerId,team_of_Player->UF_Team);
            team_of_Player->isNew = false;
        }
        else
        {
            team_of_Player->UF_Team->insert(player_node);
            player_node->data->partialSpirit = team_of_Player->teamSpirit * spirit;
            player_node->linkSpirit = (player_node->parent->spirit_whenBought).inv() * team_of_Player->teamSpirit;
            player_node->link_gamesPlayed = team_of_Player->UF_Team->gamesPlayed_whenBought;

            playersHash.insert(playerId,player_node);

        }


    }
    catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }
    teamsTreeByAbility.remove(*team_of_Player);
    team_of_Player->gksCount += goalKeeper;
    team_of_Player->playersCount++;
    team_of_Player->totalAbility += ability;
    team_of_Player->totalCards += cards;
    team_of_Player->teamSpirit = team_of_Player->teamSpirit * spirit;
    teamsTreeByAbility.insert(*team_of_Player, team_of_Player);

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
    auto player_node_ptr = playersHash.search(playerId);
    if(player_node_ptr == nullptr)
    {
        return StatusType::FAILURE;
    }
    int gamesFromUF = (*player_node_ptr)->FindGamesPlayed();

    return gamesFromUF + (*player_node_ptr)->data->gamesPlayed;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards) {
    if (playerId <= 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }

    auto player_node_ptr = playersHash.search(playerId);
    if (player_node_ptr == nullptr) {
        return StatusType::FAILURE;
    }


    std::shared_ptr<Team> team = (*player_node_ptr)->Find();
    if(team->isRemoved) {
        return StatusType::FAILURE;
    }

    team->totalCards += cards;
    (*player_node_ptr)->data->cards += cards;

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId) {
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto player_node_ptr = playersHash.search(playerId);
    if (player_node_ptr == nullptr) {
        return StatusType::FAILURE;
    }
    int cards = (*player_node_ptr)->data->cards;
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
    if(teamsTreeByAbility.get_ith_ranked_element(i + 1, &team) != AVL_TREE_SUCCESS)
    {
        return StatusType::FAILURE;
    }

    return team->teamId;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId) {
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto player_node_ptr = playersHash.search(playerId);
    if(player_node_ptr == nullptr) {
        return StatusType::FAILURE;
    }
    permutation_t linksSpirit = permutation_t::neutral();
    permutation_t rootSpirit = permutation_t::neutral();
    std::shared_ptr<Team> team = (*player_node_ptr)->FindWithSpirit(&linksSpirit,&rootSpirit);
    if(team->isRemoved) {
        return StatusType::FAILURE;
    }

    permutation_t playerSpirit = (*player_node_ptr)->data->spirit;
    permutation_t spirit_sum = rootSpirit * (linksSpirit * playerSpirit);
    return spirit_sum;
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<Team> team1;
    std::shared_ptr<Team> team2;
    AVLTreeResult res1 = teamsTreeById.find(teamId1, &team1);
    AVLTreeResult res2 = teamsTreeById.find(teamId2, &team2);
    if (res1 != AVL_TREE_SUCCESS || res2 != AVL_TREE_SUCCESS) {
        return StatusType::FAILURE;
    }

    if(teamsTreeByAbility.remove(*team1) != AVL_TREE_SUCCESS)
    {
        return StatusType::FAILURE;
    }
    team1->points += team2->points;
    permutation_t forLink = team1->teamSpirit;
    //Team2->UF_Team->gamesPlayed_whenBought = Team2->gamesPlayed;
    team1->teamSpirit = team1->teamSpirit * team2->teamSpirit;
    team1->totalAbility += team2->totalAbility;
    team1->gksCount += team2->gksCount;
    team1->totalCards += team2->totalCards;
    team1->playersCount += team2->playersCount;

    if(team1->isNew && !team2->isNew)
    {
        team1->UF_Team = team2->UF_Team;
        team1->isNew = false;
        team2->UF_Team->master = team1;
        //team2->UF_Team->initNode = false;
    }
    else if(!team1->isNew && !team2->isNew)
    {
        if(team2->UF_Team->size > team1->UF_Team->size)
        {
            int gamesTeam2 = team2->UF_Team->gamesPlayed_whenBought;
            team1->UF_Team->Unite(team2->UF_Team);
            team2->UF_Team->spirit_whenBought = forLink * team2->UF_Team->spirit_whenBought;
            team1->UF_Team->linkSpirit = team2->UF_Team->spirit_whenBought.inv();
            team1->UF_Team->link_gamesPlayed = gamesTeam2;
        }

        else
        {
            team1->UF_Team->Unite(team2->UF_Team);
            team2->UF_Team->link_gamesPlayed = team1->UF_Team->gamesPlayed_whenBought;
            team2->UF_Team->linkSpirit = (team1->UF_Team->spirit_whenBought).inv() * forLink;
        }
        team1->UF_Team = team1->UF_Team->FindRootOnly();


    }


    if(remove_team(teamId2) != StatusType::SUCCESS)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    if(teamsTreeByAbility.insert(*team1,team1) != AVLTreeResult::AVL_TREE_SUCCESS)
        return StatusType::ALLOCATION_ERROR;


    return StatusType::SUCCESS;
}
