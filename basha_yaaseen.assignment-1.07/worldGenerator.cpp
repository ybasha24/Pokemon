#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<climits>
#include<unistd.h>
#include <ncurses.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "worldGenerator.h"
#include "priorityQueue.h"
#include "characterQueue.h"
#include "parse.h"

int probabilityOfPokeStuff(int x, int y);
void dijkstrasMethod(int grid[21][80], int hikerOrRider, int distance[21][80]);
void printDistTable(int distance[21][80]);
int costOfTerrainForHiker(int x, int y, int grid[21][80]);
void moveRival(int grid[21][80], Characters *characterTable[21][80], int distanceMap[21][80], Characters **head);
int costOfTerrain(int x, int y, int grid[21][80], int hikerOrRider);
void moveHiker(int grid[21][80], Characters *characterTable[21][80], int distanceMap[21][80], Characters **head);
void moveExplorer(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head);
void moveWander(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head);
void movePacer(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head);
void placeCharacter(int grid[21][80], Characters *characterTable[21][80], int characterNumber, int seqNum, int direction, Characters **head);
void initializeNewMap(map_t *world);

int pcXcoord;
int pcYcoord;

// map_t *w[402][402];

void io_init_terminal()
    {
      initscr();
      raw();
      noecho();
      curs_set(0);
      keypad(stdscr, TRUE);
    }


int isSpotNPC(int spot){
  if(spot == 10 || spot == 11 || spot == 12 || spot == 13 || spot == 14 || spot == 15){
    return 1;
  }
  else{
    return 0;
  }
}

char typeOfNPC(int type){
     if(type == 10){
        return 'h';
      }
      else if(type == 11){
        return 'r';
      }
      else if(type == 12){
        return 'p';
      }
      else if(type == 13){
        return 's';
      }
      else if(type ==  14){
        return 'e';
      }
      else{
        return 'w';
      }
}

void printDisplay(Characters *characterTable[21][80], int grid[21][80]){
  int y = 0;
  for(int i = 0; i < 21; i++){
    for(int j =0; j< 80; j++){
      y = j;
      if(characterTable[i][j] != NULL){
        if(characterTable[i][j]->symbol ==8){
          mvaddch(i+1, j, '@');
        }
        else if(characterTable[i][j]->symbol == 10){
          mvaddch(i+1, j, 'h');
        }
        else if(characterTable[i][j]->symbol == 11){
          mvaddch(i+1, j, 'r');
        }
        else if(characterTable[i][j]->symbol == 12){
          mvaddch(i+1, j, 'p');
        }
        else if(characterTable[i][j]->symbol == 13){
          mvaddch(i+1, j, 's');
        }
        else if(characterTable[i][j]->symbol ==  14){
          mvaddch(i+1, j, 'e');
        }
        else if(characterTable[i][j]->symbol == 15){
          mvaddch(i+1, j, 'w');
        }
      }
      else{
        if(grid[i][j] == 1){
	        mvaddch(i+1, j, '%');
        }
        else if (grid[i][j]  == 2)
        {
          mvaddch(i+1, j, '#');
        }
        else if(grid[i][j] ==3){
          mvaddch(i+1, j, 'M');
        }
        else if(grid[i][j] ==4){
          mvaddch(i+1, j, 'C');
        }
        else if(grid[i][j] ==5){
          mvaddch(i+1, j, ':');
        }
        else if(grid[i][j] ==6){
          mvaddch(i+1, j, '.');
        }
        else if(grid[i][j] ==7){
          mvaddch(i+1, j, '^');
        }
        else if(grid[i][j] ==0){
          mvaddch(i+1, j, '^');
        }
      }
     }
    mvaddch(i+2, y, '\n');
  }
}

