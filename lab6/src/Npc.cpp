#include "Npc.h"
#include "Visitor.h"

Npc::Npc(NpcType type, const std::string& name, int x, int y) 
    : type(type), name(name), x(x), y(y) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::out_of_range("Coordinates must be between 0 and 500");
    }
}

NpcType Npc::getType() const { return type; }
std::string Npc::getName() const { return name; }
int Npc::getX() const { return x; }
int Npc::getY() const { return y; }

bool Npc::isClose(const std::shared_ptr<Npc>& other, int distance) const {
    if (!other) return false;
    int dx = x - other->getX();
    int dy = y - other->getY();
    return (dx*dx + dy*dy) <= (distance*distance);
}

Squirrel::Squirrel(const std::string& name, int x, int y) 
    : Npc(NpcType::Squirrel, name, x, y) {}

void Squirrel::print() const {
    std::cout << "Squirrel " << name << " at (" << x << ", " << y << ")" << std::endl;
}

std::string Squirrel::getTypeString() const {
    return "Squirrel";
}

void Squirrel::accept(BattleVisitor& visitor, std::shared_ptr<Npc> other) {
    visitor.visit(*this, other);
}

Elf::Elf(const std::string& name, int x, int y) 
    : Npc(NpcType::Elf, name, x, y) {}

void Elf::print() const {
    std::cout << "Elf " << name << " at (" << x << ", " << y << ")" << std::endl;
}

std::string Elf::getTypeString() const {
    return "Elf";
}

void Elf::accept(BattleVisitor& visitor, std::shared_ptr<Npc> other) {
    visitor.visit(*this, other);
}

Bandit::Bandit(const std::string& name, int x, int y) 
    : Npc(NpcType::Bandit, name, x, y) {}

void Bandit::print() const {
    std::cout << "Bandit " << name << " at (" << x << ", " << y << ")" << std::endl;
}

std::string Bandit::getTypeString() const {
    return "Bandit";
}

void Bandit::accept(BattleVisitor& visitor, std::shared_ptr<Npc> other) {
    visitor.visit(*this, other);
}