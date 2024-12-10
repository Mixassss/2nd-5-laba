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

        // Транспортировка ресурсов
        for (const auto& route : tradeRoutes) {
            double transportProfit = min(route.from->getProduction(), route.to->getDemand()); 
            totalProfit += transportProfit;
            cout << "Транспортировка " << transportProfit << " ресурсов из " << route.from->getName() 
                << " в " << route.to->getName() 
                << " со стоимостью " << route.cost 
                << " и безопасностью " << route.safety << endl;

            // Проверяем уровень безопасности маршрута
            if (route.safety < securityThreshold) {
                double loss = static_cast<double>(rand() % 30); // Случайные потери
                cout << "Низкая безопасность! Потеряно " << loss << " ресурсов во время транспортировки." << endl;
                totalProfit -= loss; 
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

    cout << "Введите количество планет: ";
    cin >> numPlanets;

    for (int i = 0; i < numPlanets; ++i) {
        string name;
        double production, demand, technology;

        cout << "Введите название, производство, спрос, технологию для планеты " << (i + 1) << ": ";
        cin >> name >> production >> demand >> technology;

        if (cin.fail() || production < 0 || demand < 0 || technology < 0) {
            cout << "Неверный ввод! Пожалуйста, введите положительные числа." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            --i; 
            continue;
        }

        galaxy.addPlanet(make_unique<Planet>(name, production, demand, technology));
    }

    cout << "Введите количество торговых маршрутов: ";
    cin >> numTradeRoutes;

    for (int i = 0; i < numTradeRoutes; ++i) {
        string name1, name2;
        double cost, safety;

        cout << "Введите названия двух планет, стоимость и уровень безопасности маршрута " << (i + 1) << ": ";
        cin >> name1 >> name2 >> cost >> safety;

        Planet* p1 = nullptr;
        Planet* p2 = nullptr;

        for (const auto& planet : galaxy.planets) {
            if (planet->getName() == name1) p1 = planet.get();
            if (planet->getName() == name2) p2 = planet.get();
        }

        if (p1 && p2 && cost >= 0 && safety >= 0) {
            galaxy.addTradeRoute(p1, p2, cost, safety);
        } else {
            cout << "Одна или обе планеты не найдены или неверные значения!" << endl;
        }
    }

    cout << "Введите количество корпораций: ";
    cin >> numCorporations;

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
    while (continueGame == 'y') {
        galaxy.simulateTurn(totalProfit);

        for (auto& corporation : corporations) {
            corporation->transportResources(totalProfit);
        }

        cout << "Общая прибыль: " << totalProfit << endl;

        cout << "Выберите действия: (1) Улучшить торговый маршрут (2) Развивать планету (3) Закончить игру: ";
        int choice;
        cin >> choice;

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

            if (p1 && p2) {
                galaxy.improveTradeRoute(p1, p2, reduction);
            } else {
                cout << "Одна из планет не найдена!" << endl;
            }
        } else if (choice == 2) {
            string planetName;
            double improvement;
            cout << "Введите название планеты и количество улучшений: ";
            cin >> planetName >> improvement;

            for (auto& planet : galaxy.planets) {
                if (planet->getName() == planetName) {
                    planet->improveProduction(improvement);
                    break;
                }
            }
        } else if (choice == 3) {
            continueGame = 'n';
        } else {
            cout << "Такого выбора не существует!" << endl;
        }
    }

    return 0;
}