void parseFile(std::string fileToParse){
  if(fileToParse == "type_names"){
    std::vector<Type_Names> typeNamesVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/type_names.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/type_names.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }
    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    while(getline(inputFile,lineToParse)){
      Type_Names typeNamesInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      if(editor != ""){
        typeNamesInstance.type_id = stoi(editor);
      }else{
        typeNamesInstance.type_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        typeNamesInstance.local_language_id = stoi(editor);
      }else{
        typeNamesInstance.local_language_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      typeNamesInstance.name = editor;
      editor = "";

      if(typeNamesInstance.local_language_id == 9){
        typeNamesVector.push_back(typeNamesInstance);
      }
      lineToParse = "";

    }

    for(Type_Names typeNames : typeNamesVector){
      if(typeNames.type_id != INT_MAX){
        printf("%d", typeNames.type_id);
      }
      if(typeNames.local_language_id != INT_MAX){
        printf(" %d", typeNames.local_language_id);
      }
      printf(" %s", typeNames.name.c_str());
      printf("\n");
    }

  }

  else if(fileToParse == "pokemon_types"){
    std::vector<Pokemont_Types> pokemonTypesVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon_types.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/pokemon_types.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }

    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    // getline(inputFile, line);
    int count = 0;
    while(getline(inputFile,lineToParse)){
      Pokemont_Types pokemonTypesInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonTypesInstance.pokemon_id = stoi(editor);
      }else{
        pokemonTypesInstance.pokemon_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonTypesInstance.type_id = stoi(editor);
      }else{
        pokemonTypesInstance.type_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonTypesInstance.slot = stoi(editor);
      }else{
        pokemonTypesInstance.slot  = INT_MAX;
      }
      editor = "";

      pokemonTypesVector.push_back(pokemonTypesInstance);
      
      lineToParse = "";
    }

    for (Pokemont_Types type: pokemonTypesVector){
      if(type.pokemon_id != INT_MAX){
        printf("%d", type.pokemon_id);
      }
      if(type.type_id != INT_MAX){
        printf(" %d", type.type_id);
      }
      if(type.slot != INT_MAX){
        printf(" %d", type.slot);
      }
      printf("\n");
    }

  }

  else if(fileToParse == "stats"){
    std::vector<Stats> statsVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/stats.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/stats.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }

    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    // getline(inputFile, line);
    int count = 0;
    while(getline(inputFile,lineToParse)){
      Stats statsInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      if(editor != ""){
        statsInstance.id = stoi(editor);
      }else{
        statsInstance.id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        statsInstance.damage_class_id = stoi(editor);
      }else{
        statsInstance.damage_class_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      statsInstance.identifier = editor;
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        statsInstance.is_battle_only = stoi(editor);
      }else{
        statsInstance.is_battle_only = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        statsInstance.game_index = stoi(editor);
      }else{
        statsInstance.game_index = INT_MAX;
      }
      editor = "";
      

      statsVector.push_back(statsInstance);
      
      lineToParse = "";
    }

    for (Stats stats: statsVector){
      if(stats.id != INT_MAX){
        printf("%d", stats.id);
      }
      if(stats.damage_class_id != INT_MAX){
        printf(" %d", stats.damage_class_id);
      }
      printf(" %s",stats.identifier.c_str());
      if(stats.is_battle_only != INT_MAX){
        printf(" %d", stats.is_battle_only);
      }
      if(stats.game_index != INT_MAX){
        printf(" %d", stats.game_index);
      }
      printf("\n");
    }

  }

  else if(fileToParse == "pokemon_stats"){
    std::vector<Pokemon_Stats> pokeStatsVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon_stats.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/pokemon_stats.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }

    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    // getline(inputFile, line);
    int count = 0;
    while(getline(inputFile,lineToParse)){
      Pokemon_Stats pokeStatsInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      if(editor != ""){
        pokeStatsInstance.pokemon_id = stoi(editor);
      }else{
        pokeStatsInstance.pokemon_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokeStatsInstance.stat_id = stoi(editor);
      }else{
        pokeStatsInstance.stat_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokeStatsInstance.base_stat = stoi(editor);
      }else{
        pokeStatsInstance.base_stat  = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokeStatsInstance.effort = stoi(editor);
      }else{
        pokeStatsInstance.effort  = INT_MAX;
      }
      editor = "";

      pokeStatsVector.push_back(pokeStatsInstance);
      
      lineToParse = "";
    }

    for (Pokemon_Stats pokeStats: pokeStatsVector){
      if(pokeStats.pokemon_id != INT_MAX){
        printf("%d", pokeStats.pokemon_id);
      }
      if(pokeStats.stat_id != INT_MAX){
        printf(" %d", pokeStats.stat_id);
      }
      if(pokeStats.base_stat != INT_MAX){
        printf(" %d", pokeStats.base_stat);
      }
      if(pokeStats.effort != INT_MAX){
        printf(" %d", pokeStats.effort);
      }
      printf("\n");
    }

  }

  else if(fileToParse == "experience"){
    std::vector<Experience> experienceVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/experience.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/experience.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }

    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    // getline(inputFile, line);
    int count = 0;
    while(getline(inputFile,lineToParse)){
      Experience experienceInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      if(editor != ""){
        experienceInstance.growth_rate_id = stoi(editor);
      }else{
        experienceInstance.growth_rate_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        experienceInstance.level = stoi(editor);
      }else{
        experienceInstance.level = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        experienceInstance.experience = stoi(editor);
      }else{
        experienceInstance.experience  = INT_MAX;
      }
      editor = "";

      experienceVector.push_back(experienceInstance);
      
      lineToParse = "";
    }

    for (Experience exp: experienceVector){
      if(exp.growth_rate_id != INT_MAX){
        printf("%d", exp.growth_rate_id);
      }
      if(exp.level != INT_MAX){
        printf(" %d", exp.level);
      }
      if(exp.experience != INT_MAX){
        printf(" %d", exp.experience);
      }
      printf("\n");
    }

  }

  else if(fileToParse == "pokemon_species"){
    std::vector<Pokemon_Species> pokemonSpeciesVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon_species.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/pokmemon_species.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }

    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    // getline(inputFile, line);
    int count = 0;
    while(getline(inputFile,lineToParse)){
      Pokemon_Species pokemonSpeciesInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.id = stoi(editor);
      }else{
        pokemonSpeciesInstance.id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      pokemonSpeciesInstance.identifier = editor;
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.generation_id = stoi(editor);
      }else{
        pokemonSpeciesInstance.generation_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.evolves_from_species_id = stoi(editor);
      }else{
        pokemonSpeciesInstance.evolves_from_species_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.evolution_chain_id = stoi(editor);
      }else{
        pokemonSpeciesInstance.evolution_chain_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.color_id = stoi(editor);
      }else{
        pokemonSpeciesInstance.color_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.shape_id = stoi(editor);
      }else{
        pokemonSpeciesInstance.shape_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.habitat_id = stoi(editor);
      }else{
        pokemonSpeciesInstance.habitat_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.gender_rate = stoi(editor);
      }else{
        pokemonSpeciesInstance.gender_rate = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.capture_rate = stoi(editor);
      }else{
        pokemonSpeciesInstance.capture_rate= INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.base_happiness = stoi(editor);
      }else{
        pokemonSpeciesInstance.base_happiness = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.is_baby = stoi(editor);
      }else{
        pokemonSpeciesInstance.is_baby = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.hatch_counter = stoi(editor);
      }else{
        pokemonSpeciesInstance.hatch_counter = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.has_gender_differences = stoi(editor);
      }else{
        pokemonSpeciesInstance.has_gender_differences = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.growth_rate_id = stoi(editor);
      }else{
        pokemonSpeciesInstance.growth_rate_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.forms_switchable = stoi(editor);
      }else{
        pokemonSpeciesInstance.forms_switchable = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.is_legendary = stoi(editor);
      }else{
        pokemonSpeciesInstance.is_legendary = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.is_mythical = stoi(editor);
      }else{
        pokemonSpeciesInstance.is_mythical = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.order = stoi(editor);
      }else{
        pokemonSpeciesInstance.order = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonSpeciesInstance.conquest_order = stoi(editor);
      }else{
        pokemonSpeciesInstance.conquest_order = INT_MAX;
      }
      editor = "";

      pokemonSpeciesVector.push_back(pokemonSpeciesInstance);
      
      lineToParse = "";
    }

    for (Pokemon_Species pokeSpecies: pokemonSpeciesVector){
      printf("%d %s", pokeSpecies.id, pokeSpecies.identifier.c_str());

      if(pokeSpecies.generation_id != INT_MAX){
        printf(" %d", pokeSpecies.generation_id);
      }
      if(pokeSpecies.evolves_from_species_id != INT_MAX){
        printf(" %d", pokeSpecies.evolves_from_species_id);
      }
      if(pokeSpecies.evolution_chain_id != INT_MAX){
        printf(" %d", pokeSpecies.evolution_chain_id);
      }
      if(pokeSpecies.color_id != INT_MAX){
        printf(" %d", pokeSpecies.color_id);
      }
      if(pokeSpecies.shape_id != INT_MAX){
        printf(" %d", pokeSpecies.shape_id);
      }
      if(pokeSpecies.habitat_id != INT_MAX){
        printf(" %d", pokeSpecies.habitat_id);
      }
      if(pokeSpecies.gender_rate != INT_MAX){
        printf(" %d", pokeSpecies.gender_rate);
      }
      if(pokeSpecies.capture_rate != INT_MAX){
        printf(" %d", pokeSpecies.capture_rate);
      }
      if(pokeSpecies.base_happiness != INT_MAX){
        printf(" %d", pokeSpecies.base_happiness);
      }
      if(pokeSpecies.is_baby != INT_MAX){
        printf(" %d", pokeSpecies.is_baby);
      }
      if(pokeSpecies.hatch_counter != INT_MAX){
        printf(" %d", pokeSpecies.hatch_counter);
      }
      if(pokeSpecies.has_gender_differences != INT_MAX){
        printf(" %d", pokeSpecies.has_gender_differences);
      }
      if(pokeSpecies.growth_rate_id != INT_MAX){
        printf(" %d", pokeSpecies.growth_rate_id);
      }
      if(pokeSpecies.forms_switchable != INT_MAX){
        printf(" %d", pokeSpecies.forms_switchable);
      }
      if(pokeSpecies.is_legendary != INT_MAX){
        printf(" %d", pokeSpecies.is_legendary);
      }
      if(pokeSpecies.is_mythical != INT_MAX){
        printf(" %d", pokeSpecies.is_mythical);
      }
      if(pokeSpecies.order != INT_MAX){
        printf(" %d", pokeSpecies.order);
      }
      if(pokeSpecies.conquest_order != INT_MAX){
        printf(" %d", pokeSpecies.conquest_order);
      }
      printf("\n");

    }

  }

  else if(fileToParse == "pokemon_moves"){
    std::vector<Pokemon_Moves> pokemonMovesVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon_moves.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/pokemon_moves.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }

    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    // getline(inputFile, line);
    int count = 0;
    while(getline(inputFile,lineToParse)){
      Pokemon_Moves pokemonMovesInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonMovesInstance.pokemon_id = stoi(editor);
      }else{
        pokemonMovesInstance.pokemon_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonMovesInstance.version_group_id = stoi(editor);
      }else{
        pokemonMovesInstance.version_group_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonMovesInstance.move_id = stoi(editor);
      }else{
        pokemonMovesInstance.move_id  = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonMovesInstance.pokemon_move_method_id = stoi(editor);
      }else{
        pokemonMovesInstance.pokemon_move_method_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonMovesInstance.level = stoi(editor);
      }else{
        pokemonMovesInstance.level = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        pokemonMovesInstance.order = stoi(editor);
      }else{
        pokemonMovesInstance.order= INT_MAX;
      }
      editor = "";
      

      pokemonMovesVector.push_back(pokemonMovesInstance);
      
      lineToParse = "";
    }

    for (Pokemon_Moves pokemonMoves: pokemonMovesVector){
      if(pokemonMoves.pokemon_id != INT_MAX){
        printf("%d", pokemonMoves.pokemon_id);
      }
      if(pokemonMoves.version_group_id != INT_MAX){
        printf(" %d", pokemonMoves.version_group_id);
      }
      if(pokemonMoves.move_id != INT_MAX){
        printf(" %d", pokemonMoves.move_id);
      }
      if(pokemonMoves.pokemon_move_method_id != INT_MAX){
        printf(" %d", pokemonMoves.pokemon_move_method_id);
      }
      if(pokemonMoves.level != INT_MAX){
        printf(" %d", pokemonMoves.level);
      }
      if(pokemonMoves.order != INT_MAX){
        printf(" %d", pokemonMoves.order);
      }
      printf("\n");
    }

  }

  else if(fileToParse == "moves"){
    std::vector<Moves> movesVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/moves.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/moves.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }

    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    // getline(inputFile, line);
    int count = 0;
    while(getline(inputFile,lineToParse)){
      Moves movesInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.id = stoi(editor);
      }else{
        movesInstance.id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      movesInstance.identifier = editor;
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.generation_id = stoi(editor);
      }else{
        movesInstance.generation_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.type_id = stoi(editor);
      }else{
        movesInstance.type_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.power = stoi(editor);
      }else{
        movesInstance.power = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.pp = stoi(editor);
      }else{
        movesInstance.pp = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.accuracy = stoi(editor);
      }else{
        movesInstance.accuracy = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.priority = stoi(editor);
      }else{
        movesInstance.priority = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.target_id = stoi(editor);
      }else{
        movesInstance.target_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.damage_class_id = stoi(editor);
      }else{
        movesInstance.damage_class_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.effect_id = stoi(editor);
      }else{
        movesInstance.effect_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.effect_chance = stoi(editor);
      }else{
        movesInstance.effect_chance = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.contest_type_id = stoi(editor);
      }else{
        movesInstance.contest_type_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.contest_effect_id = stoi(editor);
      }else{
        movesInstance.contest_effect_id = INT_MAX;
      }
      editor = "";
      getline(inputString, editor, ',');
      if(editor != ""){
        movesInstance.super_contest_effect_id = stoi(editor);
      }else{
        movesInstance.super_contest_effect_id = INT_MAX;
      }
      editor = "";

      movesVector.push_back(movesInstance);
      
      lineToParse = "";
    }

    for (Moves move: movesVector){
      printf("%d %s", move.id, move.identifier.c_str());

      if(move.generation_id != INT_MAX){
        printf(" %d", move.generation_id);
      }
      if(move.type_id != INT_MAX){
        printf(" %d", move.type_id);
      }
      if(move.power != INT_MAX){
        printf(" %d", move.power);
      }
      if(move.pp != INT_MAX){
        printf(" %d", move.pp);
      }
      if(move.accuracy != INT_MAX){
        printf(" %d", move.accuracy);
      }
      if(move.priority != INT_MAX){
        printf(" %d", move.priority);
      }
      if(move.target_id != INT_MAX){
        printf(" %d", move.target_id);
      }
      if(move.damage_class_id != INT_MAX){
        printf(" %d", move.damage_class_id);
      }
      if(move.effect_id != INT_MAX){
        printf(" %d", move.effect_id);
      }
      if(move.effect_chance != INT_MAX){
        printf(" %d", move.effect_chance);
      }
      if(move.contest_type_id != INT_MAX){
        printf(" %d", move.contest_type_id);
      }
      if(move.contest_effect_id != INT_MAX){
        printf(" %d", move.contest_effect_id);
      }
      if(move.super_contest_effect_id != INT_MAX){
        printf(" %d", move.super_contest_effect_id);
      }
      printf("\n");
    }

  }


  else if(fileToParse == "pokemon"){
    std::vector<Pokemon> pokemonVector;
    std::ifstream inputFile;
    inputFile.open("/share/cs327/pokedex/pokedex/data/csv/pokemon.csv");
    if(inputFile.good()){
      printf("File found!\n");
    }else{
      printf("File NOT found\n");
      std::string inputFileString = (getenv("HOME"));
      inputFileString = inputFileString + "./poke327/pokedex/pokedex/data/csv/pokemon.csv";
      inputFile.open(inputFileString);
      if(inputFile.good()){
        printf("File found!\n");
      }else{
        printf("File NOT found\n");
        return;
      }
    }

    std::string lineToParse = "";
    getline(inputFile, lineToParse);
    // getline(inputFile, line);
    int count = 0;
    while(getline(inputFile,lineToParse)){
      Pokemon pokemonInstance;
      std::string editor = "";
      std::stringstream inputString(lineToParse);
      getline(inputString, editor, ',');
      pokemonInstance.id = stoi(editor);
      editor = "";
      getline(inputString, editor, ',');
      pokemonInstance.identifier = editor;
      editor = "";
      getline(inputString, editor, ',');
      pokemonInstance.species_id = stoi(editor);
      editor = "";

      getline(inputString, editor, ',');
      pokemonInstance.height = stoi(editor);
      editor = "";

      getline(inputString, editor, ',');
      pokemonInstance.weight = stoi(editor);
      editor = "";

      getline(inputString, editor, ',');
      pokemonInstance.base_experience = stoi(editor);
      editor = "";

      getline(inputString, editor, ',');
      pokemonInstance.order = stoi(editor);
      editor = "";

      getline(inputString, editor, ',');
      pokemonInstance.is_default = stoi(editor);
      editor = "";

      pokemonVector.push_back(pokemonInstance);
      
      lineToParse = "";
    }

    for (Pokemon poke: pokemonVector){
      printf("%d %s %d %d %d %d %d %d\n", poke.id, poke.identifier.c_str(),
             poke.species_id, poke.height, poke.weight,
             poke.base_experience, poke.order, poke.is_default);
    }
  }
  else{
    printf("file does not exist or wrong input is given!\n");
    return;
  }
}

