#include "Visitor.h"
#include "Observer.h"
#include "Npc.h"
#include <algorithm>
#include <cmath>

// BattleVisitor::BattleVisitor(int distance, std::vector<std::shared_ptr<Npc>> npcList) : battleDistance(distance), npcs(std::move(npcList)) {}
BattleVisitor::BattleVisitor(int distance) : battleDistance(distance) {}
void BattleVisitor::addNpc(std::shared_ptr<Npc> npc) {
    npcs.push_back(npc);
    dead.push_back(false);
}

void BattleVisitor::addObserver(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void BattleVisitor::notify(const std::string& message) {
    for (const auto& observer : observers) {
        observer->update(message);
    }
}

bool BattleVisitor::isClose(const Npc& a, const Npc& b) const {
    int dx = a.getX() - b.getX();
    int dy = a.getY() - b.getY();
    return (dx*dx + dy*dy) <= (battleDistance * battleDistance);
}

void BattleVisitor::markAsDead(size_t index) {
    if (index < dead.size()) {
        dead[index] = true;
    }
}

void BattleVisitor::handleBattleResult(Npc* winner, std::shared_ptr<Npc> loser, const std::string& action) {
    notify(winner->getName() + " (" + winner->getTypeString() + ") " + action + " " + 
           loser->getName() + " (" + loser->getTypeString() + ")");
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (npcs[i].get() == loser.get()) {
            markAsDead(i);
            break;
        }
    }
}

void BattleVisitor::visit(Squirrel& squirrel, std::shared_ptr<Npc> other) {
    if (other->getType() == NpcType::Elf) {
        handleBattleResult(&squirrel, other, "killed");
    }
}

void BattleVisitor::visit(Elf& elf, std::shared_ptr<Npc> other) {
    if (other->getType() == NpcType::Bandit) {
        handleBattleResult(&elf, other, "killed");
    }
}

void BattleVisitor::visit(Bandit& bandit, std::shared_ptr<Npc> other) {
    if (other->getType() == NpcType::Squirrel) {
        handleBattleResult(&bandit, other, "killed");
    }
}

void BattleVisitor::battle() {
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (dead[i]) continue;
        
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            if (dead[j]) continue;
            
            if (isClose(*npcs[i], *npcs[j])) {
                npcs[i]->accept(*this, npcs[j]);
                npcs[j]->accept(*this, npcs[i]);
            }
        }
    }
    
    std::vector<std::shared_ptr<Npc>> survivors;
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (!dead[i]) {
            survivors.push_back(npcs[i]);
        }
    }
    
    npcs = std::move(survivors);
}

std::vector<std::shared_ptr<Npc>> BattleVisitor::getSurvivors() const {
    std::vector<std::shared_ptr<Npc>> survivors;
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (!dead[i]) {
            survivors.push_back(npcs[i]);
        }
    }
    return survivors;
}