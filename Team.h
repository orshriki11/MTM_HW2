//
// Created by tur22 on 05/12/2022.
//

#ifndef WET1MIVNE_MASTER_TEAM_H
#define WET1MIVNE_MASTER_TEAM_H

#include "AVLTree.h"
#include <iostream>
#include <memory>
#include <stdio.h>
#include "Player.h"
#include "wet2util.h"
#include "UnionFindNode.h"

struct Player; // forward declaration


struct Team {
    int teamId;
    int points;
    int totalCards;
    int gamesPlayed;
    int gksCount;
    int playersCount;
    int totalAbility;
    bool isRemoved;
    //int GamesPlayed_preBought;
    bool isNew;
    permutation_t teamSpirit;
    UnionFindNode<std::shared_ptr<Team>, std::shared_ptr<Player>> *UF_Team;


/*    AVLTree<int, std::shared_ptr<Player>> teamPlayers_byID;
    AVLTree<Player, int> teamPlayers_byRank;*/

    explicit Team(int teamId) : teamId(teamId),
                       points(0),
                       totalCards(0),
                       gamesPlayed(0),
                       gksCount(0),
                       playersCount(0),
                       totalAbility(0),
                       isRemoved(false),
                       isNew(true),

                       UF_Team(nullptr)

                       {
        teamSpirit = permutation_t::neutral();
    }

    Team() = default;

    ~Team() {
    };

    bool operator<(const Team& other) const {
        if (totalAbility == other.totalAbility) {
            return teamId < other.teamId;
        } else {
            return totalAbility < other.totalAbility;
        }
    }

    bool operator==(const Team& other) const {
        return teamId == other.teamId;
    }

    bool operator>(const Team& other) const {
        return !(*this < other || *this == other);
    }


};

#endif //WET1MIVNE_MASTER_TEAM_H