int main(int argc, char *argv[]){
    // int pcXcoord; 
    // int pcYcoord;
    // pcXcoord = getPCX();
    // pcYcoord = getPCY();
    // int *pcXcoordNew = &(pcXcoord);
    // int *pcYcoordNew = &(pcYcoord);
    // *pcXcoordNew = getPCX();
    // *pcYcoordNew = getPCY();
    std::string argument = argv[1];
    // parseFile("pokemon");
    // parseFile("moves");
    // parseFile("pokemon_moves");
    // parseFile("pokemon_species");
    // parseFile("experience");
    // parseFile("type_names");
    // parseFile("pokemon_stats");
    // parseFile("stats");
    // parseFile("pokemon_types");
    parseFile(argument);
    return 0;
    int chance1;
    int chanceToSend;//pokemart
    int chanceToSend2;//pokecenter
    map_t *w[402][402];
    int i, j;
    for(i =0; i<=401; i++){
        for(j =0; j<=401; j++){
            w[i][j]= NULL;
            // w[i][j]->cPQ = NULL;
            // w[i][j]->characterTable = nullptr;
        }
    }
    int x = 200;
    int y = 200;
    char userInput;
    srand(time(NULL));

    if(w[x][y] == NULL){
        w[x][y] = (map_t*) malloc((sizeof(map_t)));
        (w[x][y]->eExit) = (rand() % 17) + 2;
        (w[x][y]->wExit) = (rand() % 17) + 2;
        (w[x][y]->nExit) = (rand() % 76) + 2;
        (w[x][y]->sExit) = (rand() % 76) + 2;
        initMap(w[x][y],x,y,1,1);
    }
    // printGrid(w[200][200]->grid);
    printf("\n");
    // int characterMovements [21][80] = {0};
    Characters *characterTable[21][80];
    // for(int i = 0; i < 21; i++){
    //   for(int j =0; j< 80; j++){
    //     characterMovements[i][j] = w[200][200]->grid[i][j];
    //   }
    // }
    // printGrid(characterMovements);
    //place npc method where parameter is grid, character grid, queue, character number, direction 
    if(x == 200 && y == 200){
      // characterMovements[getPCX()][getPCY()] = 8;
      w[x][y]->grid[getPCX()][getPCY()] = 2;
    }
  
    
    

    // printGrid(characterMovements);
    

    w[x][y]->cPQ = newCharacter(getPCX(), getPCY(), 0, 0, '@', -1);
    // int distanceMapForHiker[21][80] ={0};
    // int distanceMapForRider[21][80] ={0};
    // dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
    // dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);

    



    if(argc == 1){
      char uInput = 'p';
      pcXcoord = getPCX();
      pcYcoord = getPCY();
      w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);

      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 10, 1, -1, &(w[x][y]->cPQ));
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 11, 2, -1, &(w[x][y]->cPQ));
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 12, 3, -1, &(w[x][y]->cPQ));
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 13, 4, -1, &(w[x][y]->cPQ));
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 14, 5, -1, &(w[x][y]->cPQ));
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 14, 7, -1, &(w[x][y]->cPQ));
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 14, 8, -1, &(w[x][y]->cPQ));
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 15, 6, -1, &(w[x][y]->cPQ));
      int distanceMapForHiker[21][80] ={0};
      int distanceMapForRider[21][80] ={0};
      dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
      dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
      io_init_terminal();
      // printf("It is %d\n", pcXcoord);//////////////////////////////////
      move(0,0);
      clrtoeol();
      refresh();
      mvprintw(0, 0,"Your Turn!");
      refresh();
      printDisplay(w[x][y]->characterTable, w[x][y]->grid);
      refresh();
      while(uInput != 'q'){
      //   move(0,0);
      // clrtoeol();
      // refresh();
      // mvprintw(0, 0,"x is %d and y is %d", x, y);
      // refresh();
        Characters *peeked = peekCharacter(&(w[x][y]->cPQ));
        // uInput = getch();
        if(peeked->symbolOfCharacter == '@'){
          // printGrid(characterMovements);
          // usleep(250000);
          // io_init_terminal();
          // mvprintw(0, 0,"First map");
          // refresh();
          if(w[x][y]!= NULL){
            printDisplay(w[x][y]->characterTable, w[x][y]->grid);
            refresh();
          }
          uInput = getch();
          if(uInput == '5' || uInput == ' ' || uInput == '.'){
            move(0,0);
            clrtoeol();
            refresh();
            mvprintw(0, 0,"Your Turn!");
            refresh();
            int nextTurn = peeked->nextTurn;
            // move(0,0);
            // clrtoeol();
            // refresh();
            // mvprintw(0, 0,"next turn is %d", nextTurn);
            // refresh();
            popCharacter(&(w[x][y]->cPQ));
            pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
            // pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn, 0, '@', -1);
          }
          else if(uInput == 'k' || uInput == '8'){//going up
            if(pcXcoord > 1 && costOfTerrain((pcXcoord) -1,pcYcoord,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord-1][pcYcoord]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord -1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord-1][pcYcoord]->battled==0){
                w[x][y]->characterTable[pcXcoord-1][pcYcoord]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  clear();
                }
                continue;
              }
              else{
                move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }
            }
            else if(w[x][y]->grid[pcXcoord-1][pcYcoord] == 2){//moving to the north map
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              y = y -1;
              if(w[x][y]==NULL){
                  w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
                  w[x][y]->sExit = (w[x][y+1]->nExit);
                  if(w[x-1][y] != NULL && (x != 0)){
                      // printf("wExit should be %d", *(w[x-1][y]->eExit) );
                      w[x][y]->wExit = (w[x-1][y]->eExit);
                  }
                  else{//add print statements so it works!
                      // printf("wExit is random");
                      w[x][y]->wExit = (rand() % 17) + 2;
                  }
                  if(w[x+1][y] != NULL && (x != 400)){
                      w[x][y]->eExit = (w[x+1][y]->wExit);
                  }
                  else{
                      // printf("eExit is random");
                      w[x][y]->eExit = (rand() % 17) + 2;
                  }
                  if(w[x][y-1]!= NULL && (y != 0)){
                      w[x][y]->nExit= (w[x][y-1]->sExit); 
                  }
                  else{
                      // printf("nExit is random");
                      w[x][y]->nExit = (rand() % 76) + 2;//might need an if statement as well
                  }

                  // initMap(w[x][y],x,y);//I need to check this with TA
                  chance1 = probabilityOfPokeStuff(x,y);
                  chanceToSend = (rand() % 100) + 0;//pokemart
                  chanceToSend2 = (rand() % 100) + 0;//pokecenter
                  if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                      initMap(w[x][y],x,y,1,1);//I need to check this with TA
                  }
                  else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                      initMap(w[x][y],x,y,0,0);//I need to check this with TA
                  }
                  else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                      initMap(w[x][y],x,y,1,0);
                  }
                  else{
                      initMap(w[x][y],x,y,0,1);
                  }
                  pcXcoord = 19;
                  initializeNewMap(w[x][y]);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
                  
              }
              else{
                  pcXcoord = 19;
                  // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                  w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
                
              }


            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'y' || uInput == '7'){//going up and left
            if(pcXcoord > 1 && pcYcoord > 1 && costOfTerrain((pcXcoord) -1,(pcYcoord) - 1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord-1][pcYcoord-1] == NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord -1;
              pcYcoord = pcYcoord -1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord-1][pcYcoord-1]->battled==0){
                w[x][y]->characterTable[pcXcoord-1][pcYcoord-1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  clear();
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'u' || uInput == '9'){//going up and right
            if(pcXcoord > 1 && pcYcoord < 78 && costOfTerrain((pcXcoord) -1,(pcYcoord) + 1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord-1][pcYcoord+1]== NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord]= NULL;
              pcXcoord = pcXcoord -1;
              pcYcoord = pcYcoord +1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord, 8, 0);
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord-1][pcYcoord+1]->battled==0){
                w[x][y]->characterTable[pcXcoord-1][pcYcoord+1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  clear();
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'n' || uInput == '3'){//going down and right
            if(pcXcoord < 20 && pcYcoord < 78 && costOfTerrain((pcXcoord) +1,(pcYcoord) + 1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord+1][pcYcoord+1]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              pcYcoord = pcYcoord +1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord+1][pcYcoord+1]->battled==0){
                w[x][y]->characterTable[pcXcoord+1][pcYcoord+1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  clear();
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'b' || uInput == '1'){//going down and left
            if(pcXcoord < 20 && pcYcoord > 1 && costOfTerrain((pcXcoord) +1,(pcYcoord) - 1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord+1][pcYcoord-1]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              pcYcoord = pcYcoord -1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8, 0);
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord+1][pcYcoord-1]->battled==0){
                w[x][y]->characterTable[pcXcoord+1][pcYcoord-1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  clear();
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'j' || uInput == '2'){//going down
            if(pcXcoord < 19 && costOfTerrain((pcXcoord) +1,pcYcoord,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord+1][pcYcoord]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord+1][pcYcoord]->battled==0){
                w[x][y]->characterTable[pcXcoord+1][pcYcoord]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  clear();
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else if(w[x][y]->grid[pcXcoord+1][pcYcoord] == 2){//south map
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              y = y +1;
              if(w[x][y]==NULL){
                  w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
                  w[x][y]->nExit = (w[x][y-1]->sExit);
                  if(w[x-1][y] != NULL && (x != 0)){
                      w[x][y]->wExit= (w[x-1][y]->eExit);
                  }
                  else{
                      w[x][y]->wExit = (rand() % 17) + 2;
                  }
                  if(w[x+1][y] != NULL && (x != 400)){
                      w[x][y]->eExit = (w[x+1][y]->wExit);
                  }
                  else{
                      w[x][y]->eExit = (rand() % 17) + 2;
                  }
                  if(w[x][y+1]!=NULL && (y != 400)){
                      w[x][y]->sExit= (w[x][y+1]->nExit);
                  }
                  else{
                      w[x][y]->sExit = (rand() % 76) + 2;//might need an if statement as well
                  }
          
                  // initMap(w[x][y],x,y);//I need to check this with TA
                  chance1 = probabilityOfPokeStuff(x,y);
                  chanceToSend = (rand() % 100) + 0;//pokemart
                  chanceToSend2 = (rand() % 100) + 0;//pokecenter
                  if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                      initMap(w[x][y],x,y,1,1);//I need to check this with TA
                  }
                  else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                      initMap(w[x][y],x,y,0,0);//I need to check this with TA
                  }
                  else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                      initMap(w[x][y],x,y,1,0);
                  }
                  else{
                      initMap(w[x][y],x,y,0,1);
                  }
                  pcXcoord = 1;
                  initializeNewMap(w[x][y]);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }
              else{
                  pcXcoord = 1;
                  // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                  w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }
            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == '4' || uInput == 'h'){//going left
            if(pcYcoord > 1 && costOfTerrain((pcXcoord),(pcYcoord) -1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord][pcYcoord-1]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcYcoord = pcYcoord -1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord,8, 0);
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord][pcYcoord-1]->battled==0){
                w[x][y]->characterTable[pcXcoord][pcYcoord-1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  clear();
                }
                continue;
              }
              else{
                move(0,0);
                clrtoeol();
                refresh();
                mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
                refresh();
                continue;
              }

            }
            else if(w[x][y]->grid[pcXcoord][pcYcoord-1] == 2){//go to west map
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              x = x-1;
              if(w[x][y]==NULL){
                  w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
                  w[x][y]->eExit = (w[x+1][y]->wExit);
                  if(w[x][y+1] != NULL && (y != 400)){
                      w[x][y]->sExit= (w[x][y+1]->nExit);
                  }
                  else{
                      w[x][y]->sExit = (rand() % 76) + 2;
                  }
                  if(w[x][y-1] != NULL && (y != 0)){
                      w[x][y]->nExit = (w[x][y-1]->sExit);
                  }
                  else{
                      w[x][y]->nExit = (rand() % 76) + 2;
                  }
                  if(w[x-1][y] != NULL && (x != 0)){
                      w[x][y]->wExit= (w[x-1][y]->eExit);
                  }
                  else{
                      w[x][y]->wExit = (rand() % 17) + 2;//might need an if statement as well
                  }

                  // initMap(w[x][y],x,y);//I need to check this with TA
                  chance1 = probabilityOfPokeStuff(x,y);
                  chanceToSend = (rand() % 100) + 0;//pokemart
                  chanceToSend2 = (rand() % 100) + 0;//pokecenter
                  if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                      initMap(w[x][y],x,y,1,1);//I need to check this with TA
                  }
                  else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                      initMap(w[x][y],x,y,0,0);//I need to check this with TA
                  }
                  else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                      initMap(w[x][y],x,y,1,0);
                  }
                  else{
                      initMap(w[x][y],x,y,0,1);
                  }
                  pcYcoord = 78;
                  initializeNewMap(w[x][y]);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }
              else{
                  pcYcoord = 78;
                  // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                  w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == '6' || uInput == 'l'){//going right
            if(pcYcoord < 78 && costOfTerrain((pcXcoord),(pcYcoord) +1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord][pcYcoord+1] == NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcYcoord = pcYcoord +1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord][pcYcoord+1]->battled==0){
                w[x][y]->characterTable[pcXcoord][pcYcoord+1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  clear();
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else if(w[x][y]->grid[pcXcoord][pcYcoord+1] == 2){//go to east map
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              x = x+1;
              if(w[x][y]==NULL){
                  w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
                  w[x][y]->wExit = (w[x-1][y]->eExit);
                  if(w[x][y+1] != NULL && (y != 400)){
                      w[x][y]->sExit= (w[x][y+1]->nExit);
                  }
                  else{
                      w[x][y]->sExit = (rand() % 76) + 2;
                  }
                  if(w[x][y-1] != NULL && (y != 0)){
                      w[x][y]->nExit = (w[x][y-1]->sExit);
                  }
                  else{
                      w[x][y]->nExit = (rand() % 76) + 2;
                  }
                  if(w[x+1][y]!= NULL && (x != 400)){
                      w[x][y]->eExit= (w[x+1][y]->wExit);
                  }
                  else{
                      w[x][y]->eExit = (rand() % 17) + 2; //might need an if statement as well
                  }
          
                  // initMap(w[x][y],x,y);//I need to check this with TA
                  chance1 = probabilityOfPokeStuff(x,y);
                  chanceToSend = (rand() % 100) + 0;//pokemart
                  chanceToSend2 = (rand() % 100) + 0;//pokecenter
                  if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                      initMap(w[x][y],x,y,1,1);//I need to check this with TA
                  }
                  else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                      initMap(w[x][y],x,y,0,0);//I need to check this with TA
                  }
                  else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                      initMap(w[x][y],x,y,1,0);
                  }
                  else{
                      initMap(w[x][y],x,y,0,1);
                  }
                  pcYcoord = 1;
                  initializeNewMap(w[x][y]);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }
              else{
                  pcYcoord = 1;
                  // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                  w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 't'){
            clear();
            refresh();
            mvprintw(0,0,"List of Trainers near you: \n");
            int r = 1;
            for(int i = 0; i < 21; i++){
              for(int j =0; j< 80; j++){
                if(w[x][y]->characterTable[i][j] != NULL && w[x][y]->characterTable[i][j]->symbol != 8){//should print this on the trainer list
                  if(pcXcoord - i >= 0 && pcYcoord - j >= 0){
                    mvprintw(r,0,"%c, %d north %d west", typeOfNPC(w[x][y]->characterTable[i][j]->symbol), pcXcoord - i, pcYcoord - j);
                    refresh();
                    r++;
                  }
                  else if(pcXcoord - i <= 0 && pcYcoord - j >= 0){
                    mvprintw(r,0,"%c, %d south %d west", typeOfNPC(w[x][y]->characterTable[i][j]->symbol), abs(pcXcoord - i), pcYcoord - j);
                    refresh();
                    r++;
                  }
                  else if(pcXcoord - i <= 0 && pcYcoord - j <= 0){
                    mvprintw(r,0, "%c, %d south %d east", typeOfNPC(w[x][y]->characterTable[i][j]->symbol), abs(pcXcoord - i), abs(pcYcoord - j));
                    refresh();
                    r++;
                  }
                  else if(pcXcoord - i >= 0 && pcYcoord - j <= 0){
                    mvprintw(r,0,"%c, %d north %d east", typeOfNPC(w[x][y]->characterTable[i][j]->symbol), pcXcoord - i, abs(pcYcoord - j));
                    refresh();
                    r++;
                  }
                }
              }
            }
            
            while(uInput != 27){
              uInput = getch();
              if(uInput == 27){
               break;
              }
            }
            if(uInput == 27){
               clear();
            }
            continue;
            
          }
          else if(uInput == '>'){
            if(w[x][y]->grid[pcXcoord][pcYcoord] == 3){//Pokemart
              clear();
              refresh();
              mvprintw(0,0,"In a pokeMart!");
              while(uInput != '<'){
                uInput = getch();
                if(uInput == '<'){
                  break;
                }
              }
              if(uInput == '<'){
                clear();
              }
              continue;
            }
            else if(w[x][y]->grid[pcXcoord][pcYcoord] == 4){//Pokecenter
              clear();
              refresh();
              mvprintw(0,0,"In a pokeCenter!");
              while(uInput != '<'){
                uInput = getch();
                if(uInput == '<'){
                  break;
                }
              }
              if(uInput == '<'){
                clear();
              }
              continue;
            }

          }
          else if(uInput == 'f'){
            // scanf("%d %d", &x, &y);
            free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
            w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
            // clear();
            // refresh();
            while(uInput == 'f'){
              clear();
              refresh();
            move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Type 2 ints with a space between each and hit enter after!");
              refresh();
            mvscanw(0,0,"%d %d", &x, &y);
            if((x < -200) || (x > 200) || (y < -200) || (y > 200)){
                move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Can't go there, try again!");
              refresh();
                continue;
            }
            // printf("flying to (%d,%d)\n", x, y);
            x = x + 200;
            y = y + 200;
            
            if(w[x][y] == NULL){
                w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
                if(w[x][y-1]!=NULL && (y != 0)){
                    w[x][y]->nExit = w[x][y-1]->sExit;
                }
                else{
                    w[x][y]->nExit = (rand() % 76) + 2;
                }
                if(w[x][y+1]!=NULL && (y != 400)){
                    w[x][y]->sExit = w[x][y+1]->nExit;
                }
                else{
                    w[x][y]->sExit = (rand() % 76) + 2;
                }
                if(w[x+1][y]!=NULL && (x != 400)){
                    w[x][y]->eExit = w[x+1][y]->wExit;
                }
                else{
                    w[x][y]->eExit = (rand() % 17) + 2;
                }
                if(w[x-1][y] != NULL && (x != 0)){
                    w[x][y]->wExit = w[x-1][y]->eExit;
                }
                else{
                    w[x][y]->wExit = (rand() % 17) + 2;
                }
                chance1 = probabilityOfPokeStuff(x,y);
                chanceToSend = (rand() % 100) + 0;//pokemart
                chanceToSend2 = (rand() % 100) + 0;//pokecenter
                if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                    initMap(w[x][y],x,y,1,1);//I need to check this with TA
                }
                else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,0,0);//I need to check this with TA
                }
                else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,1,0);
                }
                else{
                    initMap(w[x][y],x,y,0,1);
                }
                pcXcoord = getPCX();
                pcYcoord = getPCY();
                initializeNewMap(w[x][y]);
                for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                break;
            }
            else{
                // pcYcoord = 1;
                // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                while(w[x][y]->characterTable[pcXcoord][pcYcoord] != NULL){
                  if(w[x][y]->characterTable[pcXcoord-1][pcYcoord] == NULL){
                    pcXcoord = pcXcoord -1;
                    break;
                  }
                  else if(w[x][y]->characterTable[pcXcoord+1][pcYcoord] == NULL){
                    pcXcoord = pcXcoord +1;
                    break;
                  }
                  else if(w[x][y]->characterTable[pcXcoord][pcYcoord-1] == NULL){
                    pcYcoord = pcYcoord -1;
                    break;
                  }
                  else if(w[x][y]->characterTable[pcXcoord][pcYcoord+1] == NULL){
                    pcYcoord = pcYcoord +1;
                    break;
                  }
                }
                w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                break;
            }
            continue;

          }


          }
          else if(uInput == 'q'){
            continue;
          }
          else{
            uInput = getch();
          }
        }
        if(peeked->symbolOfCharacter == 'r'){
          moveRival(w[x][y]->grid, w[x][y]->characterTable, distanceMapForRider, &(w[x][y]->cPQ));
          continue;
        }
        if(peeked->symbolOfCharacter == 'h'){
          moveHiker(w[x][y]->grid, w[x][y]->characterTable, distanceMapForHiker, &(w[x][y]->cPQ));
          continue;
        }
        if(peeked->symbolOfCharacter == 'p'){
          int direction = peeked->direction;
          movePacer(w[x][y]->grid, w[x][y]->characterTable, &direction, &(w[x][y]->cPQ));
          continue;
        }

        if(peeked->symbolOfCharacter == 's'){
          int xCoord = peeked->xCoord;
          int yCoord = peeked->yCoord;
          int seqNum = peeked->seqNum;
          int nextTurn = peeked->nextTurn + costOfTerrain(xCoord, yCoord, w[x][y]->grid, 1);
          popCharacter(&(w[x][y]->cPQ));
          pushCharacter(&(w[x][y]->cPQ), xCoord, yCoord, nextTurn, seqNum, 's', -1);
          continue;
        }

        if(peeked->symbolOfCharacter == 'e'){
          int direction = peeked->direction;
          moveExplorer(w[x][y]->grid, w[x][y]->characterTable, &direction, &(w[x][y]->cPQ));
          continue;
        }
        if(peeked->symbolOfCharacter == 'w'){
          int direction = peeked->direction;
          moveWander(w[x][y]->grid, w[x][y]->characterTable, &direction, &(w[x][y]->cPQ));
          continue;
        }

        // uInput = getch();
        if(uInput == 'q'){
          break;
        }
      }
       endwin();
    }
    else{//user input is there!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////dont need this
      // int numOfTrainers = atoi(argv[1]);
      // pcXcoord = getPCX();
      // pcYcoord = getPCY();
      // characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
      // int distanceMapForHiker[21][80] ={0};
      // int distanceMapForRider[21][80] ={0};
      // dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
      // dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
      // char uInput = 'p';
      // io_init_terminal();

      // if(numOfTrainers == 1){//just hiker!
      //   placeCharacter(w[200][200]->grid, characterTable, 14, 1, -1, &cPQ);
      //   // printf("It is %d\n", pcXcoord);//////////////////////////////////
      //   mvprintw(0, 0,"Your Turn!");
      //   refresh();
      //   printDisplay(characterTable, w[200][200]->grid);
      //   refresh();
      //   while(uInput != 'q'){
      //     Characters *peeked = peekCharacter(&cPQ);
      //     if(peeked->symbolOfCharacter == '@'){
      //       printDisplay(characterTable, w[200][200]->grid);
      //       refresh();
      //       uInput = getch();
      //       if(uInput == 'a'){
      //         mvprintw(0, 0,"Your Turn!");
      //         refresh();
      //         int nextTurn = peeked->nextTurn;
      //         popCharacter(&cPQ);
      //         pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
      //       }
      //     }
      //     if(peeked->symbolOfCharacter == 'r'){
      //     moveRival(w[200][200]->grid, characterTable, distanceMapForRider, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'h'){
      //       moveHiker(w[200][200]->grid, characterTable, distanceMapForHiker, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'p'){
      //       int direction = peeked->direction;
      //       movePacer(w[200][200]->grid, characterTable, &direction, &cPQ);
      //     }

      //     if(peeked->symbolOfCharacter == 's'){
      //       int xCoord = peeked->xCoord;
      //       int yCoord = peeked->yCoord;
      //       int seqNum = peeked->seqNum;
      //       int nextTurn = peeked->nextTurn + costOfTerrain(xCoord, yCoord, w[200][200]->grid, 1);
      //       popCharacter(&cPQ);
      //       pushCharacter(&cPQ, xCoord, yCoord, nextTurn, seqNum, 's', -1);
      //     }

      //     if(peeked->symbolOfCharacter == 'e'){
      //       int direction = peeked->direction;
      //       moveExplorer(w[200][200]->grid, characterTable, &direction, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'w'){
      //       int direction = peeked->direction;
      //       moveWander(w[200][200]->grid, characterTable, &direction, &cPQ);
      //     }
      //   }
      //   endwin();
      // }
      // else if(numOfTrainers == 2){
      //   placeCharacter(w[200][200]->grid, characterMovements, 10, 1, -1, &cPQ);
      //   placeCharacter(w[200][200]->grid, characterMovements, 11, 2, -1, &cPQ);
      //   while(1){
      //     Characters *peeked = peekCharacter(&cPQ);
      //     if(peeked->symbolOfCharacter == '@'){
      //       printGrid(characterMovements);
      //       usleep(250000);
      //       int nextTurn = peeked->nextTurn;
      //       popCharacter(&cPQ);
      //       pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + 10, 0, '@', -1);
      //     }
      //     if(peeked->symbolOfCharacter == 'r'){
      //       moveRival(w[200][200]->grid, characterMovements, distanceMapForRider, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'h'){
      //       moveHiker(w[200][200]->grid, characterMovements, distanceMapForHiker, &cPQ);
      //     }
      //   }
      // }
      // else{//numtrainers is more than 2
      //   placeCharacter(w[200][200]->grid, characterMovements, 10, 1, -1, &cPQ);
      //   placeCharacter(w[200][200]->grid, characterMovements, 11, 2, -1, &cPQ);
      //   int seqNum = 3;
      //   int counter = 2;
      //   while(counter != numOfTrainers){
      //     placeCharacter(w[200][200]->grid, characterMovements, 12, seqNum, -1, &cPQ);
      //     seqNum += 1;
      //     counter +=1;
      //     if(counter == numOfTrainers){
      //       break;
      //     }
      //     placeCharacter(w[200][200]->grid, characterMovements, 13, seqNum, -1, &cPQ);
      //     seqNum += 1;
      //     counter +=1;
      //     if(counter == numOfTrainers){
      //       break;
      //     }
      //     placeCharacter(w[200][200]->grid, characterMovements, 14, seqNum, -1, &cPQ);
      //     seqNum += 1;
      //     counter +=1;
      //     if(counter == numOfTrainers){
      //       break;
      //     }
      //     placeCharacter(w[200][200]->grid, characterMovements, 15, seqNum, -1, &cPQ);
      //     seqNum += 1;
      //     counter +=1;
      //     if(counter == numOfTrainers){
      //       break;
      //     }
      //   }

      //   while(1){
      //     Characters *peeked = peekCharacter(&cPQ);
      //     if(peeked->symbolOfCharacter == '@'){
      //       printGrid(characterMovements);
      //       usleep(250000);
      //       int nextTurn = peeked->nextTurn;
      //       popCharacter(&cPQ);
      //       pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + 10, 0, '@', -1);
      //     }
      //     if(peeked->symbolOfCharacter == 'r'){
      //       moveRival(w[200][200]->grid, characterMovements, distanceMapForRider, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'h'){
      //       moveHiker(w[200][200]->grid, characterMovements, distanceMapForHiker, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'p'){
      //       int direction = peeked->direction;
      //       movePacer(w[200][200]->grid, characterMovements, &direction, &cPQ);
      //     }

      //     if(peeked->symbolOfCharacter == 's'){
      //       int xCoord = peeked->xCoord;
      //       int yCoord = peeked->yCoord;
      //       int seqNum = peeked->seqNum;
      //       int nextTurn = peeked->nextTurn + costOfTerrain(xCoord, yCoord, w[200][200]->grid, 1);
      //       popCharacter(&cPQ);
      //       pushCharacter(&cPQ, xCoord, yCoord, nextTurn, seqNum, 's', -1);
      //     }

      //     if(peeked->symbolOfCharacter == 'e'){
      //       int direction = peeked->direction;
      //       moveExplorer(w[200][200]->grid, characterMovements, &direction, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'w'){
      //       int direction = peeked->direction;
      //       moveWander(w[200][200]->grid, characterMovements, &direction, &cPQ);
      //     }
      //   }
      // }

    }

    // printf("Going in!\n");
    // printf("%d %d\n", getPCX(), getPCY());
    // printf("%d\n", w[200][200]->grid[getPCX()][getPCY()]);
    // dijkstrasMethod(w[200][200]->grid, 0);
    // printf("\n");
    // dijkstrasMethod(w[200][200]->grid, 1);
    // printf("Coordinates: (%d, %d)", x -200, y -200);
    // printf("Enter a direction or type q to quit: ");
    // printf("\n");
    // scanf("%c", &userInput);
    // while (userInput != 'q')
    // {
        
    //     if(userInput == 'n'){
    //         if(y == 0){
    //             printf("Out of bounds!\n");
    //             printGrid(w[x][y]->grid);
    //             printf("\n");
    //             printf("Enter a direction or type q to quit: ");
    //             printf("\n");
    //             scanf(" %c", &userInput);
    //             continue;
    //         }
    //         y = y -1;
    //         if(w[x][y]==NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             w[x][y]->sExit = (w[x][y+1]->nExit);
    //             if(w[x-1][y] != NULL && (x != 0)){
    //                 // printf("wExit should be %d", *(w[x-1][y]->eExit) );
    //                 w[x][y]->wExit = (w[x-1][y]->eExit);
    //             }
    //             else{//add print statements so it works!
    //                 // printf("wExit is random");
    //                 w[x][y]->wExit = (rand() % 17) + 2;
    //             }
    //             if(w[x+1][y] != NULL && (x != 400)){
    //                 w[x][y]->eExit = (w[x+1][y]->wExit);
    //             }
    //             else{
    //                 // printf("eExit is random");
    //                 w[x][y]->eExit = (rand() % 17) + 2;
    //             }
    //             if(w[x][y-1]!= NULL && (y != 0)){
    //                 w[x][y]->nExit= (w[x][y-1]->sExit); 
    //             }
    //             else{
    //                 // printf("nExit is random");
    //                 w[x][y]->nExit = (rand() % 76) + 2;//might need an if statement as well
    //             }

    //             // initMap(w[x][y],x,y);//I need to check this with TA
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }

    //     if(userInput == 's'){ 
    //         if(y == 400){
    //             printf("Out of bounds!\n");
    //             printGrid(w[x][y]->grid);
    //             printf("\n");
    //             printf("Enter a direction or type q to quit: ");
    //             printf("\n");
    //             scanf(" %c", &userInput);
    //             continue;
    //         }
    //         y = y +1;
    //         if(w[x][y]==NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             w[x][y]->nExit = (w[x][y-1]->sExit);
    //             if(w[x-1][y] != NULL && (x != 0)){
    //                 w[x][y]->wExit= (w[x-1][y]->eExit);
    //             }
    //             else{
    //                 w[x][y]->wExit = (rand() % 17) + 2;
    //             }
    //             if(w[x+1][y] != NULL && (x != 400)){
    //                 w[x][y]->eExit = (w[x+1][y]->wExit);
    //             }
    //             else{
    //                 w[x][y]->eExit = (rand() % 17) + 2;
    //             }
    //             if(w[x][y+1]!=NULL && (y != 400)){
    //                 w[x][y]->sExit= (w[x][y+1]->nExit);
    //             }
    //             else{
    //                 w[x][y]->sExit = (rand() % 76) + 2;//might need an if statement as well
    //             }
        
    //             // initMap(w[x][y],x,y);//I need to check this with TA
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }

    //     if(userInput == 'w'){ 
    //         if (x == 0){
    //             printf("Out of bounds!\n");
    //             printGrid(w[x][y]->grid);
    //             printf("\n");
    //             printf("Enter a direction or type q to quit: ");
    //             printf("\n");
    //             scanf(" %c", &userInput);
    //             continue;
    //         }
    //         x = x-1;
    //         if(w[x][y]==NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             w[x][y]->eExit = (w[x+1][y]->wExit);
    //             if(w[x][y+1] != NULL && (y != 400)){
    //                 w[x][y]->sExit= (w[x][y+1]->nExit);
    //             }
    //             else{
    //                 w[x][y]->sExit = (rand() % 76) + 2;
    //             }
    //             if(w[x][y-1] != NULL && (y != 0)){
    //                 w[x][y]->nExit = (w[x][y-1]->sExit);
    //             }
    //             else{
    //                 w[x][y]->nExit = (rand() % 76) + 2;
    //             }
    //             if(w[x-1][y] != NULL && (x != 0)){
    //                 w[x][y]->wExit= (w[x-1][y]->eExit);
    //             }
    //             else{
    //                 w[x][y]->wExit = (rand() % 17) + 2;//might need an if statement as well
    //             }

    //             // initMap(w[x][y],x,y);//I need to check this with TA
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }

    //     if(userInput == 'e'){ 
    //         if(x == 400){
    //             printf("Out of bounds!\n");
    //             printGrid(w[x][y]->grid);
    //             printf("\n");
    //             printf("Enter a direction or type q to quit: ");
    //             printf("\n");
    //             scanf(" %c", &userInput);
    //             continue;
    //         }
    //         x = x+1;
    //         if(w[x][y]==NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             w[x][y]->wExit = (w[x-1][y]->eExit);
    //             if(w[x][y+1] != NULL && (y != 400)){
    //                 w[x][y]->sExit= (w[x][y+1]->nExit);
    //             }
    //             else{
    //                 w[x][y]->sExit = (rand() % 76) + 2;
    //             }
    //             if(w[x][y-1] != NULL && (y != 0)){
    //                 w[x][y]->nExit = (w[x][y-1]->sExit);
    //             }
    //             else{
    //                 w[x][y]->nExit = (rand() % 76) + 2;
    //             }
    //             if(w[x+1][y]!= NULL && (x != 400)){
    //                 w[x][y]->eExit= (w[x+1][y]->wExit);
    //             }
    //             else{
    //                 w[x][y]->eExit = (rand() % 17) + 2; //might need an if statement as well
    //             }
        
    //             // initMap(w[x][y],x,y);//I need to check this with TA
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }


    //     if(userInput == 'f'){
    //         scanf(" %d %d", &x, &y);
    //         if((x < -200) || (x > 200) || (y < -200) || (y > 200)){
    //             printf("Out of bounds!\n");
    //             break;
    //         }
    //         printf("flying to (%d,%d)\n", x, y);
    //         x = x + 200;
    //         y = y + 200;
            
    //         if(w[x][y] == NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             if(w[x][y-1]!=NULL && (y != 0)){
    //                 w[x][y]->nExit = w[x][y-1]->sExit;
    //             }
    //             else{
    //                 w[x][y]->nExit = (rand() % 76) + 2;
    //             }
    //             if(w[x][y+1]!=NULL && (y != 400)){
    //                 w[x][y]->sExit = w[x][y+1]->nExit;
    //             }
    //             else{
    //                 w[x][y]->sExit = (rand() % 76) + 2;
    //             }
    //             if(w[x+1][y]!=NULL && (x != 400)){
    //                 w[x][y]->eExit = w[x+1][y]->wExit;
    //             }
    //             else{
    //                 w[x][y]->eExit = (rand() % 17) + 2;
    //             }
    //             if(w[x-1][y] != NULL && (x != 0)){
    //                 w[x][y]->wExit = w[x-1][y]->eExit;
    //             }
    //             else{
    //                 w[x][y]->wExit = (rand() % 17) + 2;
    //             }
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }


    //     if(userInput != 'f' && userInput != 'n' && userInput != 's' && userInput != 'e' && userInput != 'w' && userInput != 'q'){
    //         printf("Incorrect input!");
    //     }
    //     printf("\n");
    //     printf("Enter a direction or type q to quit: ");
    //     printf("\n");
    //     scanf(" %c", &userInput);
    // }
}

