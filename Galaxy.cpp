#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <limits>

using namespace std;

class CelestialBody {
protected:
    string name;
public:
    CelestialBody(const string& name) : name(name) {}
    virtual ~CelestialBody() {}
    string getName() const { return name; }
};

class Planet : public CelestialBody {
private:
    double production;
    double demand;
    double technology;
public:
    Planet(const string& name, double production, double demand, double technology)
        : CelestialBody(name), production(production), demand(demand), technology(technology) {}

    double produceResources() {
        return production; 
    }

    void improveProduction(double amount) {
        production += amount;
        cout << name << " production improved by " << amount << " units." << endl;
    }

    double getDemand() const { return demand; }
};

class Asteroid : public CelestialBody {
private:
    double miningValue;
public:
    Asteroid(const string& name, double miningValue)
        : CelestialBody(name), miningValue(miningValue) {}

    double mineResources() {
        return miningValue; 
    }
};

class Corporation {
protected:
    string name;
public:
    Corporation(const string& name) : name(name) {}
    virtual ~Corporation() {}
    virtual void transportResources() = 0;  
};

class LogisticsCompany : public Corporation {
public:
    LogisticsCompany(const string& name) : Corporation(name) {}

    void transportResources() override {
        cout << "Транспортировка с минимальными затратами за счет " << name << endl;
    }
};

class TechTrader : public Corporation {
public:
    TechTrader(const string& name) : Corporation(name) {}

    void transportResources() override {
        cout << "Торговля высокотехнологичными ресурсами с помощью " << name << endl;
    }
};

class MinerCompany : public Corporation {
public:
    MinerCompany(const string& name) : Corporation(name) {}

    void transportResources() override {
        cout << "Добыча ресурсов на астероидах с помощью " << name << endl;
    }
};

class Galaxy {
private:
    vector<Asteroid*> asteroids;
    map<pair<Planet*, Planet*>, double> tradeRoutes; 

public:
    vector<Planet*> planets;
    void addPlanet(Planet* planet) {
        planets.push_back(planet);
    }

    void addAsteroid(Asteroid* asteroid) {
        asteroids.push_back(asteroid);
    }

    void addTradeRoute(Planet* p1, Planet* p2, double cost) {
        tradeRoutes[{p1, p2}] = cost;
    }

    void simulateTurn() {
        for (auto planet : planets) {
            cout << planet->getName() << " produces " << planet->produceResources() << " resources." << endl;
        }

        for (const auto& route : tradeRoutes) {
            cout << "Transporting resources from " << route.first.first->getName() << " to "
                  << route.first.second->getName() << " with cost " << route.second << endl;
        }
    }

    void improveTradeRoute(Planet* p1, Planet* p2, double reduction) {
        if (tradeRoutes.find({p1, p2}) != tradeRoutes.end()) {
            tradeRoutes[{p1, p2}] -= reduction;
            cout << "Improved trade route from " << p1->getName() << " to " << p2->getName() 
                 << " by reducing cost by " << reduction << endl;
        } else {
            cout << "Trade route not found!" << endl;
        }
    }
};

int main() {
    Galaxy galaxy;

    int numPlanets, numTradeRoutes, numCorporations;

    cout << "Введите номер планеты: ";
    cin >> numPlanets;

    for (int i = 0; i < numPlanets; ++i) {
        string name;
        double production, demand, technology;

        cout << "Введите название, производство, спрос, технологию для планеты " << (i + 1) << ": ";
        cin >> name >> production >> demand >> technology;

        galaxy.addPlanet(new Planet(name, production, demand, technology));
    }

    cout << "Введите количество торговых маршрутов: ";
    cin >> numTradeRoutes;

    for (int i = 0; i < numTradeRoutes; ++i) {
        string name1, name2;
        double cost;

        cout << "Введите названия двух планет и стоимость торгового маршрута " << (i + 1) << ": ";
        cin >> name1 >> name2 >> cost;

        Planet* p1 = nullptr;
        Planet* p2 = nullptr;

        for (auto planet : galaxy.planets) {
            if (planet->getName() == name1) p1 = planet;
            if (planet->getName() == name2) p2 = planet;
        }

        if (p1 && p2) {
            galaxy.addTradeRoute(p1, p2, cost);
        } else {
            cout << "Одна или обе планеты не найдены!" << endl;
        }
    }

    cout << "Введите количество корпораций: ";
    cin >> numCorporations;

    for (int i = 0; i < numCorporations; ++i) {
        string name, type;
        cout << "Введите название и тип (Logistics/TechTrader/Miner) корпорации " << (i + 1) << ": ";
        cin >> name >> type;

        if (type == "Logistics") {
            new LogisticsCompany(name);
        } else if (type == "TechTrader") {
            new TechTrader(name);
        } else if (type == "Miner") {
            new MinerCompany(name);
        } else {
            cout << "Неизвестная корпорация!" << endl;
        }
    }

    char continueGame = 'y';
    while (continueGame == 'y') {
        galaxy.simulateTurn();

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

            for (auto planet : galaxy.planets) {
                if (planet->getName() == name1) p1 = planet;
                if (planet->getName() == name2) p2 = planet;
            }

            if (p1 && p2) {
                galaxy.improveTradeRoute(p1, p2, reduction);
            } else {
                cout << "Одна из нескольких планет не найдено!" << endl;
            }
        } else if (choice == 2) {
            string planetName;
            double improvement;
            cout << "Введите название планеты и количество улучшений: ";
            cin >> planetName >> improvement;

            for (auto planet : galaxy.planets) {
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