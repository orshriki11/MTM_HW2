#include "worldcup23a2.h"

world_cup_t::world_cup_t() : teamsHash(), playersHash(), number_of_teams(0), number_of_all_teams(0)
{
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId)
{
    //TODO: Add team to HashTable and link it with it's UF.
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if(teamsHash.search(teamId) != NULL)
    {
        return StatusType::FAILURE;
    }

    Team* new_Team(new (std::nothrow) Team(teamId));
    if(!new_Team)
    {
        return StatusType::ALLOCATION_ERROR;
    }

    try
    {
        UnionFind<Team*,Player*> new_team_UF = UnionFind<Team*,Player*>(new_Team);
        teamsHash.insert(teamId, new_team_UF);
        //new_team_UF.getMaster();
    }
    catch (const std::bad_alloc &) {
            return StatusType::ALLOCATION_ERROR;
    }

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{

    // TODO: removing team from hash table and raising a 'team removed' flag in team.
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    UnionFind<Team*,Player*> *team_UF = teamsHash.search(teamId);
    if(team_UF != nullptr)
    {
        return StatusType::FAILURE;
    }
    team_UF->getMaster()->isRemoved = true;
    team_UF->getMaster()->teamId = -1;
    teamsHash.remove(teamId);
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

    UnionFind<Team*,Player*>* UF_of_Player = teamsHash.search(teamId);
    if(playersHash.search(playerId) != nullptr || UF_of_Player == nullptr)
    {
        return StatusType::FAILURE;
    }

    try{
        Player* new_player = new Player(playerId,teamId,spirit,gamesPlayed,ability,cards,goalKeeper);

        UpTreeNode<Player*> *player_node = new UpTreeNode<Player*>(new_player);
        UF_of_Player->insert(player_node);
    }
    catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }
    // TODO: Add Player as a UpTreeNode and link pointer to hashtable and UF


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
	UpTreeNode<Player*> *player_node = playersHash.search(playerId);
    Player* player = player_node->data;
    //int played_games = player->gamesPlayed +
    //int gamesPlayed = player.gamesPlayed + (player.teamP.lock()->gamesPlayed - player.teamGamesPlayed_preAdd);
	return 22;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId <= 0 || cards < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    // TODO: Correctly Fetch player data
    //Player player = PLAYARDATABASE.find(playerId);
    //player.cards += cards;
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId <= 0)
    {
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
    {
        return StatusType::INVALID_INPUT;
    }
	// TODO: Need to figure out how to accumulate spirit from continuous bought teams.
	//return permutation_t();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0)
        return StatusType::INVALID_INPUT;
	// TODO: Need to properly modify player's part_spirit when teams are bought continuesly.
	return StatusType::SUCCESS;
}