void initializeNewMap(map_t *world){
  world->cPQ = newCharacter(pcXcoord, pcYcoord,0,0,'@',-1);
  Characters *addCharacterPC = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
  for(int i = 0; i<21; i++){
    for(int j = 0; j<80; j++){
      world->characterTable[i][j] = NULL;
    }
  }
  world->characterTable[pcXcoord][pcYcoord] = addCharacterPC;
  // world->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
  placeCharacter(world->grid, world->characterTable, 10, 1, -1, &(world->cPQ));
  placeCharacter(world->grid, world->characterTable, 11, 2, -1, &(world->cPQ));
  placeCharacter(world->grid, world->characterTable, 12, 3, -1, &(world->cPQ));
  placeCharacter(world->grid, world->characterTable, 13, 4, -1, &(world->cPQ));
  placeCharacter(world->grid, world->characterTable, 14, 5, -1, &(world->cPQ));
  placeCharacter(world->grid, world->characterTable, 14, 7, -1, &(world->cPQ));
  placeCharacter(world->grid, world->characterTable, 14, 8, -1, &(world->cPQ));
  placeCharacter(world->grid, world->characterTable, 15, 6, -1, &(world->cPQ));
  // dijkstrasMethod(world->grid, 0, distanceMapForHiker);
  // dijkstrasMethod(world->grid, 1, distanceMapForRider);

}



