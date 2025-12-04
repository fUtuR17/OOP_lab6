#include "NpcFactory.h"
#include "Npc.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::shared_ptr<Npc> NpcFactory::createNpc(NpcType type, const std::string& name, int x, int y) {
    switch(type) {
        case NpcType::Squirrel:
            return std::make_shared<Squirrel>(name, x, y);
        case NpcType::Elf:
            return std::make_shared<Elf>(name, x, y);
        case NpcType::Bandit:
            return std::make_shared<Bandit>(name, x, y);
        default:
            return nullptr;
    }
}

std::vector<std::shared_ptr<Npc>> NpcFactory::loadFromFile(const std::string& filename) {
    std::vector<std::shared_ptr<Npc>> npcs;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string typeStr, name;
        int x, y;
        
        if (iss >> typeStr >> name >> x >> y) {
            NpcType type = stringToType(typeStr);
            npcs.push_back(createNpc(type, name, x, y));
        }
    }
    
    return npcs;
}

void NpcFactory::saveToFile(const std::string& filename, const std::vector<std::shared_ptr<Npc>>& npcs) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    
    for (const auto& npc : npcs) {
        file << typeToString(npc->getType()) << " "
             << npc->getName() << " "
             << npc->getX() << " "
             << npc->getY() << "\n";
    }
}

NpcType NpcFactory::stringToType(const std::string& typeStr) {
    if (typeStr == "Squirrel") return NpcType::Squirrel;
    if (typeStr == "Elf") return NpcType::Elf;
    if (typeStr == "Bandit") return NpcType::Bandit;
    throw std::invalid_argument("Unknown NPC type: " + typeStr);
}

std::string NpcFactory::typeToString(NpcType type) {
    switch(type) {
        case NpcType::Squirrel: return "Squirrel";
        case NpcType::Elf: return "Elf";
        case NpcType::Bandit: return "Bandit";
        default: return "Unknown";
    }
}