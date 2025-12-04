#include "Npc.h"
#include "Visitor.h"
#include "Observer.h"
#include <iostream>
#include <memory>
#include <fstream>

int main() {
    std::cout << "=== СИМУЛЯЦИЯ БИТВЫ ===\n\n";
    
    auto elf = std::make_shared<Elf>("Леголас", 10, 10);
    auto bandit = std::make_shared<Bandit>("Бандит", 15, 15);
    auto squirrel = std::make_shared<Squirrel>("Белка", 100, 100);
    auto elf2 = std::make_shared<Elf>("Эльф", 200, 200);
    auto bandit2 = std::make_shared<Bandit>("Разбойник", 5, 5);
    
    std::cout << "Созданы персонажи:\n";
    std::cout << "1. " << elf->getName() << " (" << elf->getTypeString() << ") в позиции (" 
              << elf->getX() << ", " << elf->getY() << ")\n";
    std::cout << "2. " << bandit->getName() << " (" << bandit->getTypeString() << ") в позиции (" 
              << bandit->getX() << ", " << bandit->getY() << ")\n";
    std::cout << "3. " << squirrel->getName() << " (" << squirrel->getTypeString() << ") в позиции (" 
              << squirrel->getX() << ", " << squirrel->getY() << ")\n";
    std::cout << "4. " << elf2->getName() << " (" << elf2->getTypeString() << ") в позиции (" 
              << elf2->getX() << ", " << elf2->getY() << ")\n";
    std::cout << "5. " << bandit2->getName() << " (" << bandit2->getTypeString() << ") в позиции (" 
              << bandit2->getX() << ", " << bandit2->getY() << ")\n\n";
    
    BattleVisitor visitor(50);
    
    visitor.addNpc(elf);
    visitor.addNpc(bandit);
    visitor.addNpc(squirrel);
    visitor.addNpc(elf2);
    visitor.addNpc(bandit2);
    
    auto consoleObserver = std::make_shared<ConsoleObserver>();
    auto fileObserver = std::make_shared<FileObserver>("лог_битвы.txt");
    
    visitor.addObserver(consoleObserver);
    visitor.addObserver(fileObserver);
    
    std::cout << "Начинаем битву с дистанцией атаки 50...\n";
    std::cout << "Наблюдатели: ConsoleObserver, FileObserver (лог_битвы.txt)\n\n";
    
    visitor.battle();
    
    auto survivors = visitor.getSurvivors();
    
    std::cout << "\n=== РЕЗУЛЬТАТЫ БИТВЫ ===\n";
    std::cout << "Выжило: " << survivors.size() << " персонажей\n";
    
    if (survivors.empty()) {
        std::cout << "Все погибли в бою!\n";
    } else {
        for (size_t i = 0; i < survivors.size(); i++) {
            std::cout << i+1 << ". " << survivors[i]->getName() 
                      << " (" << survivors[i]->getTypeString() << ")\n";
        }
    }
    
    std::cout << "\n=== ПРОВЕРКА ФАЙЛА ЛОГА ===\n";
    std::ifstream logFile("лог_битвы.txt");
    if (logFile.is_open()) {
        std::cout << "Файл лога успешно создан.\n";
        
        std::string line;
        int lineCount = 0;
        while (std::getline(logFile, line)) {
            lineCount++;
            if (lineCount <= 3) {
                std::cout << "Лог " << lineCount << ": " << line << "\n";
            }
        }
        std::cout << "Всего записей в логе: " << lineCount << "\n";
        logFile.close();
    } else {
        std::cout << "Ошибка: файл лога не создан!\n";
    }
    
    std::cout << "\n=== СИМУЛЯЦИЯ ЗАВЕРШЕНА ===\n";
    
    return 0;
}