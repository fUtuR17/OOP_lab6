#pragma once
#include <vector>
#include <memory>
#include <string>

enum class NpcType;
class Npc;

class NpcFactory {
public:
    static std::shared_ptr<Npc> createNpc(NpcType type, const std::string& name, int x, int y);
    static std::vector<std::shared_ptr<Npc>> loadFromFile(const std::string& filename);
    static void saveToFile(const std::string& filename, const std::vector<std::shared_ptr<Npc>>& npcs);
    
private:
    static NpcType stringToType(const std::string& typeStr);
    static std::string typeToString(NpcType type);
};