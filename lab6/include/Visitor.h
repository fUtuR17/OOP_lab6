#pragma once
#include <memory>
#include <vector>
#include <string>

class Observer;
class Npc;
class Squirrel;
class Elf;
class Bandit;

class BattleVisitor {
private:
    int battleDistance;
    std::vector<std::shared_ptr<Npc>> npcs;
    std::vector<std::shared_ptr<Observer>> observers;
    std::vector<bool> dead;
    
public:
    // BattleVisitor(int distance, std::vector<std::shared_ptr<Npc>>);
    BattleVisitor(int distance);
    void addNpc(std::shared_ptr<Npc> npc);
    void addObserver(std::shared_ptr<Observer> observer);
    void notify(const std::string& message);
    
    void battle();
    std::vector<std::shared_ptr<Npc>> getSurvivors() const;
    
    void visit(Squirrel& squirrel, std::shared_ptr<Npc> other);
    void visit(Elf& elf, std::shared_ptr<Npc> other);
    void visit(Bandit& bandit, std::shared_ptr<Npc> other);
    
private:
    bool isClose(const Npc& a, const Npc& b) const;
    void markAsDead(size_t index);
    void handleBattleResult(Npc* winner, std::shared_ptr<Npc> loser, const std::string& action);
};