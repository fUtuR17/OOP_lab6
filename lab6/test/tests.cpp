#include <gtest/gtest.h>
#include "Npc.h"
#include "Visitor.h"
#include "Observer.h"
#include "NpcFactory.h"
#include <fstream>
#include <memory>

TEST(NpcTest, CreateNpcTypes) {
    auto squirrel = std::make_shared<Squirrel>("Squirrel1", 10, 20);
    EXPECT_EQ(squirrel->getName(), "Squirrel1");
    EXPECT_EQ(squirrel->getX(), 10);
    EXPECT_EQ(squirrel->getY(), 20);
    
    auto elf = std::make_shared<Elf>("Elf1", 30, 40);
    EXPECT_EQ(elf->getName(), "Elf1");
    EXPECT_EQ(elf->getX(), 30);
    EXPECT_EQ(elf->getY(), 40);
    
    auto bandit = std::make_shared<Bandit>("Bandit1", 50, 60);
    EXPECT_EQ(bandit->getName(), "Bandit1");
    EXPECT_EQ(bandit->getX(), 50);
    EXPECT_EQ(bandit->getY(), 60);
}

TEST(NpcTest, DistanceCalculation) {
    auto npc1 = std::make_shared<Squirrel>("S1", 0, 0);
    auto npc2 = std::make_shared<Elf>("E1", 3, 4);
    
    EXPECT_TRUE(npc1->isClose(npc2, 5));
    EXPECT_TRUE(npc1->isClose(npc2, 6));
    EXPECT_FALSE(npc1->isClose(npc2, 4));
}

TEST(NpcFactoryTest, CreateNpcFromFactory) {
    auto squirrel = NpcFactory::createNpc(NpcType::Squirrel, "FactorySquirrel", 1, 2);
    ASSERT_NE(squirrel, nullptr);
    EXPECT_EQ(squirrel->getName(), "FactorySquirrel");
    
    auto elf = NpcFactory::createNpc(NpcType::Elf, "FactoryElf", 3, 4);
    ASSERT_NE(elf, nullptr);
    EXPECT_EQ(elf->getName(), "FactoryElf");
    
    auto bandit = NpcFactory::createNpc(NpcType::Bandit, "FactoryBandit", 5, 6);
    ASSERT_NE(bandit, nullptr);
    EXPECT_EQ(bandit->getName(), "FactoryBandit");
}

TEST(NpcFactoryTest, FileSaveLoad) {
    std::vector<std::shared_ptr<Npc>> originalNpcs;
    originalNpcs.push_back(std::make_shared<Squirrel>("Sq1", 100, 200));
    originalNpcs.push_back(std::make_shared<Elf>("Elf1", 300, 400));
    originalNpcs.push_back(std::make_shared<Bandit>("Band1", 50, 150));
    
    const std::string testFilename = "test_npcs.txt";
    
    NpcFactory::saveToFile(testFilename, originalNpcs);
    
    std::ifstream infile(testFilename);
    EXPECT_TRUE(infile.good());
    infile.close();
    
    auto loadedNpcs = NpcFactory::loadFromFile(testFilename);
    EXPECT_EQ(loadedNpcs.size(), originalNpcs.size());
    
    for (size_t i = 0; i < loadedNpcs.size(); ++i) {
        EXPECT_EQ(loadedNpcs[i]->getName(), originalNpcs[i]->getName());
        EXPECT_EQ(loadedNpcs[i]->getX(), originalNpcs[i]->getX());
        EXPECT_EQ(loadedNpcs[i]->getY(), originalNpcs[i]->getY());
    }
    
    std::remove(testFilename.c_str());
}

TEST(BattleTest, BasicBattle) {
    BattleVisitor visitor(50);
    
    auto elf = std::make_shared<Elf>("Elf1", 0, 0);
    auto bandit = std::make_shared<Bandit>("Bandit1", 10, 10);
    
    visitor.addNpc(elf);
    visitor.addNpc(bandit);
    
    visitor.battle();
    auto survivors = visitor.getSurvivors();
    
    EXPECT_TRUE(survivors.size() >= 0 && survivors.size() <= 2);
}

TEST(BattleTest, NoBattleWhenFar) {
    BattleVisitor visitor(5);
    
    auto elf = std::make_shared<Elf>("Elf1", 0, 0);
    auto bandit = std::make_shared<Bandit>("Bandit1", 100, 100);
    
    visitor.addNpc(elf);
    visitor.addNpc(bandit);
    
    visitor.battle();
    auto survivors = visitor.getSurvivors();
    
    EXPECT_EQ(survivors.size(), 2);
}

TEST(BattleTest, MultipleNpcs) {
    BattleVisitor visitor(100);
    
    visitor.addNpc(std::make_shared<Elf>("Elf1", 0, 0));
    visitor.addNpc(std::make_shared<Bandit>("Bandit1", 50, 50));
    visitor.addNpc(std::make_shared<Squirrel>("Squirrel1", 25, 25));
    visitor.addNpc(std::make_shared<Elf>("Elf2", 75, 75));
    
    visitor.battle();
    auto survivors = visitor.getSurvivors();
    
    EXPECT_TRUE(survivors.size() >= 0 && survivors.size() <= 4);
}

TEST(ObserverTest, ConsoleObserver) {
    ConsoleObserver observer;
    EXPECT_NO_THROW(observer.update("Test message"));
}

TEST(ObserverTest, FileObserver) {
    const std::string filename = "test_log.txt";
    
    {
        FileObserver observer(filename);
        observer.update("Message 1");
        observer.update("Message 2");
    }
    
    std::ifstream file(filename);
    EXPECT_TRUE(file.is_open());
    
    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        count++;
    }
    file.close();
    
    EXPECT_GE(count, 2);
    
    std::remove(filename.c_str());
}

TEST(NpcTest, TypeStrings) {
    auto squirrel = std::make_shared<Squirrel>("Sq", 0, 0);
    auto elf = std::make_shared<Elf>("Elf", 0, 0);
    auto bandit = std::make_shared<Bandit>("Bandit", 0, 0);
    
    EXPECT_FALSE(squirrel->getTypeString().empty());
    EXPECT_FALSE(elf->getTypeString().empty());
    EXPECT_FALSE(bandit->getTypeString().empty());
}

TEST(NpcTest, AcceptVisitor) {
    auto elf = std::make_shared<Elf>("Elf1", 0, 0);
    auto bandit = std::make_shared<Bandit>("Bandit1", 10, 10);  // Противник
    BattleVisitor visitor(100);
    EXPECT_NO_THROW(elf->accept(visitor, bandit));
}

TEST(NpcFactoryTest, EmptyFile) {
    const std::string emptyFile = "empty.txt";
    
    std::ofstream out(emptyFile);
    out.close();
    
    auto npcs = NpcFactory::loadFromFile(emptyFile);
    EXPECT_TRUE(npcs.empty());
    
    std::remove(emptyFile.c_str());
}

TEST(BattleTest, SingleNpc) {
    BattleVisitor visitor(100);
    
    auto elf = std::make_shared<Elf>("Elf1", 0, 0);
    visitor.addNpc(elf);
    
    visitor.battle();
    auto survivors = visitor.getSurvivors();
    
    EXPECT_EQ(survivors.size(), 1);
}