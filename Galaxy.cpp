#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include <algorithm>
#include <random>

using namespace std;

class CelestialBody { // Базовый класс для небесных тел
protected:
    string name;
public:
    CelestialBody(const string& name) : name(name) {}
    virtual ~CelestialBody() {} // Виртуальный деструктор
    string getName() const { return name; } // Метод для получения названия
};

class Planet : public CelestialBody { // Класс планеты, наследующий от CelestialBody
private:
    double production; // Производство ресурсов
    double demand; // Спрос на ресурсы
    double technology; // Уровень технологии
public:
    Planet(const string& name, double production, double demand, double technology)
        : CelestialBody(name), production(production), demand(demand), technology(technology) {}

    double produceResources() {
        return production; // Возвращаем полное производство
    }

    void improveProduction(double amount) {
        production += amount; // Улучшаем производство на заданное количество
        cout << name << " производство улучшилось на " << amount << " кредитов." << endl;
    }

    double getDemand() const { return demand; }
    double getProduction() const { return production; }
    virtual void printType() const { cout << "Планета: " << name << endl; }
};

class Asteroid : public CelestialBody { // Класс астероида, наследующий от CelestialBody
private:
    double miningValue; // Значение добычи ресурсов
public:
    Asteroid(const string& name, double miningValue)
        : CelestialBody(name), miningValue(miningValue) {}

    double mineResources() {
        return miningValue; // Возвращаем значение добычи ресурсов
    }

    virtual void printType() const { cout << "Астероид: " << name << endl; }
};

class Corporation { // Базовый класс для корпораций
protected:
    string name;
public:
    Corporation(const string& name) : name(name) {}
    virtual ~Corporation() {}
    virtual void transportResources(double& profit) = 0;
    string getName() const { return name; }
};

class LogisticsCompany : public Corporation { // Класс логистической компании
public:
    LogisticsCompany(const string& name) : Corporation(name) {}
    void transportResources(double& profit) override {
        cout << "Транспортировка ресурсов с минимальными затратами за счет " << name << endl;
        profit *= 0.9; // Уменьшаем прибыль на 10%
    }
};

class TechTrader : public Corporation { // Класс трейдера технологий
public:
    TechTrader(const string& name) : Corporation(name) {}
    void transportResources(double& profit) override {
        cout << "Торговля высокотехнологичными ресурсами с " << name << endl;
        profit *= 1.2; // Увеличиваем прибыль на 20%
    }
};

class MinerCompany : public Corporation { // Класс компании-майнера
public:
    MinerCompany(const string& name) : Corporation(name) {}
    void transportResources(double& profit) override {
        cout << "Добыча ресурсов на астероидах с помощью " << name << endl;
        profit += 50; // Увеличиваем прибыль на 50
    }
};

class TradeRoute {
public:
    Planet* from; // Исходная планета
    Planet* to; // Планета назначения
    double cost; // Стоимость маршрута
    double safety; // Уровень безопасности маршрута

    TradeRoute(Planet* from, Planet* to, double cost, double safety)
        : from(from), to(to), cost(cost), safety(safety) {}
};

class Galaxy {
private:
    vector<unique_ptr<Asteroid>> asteroids; // Вектор астероидов
    vector<TradeRoute> tradeRoutes; // Вектор торговых маршрутов
    const double securityThreshold = 5.0; // Порог безопасности для маршрутов
public:
    vector<unique_ptr<Planet>> planets; // Вектор планет

    void addPlanet(unique_ptr<Planet> planet) {
        planets.push_back(move(planet)); // Добавляем планету в вектор
    }

    void addAsteroid(unique_ptr<Asteroid> asteroid) {
        asteroids.push_back(move(asteroid)); // Добавляем астероид в вектор
    }

