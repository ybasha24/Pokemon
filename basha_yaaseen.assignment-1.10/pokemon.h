#include "db_parse.h"
#include <string>
#include <vector>
#include <cmath>

#ifndef POKEMON_H
#define POKEMON_H

class pokemon_generator
{
    public:
    int pokemonId=0;
    char *pokemonIdentifier;
    bool isMale=false;
    int level = 0;
    int hp =0;
    int currentHp;
    int attack= 0;
    int defense=0;
    int speed=0;
    int specialA=0;
    int specialD=0;
    bool isShiny=false;
    pokemon_move_db move1;
    pokemon_move_db move2;

    pokemon_generator(int x, int y){//x and y is for world coordinates so i can properly account for level based on distance from 'origin'
        int randomPokemonIndex = rand() % 1092;
        pokemonId = pokemon[randomPokemonIndex].id;
        pokemonIdentifier = pokemon[randomPokemonIndex].identifier;
        while(pokemonIdentifier[1] == '\0' || pokemonIdentifier[0] == '\0' || pokemonIdentifier[2] == '\0'){
            randomPokemonIndex = rand() % 1092;
            pokemonId = pokemon[randomPokemonIndex].id;
            pokemonIdentifier = pokemon[randomPokemonIndex].identifier;
        }
        int iv = rand() % 16;
        int distance = abs((x - 200)) + abs((y - 200));
        int minLevel;
        int maxLevel;
        if(distance <= 200){
            minLevel = 1;
            maxLevel = std::max(distance/2, 1);
        }else{
            minLevel = (distance - 200)/2;
            maxLevel = 100;
        }

        level = (rand() % (maxLevel - minLevel +1)) + minLevel;//changed this
        // level = 1;

        std::vector<pokemon_stats_db> pokeStats;
        pokeStats.clear();
        for(int i = 0; i < 6552; i++){
            if(pokemon_stats[i].pokemon_id == pokemonId){
                pokeStats.push_back(pokemon_stats[i]);
            }
        }
        int hpBase=0;
        int attackBase=0;
        int defenseBase=0;
        int specialABase=0;
        int specialDBase=0;
        int speedBase=0;
        for(pokemon_stats_db currentStat: pokeStats){
            if(currentStat.stat_id == 1){
                hp = currentStat.base_stat + iv;
                currentHp = hp;
                hpBase = currentStat.base_stat;
            }
            if(currentStat.stat_id == 2){
                attack = currentStat.base_stat + iv;
                attackBase = currentStat.base_stat;
            }
            if(currentStat.stat_id == 3){
                defense = currentStat.base_stat + iv;
                defenseBase = currentStat.base_stat;
            }
            if(currentStat.stat_id == 4){
                specialA = currentStat.base_stat + iv;
                specialABase = currentStat.base_stat;
            }
            if(currentStat.stat_id == 5){
                specialD= currentStat.base_stat + iv;
                specialDBase = currentStat.base_stat;
            }
            if(currentStat.stat_id == 6){
                speed = currentStat.base_stat + iv;
                speedBase = currentStat.base_stat;
            }
        } 
        pokeStats.clear();

        int gender = rand() % 2;
        if(gender == 0) {
            isMale = false;
        }
        else{
            isMale = true;
        }

        //initialize the move stuff
        bool changedLevel = false;
        std::vector<pokemon_move_db> pokeMoves;
        while(pokeMoves.empty() == true){
            pokeMoves.clear();
            for(int i = 0; i < 528238; i++){
                if(pokemon_moves[i].pokemon_move_method_id == 1 && 
                pokemon_moves[i].pokemon_id == pokemon[randomPokemonIndex].species_id && level >= pokemon_moves[i].level){// && this->level >= pokemon_moves[i].level
                    pokeMoves.push_back(pokemon_moves[i]);
                }
            }
            if(pokeMoves.empty() == true){
                level = level +1;
                changedLevel = true;
            }
        }

        int move1rand = rand() % pokeMoves.size();
        int move2rand = rand() % pokeMoves.size();
        //uniform distribution
        move1 = pokeMoves[move1rand];
        move2 = pokeMoves[move2rand];
        pokeMoves.clear();
        


        if(changedLevel == true){
            hp = levelHP(hpBase, iv);
            currentHp = hp;
            attack = otherStats(attackBase, iv);
            defense = otherStats(defenseBase, iv);
            specialA = otherStats(specialABase, iv);
            specialD = otherStats(specialDBase, iv);
            speed = otherStats(speedBase, iv);
        }

        int shiny = rand() % 8192;
        if(shiny == 0){
            isShiny = true;
        }
        else{
            isShiny = false;
        }

    }

    int otherStats(int base, int iv){
        return floor(((((base + iv) * 2) * this->level) / 100)) + 5;
    }

    int levelHP(int base, int iv){
        return floor((((((base + iv) * 2) * this->level) / 100)) + this->level + 10);
    }


};

#endif