//have direction in the place character struct??
void placeCharacter(int grid[21][80], Characters *characterTable[21][80], int characterNumber, int seqNum, int direction, Characters **head){
  int xCoord = ((rand() % 17) + 2);
  int yCoord = ((rand() % 76) + 2);
  //(1 || 2 || 7 || 8 || 3 || 4 || 10 || 11 || 12 || 13 || 14 )
  while(characterTable[xCoord][yCoord]!=NULL){
    xCoord = ((rand() % 17) + 2);
    yCoord = ((rand() % 76) + 2);
    if(characterTable[xCoord][yCoord]==NULL){
      break;
    }
  }

  // characterGrid[xCoord][yCoord] = characterNumber;
  characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, characterNumber, 0);
  

  if(characterNumber == 10){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'h', -1);
  }
  if(characterNumber == 11){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'r', -1);
  }
  if(characterNumber == 12){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'p', -1);
  }
  if(characterNumber == 13){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 's', -1);
  }
  if(characterNumber == 14){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'e', -1);
  }
  if(characterNumber == 15){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'w', -1);
  }
}

void movePacer(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head){
        Characters *peeked = peekCharacter(head);
        if(*direction == -1){
          *direction = (rand() % 8) + 1;
        }
        int xCoord = peeked->xCoord;
        int yCoord = peeked->yCoord;
        int nextTurn = peeked->nextTurn;
        int seqNum = peeked->seqNum;
        int battled = characterTable[xCoord][yCoord]->battled;
        // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
        free(characterTable[xCoord][yCoord]);
        characterTable[xCoord][yCoord] = NULL;
        int change = 0;
        int count = 0;
        while(change != 1 && count != 9){
          if(*direction ==1){//go north
            if(xCoord > 1 && (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX)&& (characterTable[xCoord-1][yCoord] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord-1][yCoord] = 12;
              characterTable[xCoord-1][yCoord] = newCharacterOnMap(xCoord-1,yCoord,12,battled);
              change = 1;
              break;
            }
            else{
              *direction = 5;//s
              count+=1;
              continue;
            }
          }

          if(*direction == 2){//ne
            if(xCoord > 1 && yCoord < 78 && (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX)&& (characterTable[xCoord-1][yCoord+1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord-1][yCoord+1] = 12;
              characterTable[xCoord-1][yCoord+1] = newCharacterOnMap(xCoord-1, yCoord+1, 12, battled);
              change = 1;
              break;
            }
            else{
              *direction = 6;//se
              count+=1;
              continue;
            }
          }

          if(*direction == 3){//e
            if(yCoord < 78 && (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX)&& (characterTable[xCoord][yCoord+1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord][yCoord+1] = 12;
              characterTable[xCoord][yCoord+1] = newCharacterOnMap(xCoord, yCoord+1, 12, battled);
              change =1;
              break;
            }
            else{
              *direction = 7;//w
              count+=1;
              continue;
            }
          }

          if(*direction == 4){//se
            if(xCoord < 19 && yCoord < 78 && (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)&& (characterTable[xCoord+1][yCoord+1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord+1][yCoord+1] = 12;
              characterTable[xCoord+1][yCoord+1] = newCharacterOnMap(xCoord+1, yCoord+1,12,battled);
              change = 1;
              break;
            }
            else{
              *direction = 8;//nw
              count+=1;
              continue;
            }
          }

          if(*direction == 5){//s
            if(xCoord < 19 && (costOfTerrain(xCoord +1, yCoord, grid, 1) != INT_MAX)&& (characterTable[xCoord+1][yCoord] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord +1, yCoord, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord+1][yCoord] = 12;
              characterTable[xCoord+1][yCoord] = newCharacterOnMap(xCoord+1,yCoord,12, battled);
              change =1;
              break;
            }
            else{
              *direction = 1;//n
              count+=1;
              continue;
            }
          }

          if(*direction == 6){//sw
            if(xCoord < 19 && yCoord > 1 && (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX)&& (characterTable[xCoord+1][yCoord-1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord+1][yCoord-1] = 12;
              characterTable[xCoord+1][yCoord-1] = newCharacterOnMap(xCoord+1,yCoord-1,12,battled);
              change = 1;
              break;
            }
            else{
              *direction = 2;//nw
              count+=1;
              continue;
            }
          }

          if(*direction == 7){//w
            if(yCoord > 1 && (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX)&& (characterTable[xCoord][yCoord-1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord][yCoord-1] = 12;
              characterTable[xCoord][yCoord-1] = newCharacterOnMap(xCoord,yCoord-1,12,battled);
              change = 1;
              break;
            }
            else{
              *direction = 3;//w
              count+=1;
              continue;
            }
          }

          if(*direction == 8){//nw
            if(xCoord > 1 && yCoord > 1 && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX)&& (characterTable[xCoord-1][yCoord-1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord-1][yCoord-1] = 12;
              characterTable[xCoord-1][yCoord-1] = newCharacterOnMap(xCoord-1,yCoord-1, 12, battled);
              change = 1;
              break;
            }
            else{
              *direction = 4;//nw
              count+=1;
              continue;
            }
          }

        }
        if(count == 9){
          popCharacter(head);
          pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, grid, 1), seqNum, 'p', *direction);
          // characterGrid[xCoord][yCoord] = 12;
          characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord,yCoord, 12, battled);
        }
}

void moveExplorer(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head){
  Characters *peeked = peekCharacter(head);
  if(*direction == -1){
     *direction = (rand() % 8) + 1;
  }
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int seqNum = peeked->seqNum;
  int nextTurn = peeked->nextTurn;
  int battled = characterTable[xCoord][yCoord]->battled;
  // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  free(characterTable[xCoord][yCoord]);
  characterTable[xCoord][yCoord] = NULL;
  int change = 0;
  int count = 0;
  while(change != 1 && count != 9){
    if(*direction == 1){
      if(xCoord > 1 && (characterTable[xCoord-1][yCoord] == NULL) && (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord-1][yCoord] = 14;
        characterTable[xCoord-1][yCoord] = newCharacterOnMap(xCoord-1, yCoord, 14, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 2){
      if(xCoord > 1 && yCoord < 78 && (characterTable[xCoord-1][yCoord+1] == NULL) && (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord-1][yCoord+1] = 14;
        characterTable[xCoord-1][yCoord+1] = newCharacterOnMap(xCoord-1, yCoord+1, 14, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 3){
      if(yCoord < 78 && (characterTable[xCoord][yCoord+1] == NULL) && (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord][yCoord+1] = 14;
        characterTable[xCoord][yCoord+1] = newCharacterOnMap(xCoord, yCoord+1, 14, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 4){
      if(xCoord < 19 && yCoord < 78 && (characterTable[xCoord+1][yCoord+1] == NULL) && (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord+1][yCoord+1] = 14;
        characterTable[xCoord+1][yCoord+1] = newCharacterOnMap(xCoord+1, yCoord+1, 14, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 5){
      if(xCoord < 19 && (characterTable[xCoord+1][yCoord] == NULL) && (costOfTerrain(xCoord +1, yCoord, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord+1, yCoord, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord+1][yCoord] = 14;
        characterTable[xCoord+1][yCoord] = newCharacterOnMap(xCoord+1, yCoord, 14, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 6){
      if(xCoord < 19 && yCoord > 1 && (characterTable[xCoord+1][yCoord-1] == NULL) && (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord+1][yCoord-1] = 14;
        characterTable[xCoord+1][yCoord-1] = newCharacterOnMap(xCoord+1, yCoord-1, 14, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 7){
      if(yCoord > 1 && (characterTable[xCoord][yCoord-1] == NULL) && (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord][yCoord-1] = 14;
        characterTable[xCoord][yCoord-1] = newCharacterOnMap(xCoord, yCoord-1, 14, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 8){
      if(xCoord > 2 && yCoord > 1 && (characterTable[xCoord-1][yCoord-1] == NULL) && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord-1][yCoord-1] = 14;
        characterTable[xCoord-1][yCoord-1] = newCharacterOnMap(xCoord-1, yCoord-1, 14, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else{
      // printf("Inside Here!!!\n");
      *direction = (rand() % 8) + 1;
      // printf("New Direction is %d\n", *direction);
      continue;
    }
  }
  if(count == 9){
    popCharacter(head);
    pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, grid, 1), seqNum, 'e', *direction);
    // characterGrid[xCoord][yCoord] = 14;
    characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, 14, battled);
  }
}

void moveWander(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head){
  Characters *peeked = peekCharacter(head);
  if(*direction == -1){
    *direction = (rand() % 8) + 1;
  }
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int seqNum = peeked->seqNum;
  int nextTurn = peeked->nextTurn;
  int battled = characterTable[xCoord][yCoord]->battled;
  // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  free(characterTable[xCoord][yCoord]);
  characterTable[xCoord][yCoord]=NULL;
  int change = 0;
  int count = 0;
  while(change != 1 && count != 9){
    if(*direction == 1){
      if(xCoord > 1 && characterTable[xCoord-1][yCoord] == NULL && (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX) && (grid[xCoord-1][yCoord] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord-1][yCoord] = 15;
        characterTable[xCoord-1][yCoord] = newCharacterOnMap(xCoord-1, yCoord, 15, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 2){
      if(xCoord > 1 && yCoord < 78 && characterTable[xCoord-1][yCoord+1] == NULL && (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX) && (grid[xCoord-1][yCoord+1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord-1][yCoord+1] = 15;
        characterTable[xCoord-1][yCoord+1] = newCharacterOnMap(xCoord-1, yCoord+1, 15, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 3){
      if(yCoord < 78 && characterTable[xCoord][yCoord+1] == NULL && (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX) && (grid[xCoord][yCoord+1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord][yCoord+1] = 15;
        characterTable[xCoord][yCoord+1] = newCharacterOnMap(xCoord, yCoord+1, 15, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 4){
      if(xCoord < 19 && yCoord < 78 && characterTable[xCoord+1][yCoord+1] == NULL && (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)&& (grid[xCoord+1][yCoord+1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord+1][yCoord+1] = 15;
        characterTable[xCoord+1][yCoord+1] = newCharacterOnMap(xCoord+1, yCoord+1, 15, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 5){
      if(xCoord < 19 && characterTable[xCoord+1][yCoord] == NULL && (costOfTerrain(xCoord+1, yCoord, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord, grid, 1) != INT_MAX)&& (grid[xCoord+1][yCoord] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord+1, yCoord, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord+1][yCoord] = 15;
        characterTable[xCoord+1][yCoord] = newCharacterOnMap(xCoord+1, yCoord, 15, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 6){
      if(xCoord < 19 && yCoord > 1 && characterTable[xCoord+1][yCoord-1] == NULL && (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX) && (grid[xCoord+1][yCoord-1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord+1][yCoord-1] = 15;
        characterTable[xCoord+1][yCoord-1] = newCharacterOnMap(xCoord+1, yCoord-1, 15, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 7){
      if(yCoord > 1 && characterTable[xCoord][yCoord-1] == NULL && (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX) && (grid[xCoord][yCoord-1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord][yCoord-1] = 15;
        characterTable[xCoord][yCoord-1] = newCharacterOnMap(xCoord, yCoord-1, 15, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 8){
      if(xCoord > 1 && yCoord > 1 && characterTable[xCoord-1][yCoord-1] == NULL && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX) && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX) && (grid[xCoord-1][yCoord-1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord-1][yCoord-1] = 15;
        characterTable[xCoord-1][yCoord-1] = newCharacterOnMap(xCoord-1, yCoord-1, 15, battled);
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else{
      // printf("Inside Here!!!\n");
      *direction = (rand() % 8) + 1;
      // printf("New Direction is %d\n", *direction);
      continue;
    }
  }
  if(count == 9){
    popCharacter(head);
    pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, grid, 1), seqNum, 'w', *direction);
    // characterGrid[xCoord][yCoord] = 15;
    characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, 15, battled);
  }
}


void moveRival(int grid[21][80], Characters *characterTable[21][80], int distanceMap[21][80], Characters **head){
  //check all 8 neighbors to find the smallest value, the smallest value should not be 00
  //move to that space.
  //update next turn
  //pop the queue
  //push the queue
  Characters *peeked = peekCharacter(head);
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int distance = INT_MAX;
  int direction = peeked->direction;
  int minX = 0;
  int minY = 0;
  int battled = characterTable[xCoord][yCoord]->battled;
  // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  free(characterTable[xCoord][yCoord]);
  characterTable[xCoord][yCoord] = NULL;
  //if character table is not taken by a character
  if(battled == 1){
    popCharacter(head);
    characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord,yCoord,11,battled);
  }
  else{
    if(xCoord > 1 && characterTable[xCoord-1][yCoord]==NULL){
      if(distanceMap[xCoord -1][yCoord] != 0 && distanceMap[xCoord -1][yCoord] < distance){
        distance = distanceMap[xCoord -1][yCoord];
        minX = xCoord -1;
        minY = yCoord;
      }
    }

    if(xCoord > 1 && yCoord < 78 && characterTable[xCoord-1][yCoord+1]==NULL){
      if(distanceMap[xCoord -1][yCoord+1] != 0 && distanceMap[xCoord -1][yCoord+1] < distance){
        distance = distanceMap[xCoord -1][yCoord+1];
        minX = xCoord -1;
        minY = yCoord+1;
      }
    }

    if(yCoord < 78 && characterTable[xCoord][yCoord+1]==NULL){
      if(distanceMap[xCoord][yCoord+1] != 0 && distanceMap[xCoord][yCoord+1] < distance){
        distance = distanceMap[xCoord][yCoord+1];
        minX = xCoord;
        minY = yCoord+1;
      }
    }

    if(xCoord < 19 && yCoord < 78 && characterTable[xCoord+1][yCoord+1]==NULL){
      if(distanceMap[xCoord +1][yCoord+1] != 0 && distanceMap[xCoord +1][yCoord+1] < distance){
        distance = distanceMap[xCoord +1][yCoord+1];
        minX = xCoord +1;
        minY = yCoord+1;
      }
    }

    if(xCoord < 19 && characterTable[xCoord+1][yCoord]==NULL){
      if(distanceMap[xCoord +1][yCoord] != 0 && distanceMap[xCoord +1][yCoord] < distance){
        distance = distanceMap[xCoord +1][yCoord];
        minX = xCoord +1;
        minY = yCoord;
      }
    }

    if(xCoord < 19 && yCoord > 1 && characterTable[xCoord+1][yCoord-1]==NULL){
      if(distanceMap[xCoord +1][yCoord-1] != 0 && distanceMap[xCoord +1][yCoord-1] < distance){
        distance = distanceMap[xCoord +1][yCoord-1];
        minX = xCoord +1;
        minY = yCoord-1;
      }
    }

    if(yCoord > 1 && characterTable[xCoord][yCoord-1]==NULL){
      if(distanceMap[xCoord ][yCoord-1] != 0 && distanceMap[xCoord][yCoord-1] < distance){
        distance = distanceMap[xCoord][yCoord-1];
        minX = xCoord;
        minY = yCoord-1;
      }
    }

    if(xCoord > 1 && yCoord > 1 && characterTable[xCoord-1][yCoord-1]==NULL){
      if(distanceMap[xCoord -1][yCoord-1] != 0 && distanceMap[xCoord -1][yCoord-1] < distance){
        distance = distanceMap[xCoord -1][yCoord-1];
        minX = xCoord -1;
        minY = yCoord-1;
      }
    }
    int seqNum = peeked->seqNum;
    if(minX != 0 && minY != 0){
      int nextTurn = (peeked->nextTurn) + costOfTerrain(minX, minY, grid, 1);
      popCharacter(head);
      pushCharacter(head, minX, minY, nextTurn, seqNum, 'r', direction);
      // characterGrid[minX][minY] = 11;
      characterTable[minX][minY] = newCharacterOnMap(minX, minY, 11, battled);
    }
    else{
      int nextTurn = (peeked->nextTurn) + costOfTerrain(xCoord, yCoord, grid, 1);
      popCharacter(head);
      pushCharacter(head, xCoord, yCoord, nextTurn, seqNum, 'r', direction);
      // characterGrid[xCoord][yCoord] = 11;
      characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, 11, battled);
    }
  }

}

void moveHiker(int grid[21][80], Characters *characterTable[21][80], int distanceMap[21][80], Characters **head){
  //check all 8 neighbors to find the smallest value, the smallest value should not be 00
  //move to that space.
  //update next turn
  //pop the queue
  //push the queue
  Characters *peeked = peekCharacter(head);
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int distance = INT_MAX;
  int direction = peeked->direction;
  int minX = 0;
  int minY = 0;
  int battled = characterTable[xCoord][yCoord]->battled;
  // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  free(characterTable[xCoord][yCoord]);
  characterTable[xCoord][yCoord]=NULL;
  //if character table is not taken by a 
  if(battled == 1){
    popCharacter(head);
    characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord,yCoord,10,battled);
  }
  else{
    if(xCoord > 1 && characterTable[xCoord-1][yCoord]==NULL){
      if(distanceMap[xCoord -1][yCoord] != 0 && distanceMap[xCoord -1][yCoord] < distance){
        distance = distanceMap[xCoord -1][yCoord];
        minX = xCoord -1;
        minY = yCoord;
      }
    }

    if(xCoord > 1 && yCoord < 78 && characterTable[xCoord-1][yCoord+1]==NULL){
      if(distanceMap[xCoord -1][yCoord+1] != 0 && distanceMap[xCoord -1][yCoord+1] < distance){
        distance = distanceMap[xCoord -1][yCoord+1];
        minX = xCoord -1;
        minY = yCoord+1;
      }
    }

    if(yCoord < 78 && characterTable[xCoord][yCoord+1]==NULL){
      if(distanceMap[xCoord][yCoord+1] != 0 && distanceMap[xCoord][yCoord+1] < distance){
        distance = distanceMap[xCoord][yCoord+1];
        minX = xCoord;
        minY = yCoord+1;
      }
    }

    if(xCoord < 19 && yCoord < 78 && characterTable[xCoord+1][yCoord+1]==NULL){
      if(distanceMap[xCoord +1][yCoord+1] != 0 && distanceMap[xCoord +1][yCoord+1] < distance){
        distance = distanceMap[xCoord +1][yCoord+1];
        minX = xCoord +1;
        minY = yCoord+1;
      }
    }

    if(xCoord < 19 && characterTable[xCoord+1][yCoord]==NULL){
      if(distanceMap[xCoord +1][yCoord] != 0 && distanceMap[xCoord +1][yCoord] < distance){
        distance = distanceMap[xCoord +1][yCoord];
        minX = xCoord +1;
        minY = yCoord;
      }
    }

    if(xCoord < 19 && yCoord > 1 && characterTable[xCoord+1][yCoord-1]==NULL){
      if(distanceMap[xCoord +1][yCoord-1] != 0 && distanceMap[xCoord +1][yCoord-1] < distance){
        distance = distanceMap[xCoord +1][yCoord-1];
        minX = xCoord +1;
        minY = yCoord-1;
      }
    }

    if(yCoord > 1 && characterTable[xCoord][yCoord-1]==NULL){
      if(distanceMap[xCoord ][yCoord-1] != 0 && distanceMap[xCoord][yCoord-1] < distance){
        distance = distanceMap[xCoord][yCoord-1];
        minX = xCoord;
        minY = yCoord-1;
      }
    }

    if(xCoord > 1 && yCoord > 1 && characterTable[xCoord-1][yCoord-1]==NULL){
      if(distanceMap[xCoord -1][yCoord-1] != 0 && distanceMap[xCoord -1][yCoord-1] < distance){
        distance = distanceMap[xCoord -1][yCoord-1];
        minX = xCoord -1;
        minY = yCoord-1;
      }
    }
  int seqNum = peeked->seqNum;
    if(minX != 0 && minY != 0){
      int nextTurn = (peeked->nextTurn) + costOfTerrain(minX, minY, grid, 0);
      popCharacter(head);
      pushCharacter(head, minX, minY, nextTurn, seqNum, 'h', direction);
      // characterGrid[minX][minY] = 10;
      characterTable[minX][minY] = newCharacterOnMap(minX, minY, 10, battled);
    }else{
      int nextTurn = (peeked->nextTurn) + costOfTerrain(xCoord, yCoord, grid, 0);
      popCharacter(head);
      pushCharacter(head, xCoord, yCoord, nextTurn, seqNum, 'h', direction);
      // characterGrid[xCoord][yCoord] = 10;
      characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, 10, battled);
    }
  }
}

int probabilityOfPokeStuff(int x, int y){
    int probability1 = abs(x - 200) + abs(y - 200);
    if(probability1 < 200){
        int probability = ((-45 * probability1)/200) + 50;
        return probability;
    }
    else{
        return 5;
    }
}

int costOfTerrain(int x, int y, int grid[21][80], int hikerOrRider){//0 for hiker, 1 for rider
  if(hikerOrRider == 0){
    if(grid[x][y] == 2){
      return 10;
    }
    else if(grid[x][y] == 8){
      return 0;
    }
    else if((grid[x][y] == 3) || grid[x][y] == 4){
      return 50;
    }
    else if(grid[x][y] == 5){
      return 15;
    }
    else if(grid[x][y] == 6){
      return 10;
    }
    else if(grid[x][y] == 1){
      return INT_MAX;
    }
    else if(grid[x][y] == 7){
      return INT_MAX;
    }
    else{
      return INT_MAX;
    }
  }
  else if (hikerOrRider == 2){
    if(grid[x][y] == 2){
      return 10;
    }
    else if(grid[x][y] == 8){
      return 0;
    }
    else if((grid[x][y] == 3) || grid[x][y] == 4){
      return 50;
    }
    else if(grid[x][y] == 5){
      return 20;
    }
    else if(grid[x][y] == 6){
      return 10;
    }
    else if(grid[x][y] == 1){
      return INT_MAX;
    }
    else if(grid[x][y] == 7){
      return INT_MAX;
    }
    else{
      return INT_MAX;
    }
  }
  else{
    if(grid[x][y] == 2){
      return 10;
    }
    else if(grid[x][y] == 8){
      return 0;
    }
    else if((grid[x][y] == 3) || grid[x][y] == 4){
      return 10;
    }
    else if(grid[x][y] == 5){
      return 20;
    }
    else if(grid[x][y] == 6){
      return 10;
    }
    else if(grid[x][y] == 1){
      return INT_MAX;
    }
    else if(grid[x][y] == 7){
      return INT_MAX;
    }
    else{
      return INT_MAX;
    }
  }
    return 0;
  
}

void printDistTable(int distance[21][80]){
  for(int i = 0; i < 21; i++){
    for(int j =0; j< 80; j++){
      if(distance[i][j] == INT_MAX){
        printf("   ");
      }
      else if((i==getPCX()) && (j == getPCY())){
        printf("00 ");
      }
      else{
        printf("%02d ", distance[i][j] % 100);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void exitsUnreachable(int distance[21][80]){
  for (int i = 0; i < 21; i++)
  {
    distance[i][0] = INT_MAX;
    distance[i][79] = INT_MAX;
  }
  for (int i = 0; i < 80; i++)
  {
    distance[0][i] = INT_MAX;
    distance[20][i] = INT_MAX;
  }
  
}

void dijkstrasMethod(int grid[21][80], int hikerOrRider, int distance[21][80]){
  int visited[21][80] = {0};//all nodes not visited.
  for(int i = 0; i < 21; i++){
    for(int j =0; j< 80; j++){
      distance[i][j] = INT_MAX;
    }
  }
  distance[pcXcoord][pcYcoord] = 0;


  // printf("Making a queue\n");
  Node* pq = newNode(pcXcoord, pcYcoord, 0);
  // printf("Made a queue\n");
  // printf("%d %d\n", pq->xCoord, pq->yCoord);

  while(!isEmpty(&pq)){
    Node* current = peek(&pq);
    int cost;

    if(visited[current->xCoord][current->yCoord] == 0){

      if(current->xCoord - 1 >= 0 ){
        cost = costOfTerrain(current->xCoord-1, current->yCoord, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord -1][current->yCoord] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord -1][current->yCoord] = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord -1, current->yCoord, distance[current->xCoord -1][current->yCoord]);
        }
        }
      }
       if((current->yCoord + 1 <= 79) && (current->xCoord -1 >= 0)){
        cost = costOfTerrain(current->xCoord -1, current->yCoord +1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord - 1][current->yCoord +1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord - 1][current->yCoord +1]  = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord -1, current->yCoord +1, distance[current->xCoord - 1][current->yCoord +1]);
        }
        }
      }
      if(current->yCoord + 1 <= 79 ){
        cost = costOfTerrain(current->xCoord, current->yCoord +1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord][current->yCoord +1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord][current->yCoord +1] = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord, current->yCoord +1, distance[current->xCoord][current->yCoord +1]);
        }
        }
      }
      if((current->yCoord + 1 <= 79) && (current->xCoord + 1 <= 20)){
        cost = costOfTerrain(current->xCoord +1, current->yCoord +1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord + 1][current->yCoord +1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord + 1][current->yCoord +1]  = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord +1, current->yCoord +1, distance[current->xCoord + 1][current->yCoord +1]);
        }
        }
      }
      if(current->xCoord + 1 <= 20 ){
        cost = costOfTerrain(current->xCoord +1, current->yCoord, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord +1][current->yCoord] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord +1][current->yCoord] = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord +1, current->yCoord, distance[current->xCoord +1][current->yCoord]);
        }
        }
      }
      if((current->yCoord - 1 >= 0) && (current->xCoord +1 <= 20)){
        cost = costOfTerrain(current->xCoord +1, current->yCoord -1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord +1][current->yCoord -1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord +1][current->yCoord -1]  = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord +1, current->yCoord -1, distance[current->xCoord + 1][current->yCoord -1]);
        }
        }
      }
      if(current->yCoord - 1 >= 0 ){
        cost = costOfTerrain(current->xCoord, current->yCoord -1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord][current->yCoord -1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord][current->yCoord -1] = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord, current->yCoord -1, distance[current->xCoord][current->yCoord -1]);
        }
        }
      }
      if((current->yCoord - 1 >= 0) && (current->xCoord - 1 >= 0)){
        cost = costOfTerrain(current->xCoord -1, current->yCoord -1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord -1][current->yCoord -1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord - 1][current->yCoord -1]  = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord -1, current->yCoord -1, distance[current->xCoord - 1][current->yCoord -1]);
        }
        }
      }
    }
    visited[current->xCoord][current->yCoord] = 1;
    pop(&pq);
    if(isEmpty(&pq)){
      break;
    }
  }

  exitsUnreachable(distance);
  // printDistTable(distance);
}