    void addTradeRoute(Planet* p1, Planet* p2, double cost, double safety) {
        for (const auto& route : tradeRoutes) { // Проверяем, существует ли маршрут между p1 и p2
            if ((route.from == p1 && route.to == p2) || (route.from == p2 && route.to == p1)) {
                cout << "Маршрут уже существует между " << p1->getName() << " и " << p2->getName() << "!" << endl;
                return;
            }
        }
        tradeRoutes.emplace_back(p1, p2, cost, safety);
        cout << "Добавлен новый торговый маршрут из " << p1->getName() << " в " << p2->getName() 
             << " со стоимостью " << cost << " и безопасностью " << safety << endl;
    }

    void simulateTurn(double& totalProfit) {
        for (const auto& planet : planets) { // Симуляция одного игрового хода
            double resources = planet->produceResources();
            cout << planet->getName() << " производит " << resources << " ресурсов." << endl;
            totalProfit += resources; // Добавляем произведенные ресурсы к общей прибыли
        }
        for (const auto& route : tradeRoutes) { // Транспортировка ресурсов
            double transportable = min(route.from->getProduction(), route.to->getDemand());
            double transportProfit = transportable - route.cost; // Прибыль от транспортировки

            totalProfit += transportProfit;
            cout << "Транспортировка " << transportable << " ресурсов из " << route.from->getName() 
                << " в " << route.to->getName() 
                << " со стоимостью " << route.cost 
                << " и безопасностью " << route.safety << endl;

            if (route.safety < securityThreshold) { // Проверяем уровень безопасности маршрута
                double loss = static_cast<double>(rand() % static_cast<int>(transportable)); // Случайные потери
                cout << "Низкая безопасность! Потеряно " << loss << " ресурсов во время транспортировки." << endl;
                totalProfit -= loss; 

                // Наложение штрафа на общую прибыль
                double penalty = 0.1 * loss; // Штраф за утерю ресурсов (10%)
                totalProfit -= penalty;
                cout << "Наложен штраф в размере " << penalty << " на общую прибыль." << endl;
            }
        }

        if (totalProfit < 0) { // Проверка на отрицательную прибыль
            totalProfit = 0; // Никогда не даем отрицательную прибыль
            cout << "Ошибка! Прибыль отрицательна!" << endl;
        }
    }

    void improveTradeRoute(Planet* p1, Planet* p2, double reduction) {
        for (auto& route : tradeRoutes) {
            if (route.from == p1 && route.to == p2) {
                route.cost -= reduction; // Уменьшаем стоимость маршрута
                cout << "Улучшенный торговый маршрут из " << p1->getName() << " в " << p2->getName() 
                     << " за счет снижения затрат на " << reduction << endl;
                return;
            }
        }
        cout << "Торговый маршрут не найден!" << endl; // Если маршрут не найден
    }
};

