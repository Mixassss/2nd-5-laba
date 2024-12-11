#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include <algorithm>
#include <random>

using namespace std;

class CelestialBody { // Базовый класс для небесных тел
protected:
    string name; // Название небесного тела
public:
    CelestialBody(const string& name) : name(name) {}
    virtual ~CelestialBody() {}
    string getName() const { return name; }
    virtual void printType() const = 0; // Виртуальный метод для полиморфизма
};

class Planet : public CelestialBody { // Класс для планет
private:
    double production; // Производство ресурсов
    double demand; // Спрос на ресурсы
    double technology; // Технологический уровень
public:
    Planet(const string& name, double production, double demand, double technology)
        : CelestialBody(name), production(production), demand(demand), technology(technology) {}

    double produceResources() const {
        return production;
    }

    void improveProduction(double amount) {
        production += amount;  // Увеличение производства
        cout << name << " производство улучшилось на " << amount << " кредитов." << endl;
    }

    double getDemand() const { return demand; }
    double getProduction() const { return production; }
    void printType() const override { cout << "Планета: " << name << endl; }
};

class Asteroid : public CelestialBody {  // Класс для астероидов
private:
    double miningValue; // Стоимость добычи астероида
public:
    Asteroid(const string& name, double miningValue)
        : CelestialBody(name), miningValue(miningValue) {}

    double mineResources() const {
        return miningValue; // Возврат стоимости добычи
    }

    void printType() const override { cout << "Астероид: " << name << endl; }
};

class Corporation { // Базовый класс для корпораций
protected:
    string name;
public:
    Corporation(const string& name) : name(name) {}
    virtual ~Corporation() {}
    virtual double transportResources(double& profit) = 0; // Изменен для возвращения прибыли
    string getName() const { return name; }
};

class LogisticsCompany : public Corporation {
public:
    LogisticsCompany(const string& name) : Corporation(name) {}
    double transportResources(double& profit) override {
        cout << "Транспортировка ресурсов с минимальными затратами за счет " << name << endl;
        profit *= 0.9; // Уменьшение прибыли за счет затрат
        return profit; // Возвращаем прибыль
    }
};

class TechTrader : public Corporation {
public:
    TechTrader(const string& name) : Corporation(name) {}
    double transportResources(double& profit) override {
        cout << "Торговля высокотехнологичными ресурсами с " << name << endl;
        profit *= 1.2; // Увеличение прибыли от высоких технологий
        return profit; 
    }
};

class MinerCompany : public Corporation {
public:
    MinerCompany(const string& name) : Corporation(name) {}
    double transportResources(double& profit) override {
        cout << "Добыча ресурсов на астероидах с помощью " << name << endl;
        profit += 50; // Дополнительная прибыль от добычи
        return profit; 
    }
};

class TradeRoute { // Класс для торговых маршрутов между планетами
public:
    Planet* from; // Планета-отправитель
    Planet* to; // Планета-получатель
    double cost; // Стоимость маршрута
    double safety; // Уровень безопасности маршрута

    TradeRoute(Planet* from, Planet* to, double cost, double safety)
        : from(from), to(to), cost(cost), safety(safety) {}
};

class Galaxy { // Класс для галактики, содержащий планеты и астероиды
private:
    vector<unique_ptr<Asteroid>> asteroids; // Вектор астероидов
    vector<TradeRoute> tradeRoutes; // Вектор торговых маршрутов
    const double securityThreshold = 5.0; // Порог безопасности для маршрутов
public:
    vector<unique_ptr<Planet>> planets; // Вектор планет

    void addPlanet(unique_ptr<Planet> planet) {
        planets.push_back(move(planet));
    }

    void addAsteroid(unique_ptr<Asteroid> asteroid) {
        asteroids.push_back(move(asteroid));
    }

    void addTradeRoute(Planet* p1, Planet* p2, double cost, double safety) { // Метод для добавления торгового маршрута
        for (const auto& route : tradeRoutes) { 
            if ((route.from == p1 && route.to == p2) || (route.from == p2 && route.to == p1)) {
                cout << "Маршрут уже существует!" << endl;
                return; // Выход, если маршрут уже существует
            }
        }
        tradeRoutes.emplace_back(p1, p2, cost, safety);
        cout << "Добавлен маршрут из " << p1->getName() << " в " << p2->getName() 
             << " со стоимостью " << cost << " и безопасностью " << safety << endl;
    }

    void simulateTurn(double& totalProfit) {
        for (const auto& planet : planets) {
            double resources = planet->produceResources();
            cout << planet->getName() << " производит " << resources << " ресурсов." << endl;
            totalProfit += resources; // Увеличение общей прибыли
        }
        for (const auto& route : tradeRoutes) {
            double transportable = min(route.from->getProduction(), route.to->getDemand());
            double transportProfit = transportable - route.cost;
            totalProfit += transportProfit; // Увеличение прибыли от транспортировки
            cout << "Транспортировка " << transportable << " из " << route.from->getName() 
                 << " в " << route.to->getName() 
                 << " со стоимостью " << route.cost 
                 << " и безопасностью " << route.safety << endl;

            if (route.safety < securityThreshold) {
                double loss = static_cast<double>(rand() % static_cast<int>(transportable));
                cout << "Потеряно " << loss << " ресурсов во время транспортировки." << endl;
                totalProfit -= loss; // Уменьшение прибыли из-за потерь

                double penalty = 5.0 * loss; 
                totalProfit -= penalty; // Штраф за потерю ресурсов
                cout << "Штраф за потерю: " << penalty << endl;
            }
        }
        if (totalProfit < 0) { // Проверка отрицательной прибыли
            totalProfit = 0;
            cout << "Ошибка! Прибыль отрицательна!" << endl;
        }
    }

