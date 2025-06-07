#pragma once

#include "serializable.h"
#include "game.h"

class Console : public Serializable {
    int id;
    int storageSpace;
    std::string name;
    std::unordered_map<int, Game> games;
public:
    Console() : id(0), storageSpace(0) {}
    Console(const int id, const int storageSpace, std::string name) : id(id), storageSpace(storageSpace), name(std::move(name)) {}
    void printEntry() const;
    void printAllGames() const;
    void modify(std::string newName, int storageSpace);
    bool addGame(Game& game);
    bool removeGame(int id);
    Game* getGame(const int id);
    void serialize(std::ofstream& file) const override;
    void deserialize(std::ifstream& file) override;
    int getId() const;
};