int main() {
    Galaxy galaxy;
    int numPlanets, numTradeRoutes, numCorporations;
    int maxTurns; // Максимальное количество ходов

    cout << "Введите количество ходов: ";
    while (!(cin >> maxTurns) || maxTurns <= 0) {
        cout << "Неверный ввод. Пожалуйста, введите положительное число." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Введите количество планет: ";
    while (!(cin >> numPlanets) || numPlanets <= 0) {
        cout << "Неверный ввод. Пожалуйста, введите положительное число." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    for (int i = 0; i < numPlanets; ++i) {
        string name;
        double production, demand, technology;

        cout << "Введите название, производство, спрос, технологию для планеты " << (i + 1) << ": ";
        while (!(cin >> name >> production >> demand >> technology) || production < 0 || demand < 0 || technology < 0) {
            cout << "Неверный ввод! Пожалуйста, введите корректные данные." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Введите название, производство, спрос, технологию для планеты " << (i + 1) << ": ";
        }

        galaxy.addPlanet(make_unique<Planet>(name, production, demand, technology));
    }

    cout << "Введите количество торговых маршрутов: ";
    while (!(cin >> numTradeRoutes) || numTradeRoutes < 0) {
        cout << "Неверный ввод. Пожалуйста, введите неотрицательное число." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    for (int i = 0; i < numTradeRoutes; ++i) {
        string name1, name2;
        double cost, safety;

        cout << "Введите названия двух планет, стоимость и уровень безопасности маршрута " << (i + 1) << ": ";
        while (!(cin >> name1 >> name2 >> cost >> safety) || cost < 0 || safety < 0) {
            cout << "Неверный ввод! Пожалуйста, введите корректные данные." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Введите названия двух планет, стоимость и уровень безопасности маршрута " << (i + 1) << ": ";
        }

        Planet* p1 = nullptr;
        Planet* p2 = nullptr;

        for (const auto& planet : galaxy.planets) {
            if (planet->getName() == name1) p1 = planet.get();
            if (planet->getName() == name2) p2 = planet.get();
        }

        if (p1 && p2) {
            galaxy.addTradeRoute(p1, p2, cost, safety);
        } else {
            cout << "Одна или обе планеты не найдены!" << endl;
        }
    }

    cout << "Введите количество корпораций: ";
    while (!(cin >> numCorporations) || numCorporations <= 0) {
        cout << "Неверный ввод. Пожалуйста, введите положительное число." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<unique_ptr<Corporation>> corporations;

    for (int i = 0; i < numCorporations; ++i) {
        string name, type;
        
        cout << "Введите название и тип (Logistics/TechTrader/Miner) корпорации " << (i + 1) << ": ";
        cin >> name >> type;

        unique_ptr<Corporation> corporation;

        if (type == "Logistics") {
            corporation = make_unique<LogisticsCompany>(name);
        } else if (type == "TechTrader") {
            corporation = make_unique<TechTrader>(name);
        } else if (type == "Miner") {
            corporation = make_unique<MinerCompany>(name);
        } else {
            cout << "Неизвестная корпорация!" << endl;
            --i; 
            continue;
        }

        corporations.push_back(move(corporation));
        cout << "Создана корпорация: " << name << " типа " << type << endl;
    }

    double totalProfit = 0;
    char continueGame = 'y';
    int turnCount = 0; // Счетчик ходов

    while (continueGame == 'y' && turnCount < maxTurns) {
        galaxy.simulateTurn(totalProfit);
        for (auto& corporation : corporations) {
            corporation->transportResources(totalProfit);
        }

        cout << "Общая прибыль: " << totalProfit << endl;

        cout << "Выберите действия: (1) Улучшить торговый маршрут (2) Развивать планету (3) Закончить игру: ";
        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Недопустимый выбор! Пожалуйста, выберите 1, 2 или 3." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == 1) {
            string name1, name2;
            double reduction;
            cout << "Введите названия двух планет и сумму уменьшения: ";
            cin >> name1 >> name2 >> reduction;

            Planet* p1 = nullptr;
            Planet* p2 = nullptr;
            for (auto& planet : galaxy.planets) {
                if (planet->getName() == name1) p1 = planet.get();
                if (planet->getName() == name2) p2 = planet.get();
            }

            if (p1 && p2 && reduction >= 0) {
                galaxy.improveTradeRoute(p1, p2, reduction);
            } else {
                cout << "Одна из планет не найдена или неправильное значение снижения!" << endl;
            }
        } else if (choice == 2) {
            string planetName;
            double improvement;
            cout << "Введите название планеты и количество улучшений: ";
            cin >> planetName >> improvement;

            if (improvement < 0) {
                cout << "Неверное значение улучшений! Оно должно быть неотрицательным." << endl;
            } else {
                for (auto& planet : galaxy.planets) {
                    if (planet->getName() == planetName) {
                        planet->improveProduction(improvement);
                        break;
                    }
                }
            }
        } else if (choice == 3) {
            continueGame = 'n';
        } else {
            cout << "Такого выбора не существует!" << endl;
        }
        turnCount++; // Увеличиваем счетчик ходов
        cout << "Ход номер: " << turnCount << "/" << maxTurns << endl; // Показываем номер хода
    }
    cout << "Игра окончена. Итоговая прибыль: " << totalProfit << endl;
    if (turnCount > maxTurns) {
        cout << "Достигнуто максимальное количество ходов. Игра окончена." << endl;
    }

    return 0;
}