    void improveTradeRoute(Planet* p1, Planet* p2, double reduction) { // Метод для улучшения торгового маршрута
        for (auto& route : tradeRoutes) {
            if (route.from == p1 && route.to == p2) {
                route.cost -= reduction; // Уменьшение стоимости маршрута
                cout << "Улучшенный маршрут из " << p1->getName() << " в " << p2->getName() << endl;
                return;
            }
        }
        cout << "Маршрут не найден!" << endl;
    }
};

int main() {
    Galaxy galaxy;
    int numPlanets, numTradeRoutes, numCorporations;
    int maxTurns;

    cout << "Введите количество ходов: ";
    while (!(cin >> maxTurns) || maxTurns <= 0) {
        cout << "Неверный ввод!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Введите количество планет: ";
    while (!(cin >> numPlanets) || numPlanets <= 0) {
        cout << "Неверный ввод!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    for (int i = 0; i < numPlanets; ++i) {
        string name;
        double production, demand, technology;

        cout << "Введите название, производство, спрос, технологию для планеты " << (i + 1) << ": ";
        while (!(cin >> name >> production >> demand >> technology) || production < 0 || demand < 0 || technology < 0) {
            cout << "Неверный ввод!" << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        galaxy.addPlanet(make_unique<Planet>(name, production, demand, technology));
    }

    cout << "Введите количество торговых маршрутов: ";
    while (!(cin >> numTradeRoutes) || numTradeRoutes < 0) {
        cout << "Неверный ввод." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    for (int i = 0; i < numTradeRoutes; ++i) {
        string name1, name2;
        double cost, safety;

        cout << "Введите названия двух планет, стоимость и уровень безопасности маршрута " << (i + 1) << ": ";
        while (!(cin >> name1 >> name2 >> cost >> safety) || cost < 0 || safety < 0) {
            cout << "Неверный ввод!" << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        Planet* p1 = nullptr;
        Planet* p2 = nullptr;

        for (const auto& planet : galaxy.planets) {
            if (planet->getName() == name1) p1 = planet.get(); // Поиск первой планеты
            if (planet->getName() == name2) p2 = planet.get(); // Поиск второй планеты
        }

        if (p1 && p2) {
            galaxy.addTradeRoute(p1, p2, cost, safety); // Добавление маршрута если обе планеты найдены
        } else {
            cout << "Планеты не найдены!" << endl;
        }
    }

    cout << "Введите количество корпораций: ";
    while (!(cin >> numCorporations) || numCorporations <= 0) {
        cout << "Неверный ввод." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<unique_ptr<Corporation>> corporations; // Вектор корпораций

    for (int i = 0; i < numCorporations; ++i) {
        string name, type;
        cout << "Введите название и тип (Logistics/TechTrader/Miner) корпорации " << (i + 1) << ": ";
        cin >> name >> type;

        unique_ptr<Corporation> corporation; 

        if (type == "Logistics") { // Создание корпорации в зависимости от типа
            corporation = make_unique<LogisticsCompany>(name);
        } else if (type == "TechTrader") {
            corporation = make_unique<TechTrader>(name);
        } else if (type == "Miner") {
            corporation = make_unique<MinerCompany>(name);
        } else {
            cout << "Неизвестная корпорация!" << endl;
            --i; 
            continue; // Повтор ввода корпорации
        }

        corporations.push_back(move(corporation));
        cout << "Создана корпорация: " << name << " типа " << type << endl;
    }

    double totalProfit = 0; // Общая прибыль
    char continueGame = 'y'; // Переменная для продолжения игры
    int turnCount = 0; // Счетчик ходов

    while (continueGame == 'y' && turnCount < maxTurns) {
        galaxy.simulateTurn(totalProfit); // Симуляция одного хода
        for (auto& corporation : corporations) {
            corporation->transportResources(totalProfit); // Транспортировка ресурсов каждой корпорацией
        }

        cout << "Общая прибыль: " << totalProfit << endl;

        cout << "Выберите действия: (1) Улучшить маршрут (2) Развивать планету (3) Закончить игру: ";
        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Недопустимый выбор!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (choice == 1) {
            string name1, name2;
            double reduction;
            cout << "Введите данные для улучшения маршрута: ";
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
                cout << "Ошибочные данные!" << endl;
            }
        } else if (choice == 2) {
            string planetName;
            double improvement;
            cout << "Введите данные для улучшения планеты: ";
            cin >> planetName >> improvement;

            if (improvement < 0) {
                cout << "Положительное значение!" << endl;
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
            cout << "Недопустимый выбор!" << endl;
        }
        turnCount++; // Увеличение счетчика ходов
        cout << "Ход номер: " << turnCount << "/" << maxTurns << endl;
    }
    cout << "Игра окончена. Итоговая прибыль: " << totalProfit << endl;
    return 0;
}