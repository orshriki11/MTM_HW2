#include "worldcup23a2.h"


world_cup_t::world_cup_t()
{
	// TODO: Your code goes here
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> new_Team(new (std::nothrow) Team(teamId));
    if(!new_Team)
    {
        return StatusType::ALLOCATION_ERROR;
    }
/*    try
    {
        if(TeamsTree.insert(teamId, new_Team) != AVLTreeResult::AVL_TREE_SUCCESS)
        {
            return StatusType::FAILURE;
        }
    }*/
    catch (const std::bad_alloc &) {
            return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> team_to_remove;
    /*if (TeamsTree.find(teamId, &team_to_remove) != AVLTreeResult::AVL_TREE_SUCCESS) {
        return StatusType::FAILURE;
    }*/
    if (team_to_remove->playersCount == 0) {
        /*if (TeamsTree.remove(teamId) != AVLTreeResult::AVL_TREE_SUCCESS) {
            return StatusType::FAILURE;
        }*/
        return StatusType::SUCCESS;
    }
    return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if (playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }
    if (gamesPlayed == 0 && (cards > 0)) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> team_of_player;
    std::shared_ptr<Player> new_player_ptr;









	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    // TODO: Correctly Fetch player data
	Player player = PLAYARDATABASE.find(playerId);
    int gamesPlayed = player.gamesPlayed + (player.teamP.lock()->gamesPlayed - player.teamGamesPlayed_preAdd);
	return 22;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId <= 0 || cards < 0)
        return StatusType::INVALID_INPUT;
    }
    // TODO: Correctly Fetch player data
    Player player = PLAYARDATABASE.find(playerId);
    player.cards += cards;
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId <= 0)
        return StatusType::INVALID_INPUT;
    }
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <= 0)
        return StatusType::INVALID_INPUT;

    // TODO: Correctly Fetch Team data
    Team team = TEAMDATABASE.find(teamId);
    int points = team.points;
	// TODO: Your code goes here
	return 30003;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    if(i < 0 || i >= number_of_teams )
    {
        return StatusType::FAILURE;
    }

	// TODO: Your code goes here
	return 12345;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId <= 0)
        return StatusType::INVALID_INPUT;
    }
	// TODO: Your code goes here
	return permutation_t();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0)
        return StatusType::INVALID_INPUT;
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
