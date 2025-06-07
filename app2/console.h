#pragma once

#include "serializable.h"
#include "game.h"
#include <string>
#include <unordered_map>

class Console : public Serializable {
    int id;
    int storageSpace;
    std::string name;
    std::unordered_map<int, Game> games;
public:
    Console() : id(0), storageSpace(0) {}
    Console(int id, int storageSpace, std::string name) : id(id), storageSpace(storageSpace), name(std::move(name)) {}
    
    void printEntry() const;
    void printAllGames() const;
    
    Game* getGame(int id);
    const Game* getGame(int id) const;

    void serialize(std::ofstream& file) const override;
    void deserialize(std::ifstream& file) override;
    
    int getId() const;
    std::string getName() const;
};
