#pragma once
#include <string>
#include <memory>
#include <iostream>

enum class NpcType { Squirrel, Elf, Bandit };

class BattleVisitor;

class Npc {
protected:
    NpcType type;
    std::string name;
    int x, y;
public:
    Npc(NpcType type, const std::string& name, int x, int y);
    virtual ~Npc() = default;

    NpcType getType() const;
    std::string getName() const;
    int getX() const;
    int getY() const;

    virtual void print() const = 0;
    virtual std::string getTypeString() const = 0;

    bool isClose(const std::shared_ptr<Npc>& other, int distance) const;
    virtual void accept(BattleVisitor& visitor, std::shared_ptr<Npc> other) = 0;
};

class Squirrel : public Npc {
public:
    Squirrel(const std::string& name, int x, int y);
    void print() const override;
    std::string getTypeString() const override;
    void accept(BattleVisitor& visitor, std::shared_ptr<Npc> other) override;
};

class Elf : public Npc {
public:
    Elf(const std::string& name, int x, int y);
    void print() const override;
    std::string getTypeString() const override;
    void accept(BattleVisitor& visitor, std::shared_ptr<Npc> other) override;
};

class Bandit : public Npc {
public:
    Bandit(const std::string& name, int x, int y);
    void print() const override;
    std::string getTypeString() const override;
    void accept(BattleVisitor& visitor, std::shared_ptr<Npc> other) override;
};