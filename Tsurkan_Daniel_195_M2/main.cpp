#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#pragma ide diagnostic ignored "EndlessLoop"

int const costOfBullet = 10; //Стоимость одного снаряда
int const costOfObj = 100; //Стоимость одного объекта
int const sizeZone = 10; //Размер поля
int const minCountOfObjectsOnLine = 1; //Минимальное количество объектов на одной линии
int const maxCountOfObjectsOnLine = sizeZone; //Максимальное количество объектов на одной линии
int const minLoadTime = 1000; //Минимальное время зарядки орудия (мс)
int const maxLoadTime = 3000; //Максимальное время зарядки орудия (мс)
int const minDelayTime = 100; //Минимальная задержка при выстреле (мс)
int const maxDelayTime = 300; //Максимальное задерэка при выстреле (мс)


std::condition_variable cv;
std::mutex show; //мьютекс для корректного отображения сообщений
std::mutex volley; //мьютекс залпа для условной переменной
int countLoaded = 0; //количество заряженных орудий
bool startVolley; //начался ли залп

class Zone {
private:
    bool** zone;
    std::mutex zoneMtx;
    std::string name;
    int countOfObjects;
    int startCountOfObjects;

public:
    /**
     * Создает зону
     * @param name имя зоны
     */
    Zone(std::string name) {
        startCountOfObjects = 0;
        zone = new bool*[sizeZone];
        for (int i = 0; i < sizeZone; ++i) {
            zone[i] = new bool[sizeZone];
            int countOfObjects = 0;
            int countObjectsOnLine = rand() % (maxCountOfObjectsOnLine - minCountOfObjectsOnLine)
                    + minCountOfObjectsOnLine;
            for (int j = 0; j < sizeZone; ++j) {
                if (countOfObjects < countObjectsOnLine && rand() % 2 == 0) {
                    zone[i][j] = true;
                    countOfObjects++;
                } else
                    zone[i][j] = false;
            }
            startCountOfObjects += countOfObjects;
            this->countOfObjects = startCountOfObjects;
        }
        this->name = name;
    }

    /**
     * Атакует переданный сектор
     * @param i
     * @param j
     * @return
     */
    bool attack(int i, int j) {
        zoneMtx.lock();
        bool value = zone[i][j];
        zoneMtx.unlock();
        if (value) {
            zoneMtx.lock();
            zone[i][j] = false;
            zoneMtx.unlock();
            countOfObjects--;
            show.lock();
            printf("[%s] Object in sector (%d, %d) was destroyed\n", name.c_str(), i, j);
            show.unlock();
        } else {
            show.lock();
            printf("[%s] There was nothing in sector (%d, %d)\n", name.c_str(), i, j);
            show.unlock();
        }
        return value;
    }

    /**
     * Выводит зону на экран
     */
    void print() {
        std::cout << name << ":" << std::endl;
        for (int i = 0; i < sizeZone; ++i) {
            for (int j = 0; j < sizeZone; ++j) {
                if (zone[i][j])
                    std::cout << "o ";
                else
                    std::cout << "# ";
            }
            std::cout << "\n";
        }
    }

    bool isAlive() {
        return countOfObjects > 0;
    }

    int getAllCost() {
        return startCountOfObjects * costOfObj;
    }
};

void mortar(int id, std::string team, Zone* enemy, int randKey) {
    srand(randKey);
    //Занимает позицию
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000 + 4000));
    show.lock();
    printf("[%s : Martian %d] Took position!\n", team.c_str(), id);
    show.unlock();
    //Заряжает орудие
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (maxLoadTime - minLoadTime) + minLoadTime));
    show.lock();
    printf("[%s : Martian %d] The gun is loaded!\n", team.c_str(), id);
    countLoaded++;
    show.unlock();
    while (true) {
        //Ждет приказа стрелять
        std::unique_lock<std::mutex> volleyLock(volley);
        while (!startVolley)
            cv.wait(volleyLock);
        volleyLock.unlock();
        //Выбирает сектор для обстрела
        int x = rand() % sizeZone;
        int y = rand() % sizeZone;
        show.lock();
        printf("[%s : Martian %d] Attacking zone (%d, %d)\n", team.c_str(), id, x, y);
        show.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (maxDelayTime - minDelayTime) + minDelayTime));
        enemy->attack(x, y);
        //Заряжает орудие
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % (maxLoadTime - minLoadTime) + minLoadTime));
        show.lock();
        printf("[%s : Martian %d] The gun is loaded!\n", team.c_str(), id);
        countLoaded++;
        show.unlock();
    }
}

int readNumber(int minVal, int maxValue) {
    int number;
    std::cin >> number;
    while (number < minVal && number > maxValue) {
        std::cout << "Incorrect number...";
        std::cin >> number;
    }
    return number;
}

int main() {
    srand(time(0));
    std::cout << "Enter count of mortars in first army:";
    int countOfMortarsFirstArmy = readNumber(1, 50);
    std::cout << "Enter count of mortars in second army:";
    int countOfMortarsSecondArmy = readNumber(1, 50);
    //Создаем и выводим поля
    Zone zone1("Anchuria");
    Zone zone2("Taranttery");
    zone1.print();
    zone2.print();
    //Создаем потоки
    std::thread* mortars1 = new std::thread[countOfMortarsFirstArmy];
    std::thread* mortars2 = new std::thread[countOfMortarsSecondArmy];
    std::cout << "[Main Thread] Prepare your weapons for the battle!\n";
    for (int i = 0; i < countOfMortarsFirstArmy; ++i)
        mortars1[i] = std::thread(mortar, i + 1, "Anchuria", &zone2, rand());
    for (int i = 0; i < countOfMortarsSecondArmy; ++i)
        mortars2[i] = std::thread(mortar, i + 1, "Taranttery", &zone1, rand());

    int cost1 = 0;
    int cost2 = 0;
    //Начинаем войну до того момента пока кто-то не победит или стоимось снарядов не привысит
    //Стоимости варжеских сооружений
    while (zone1.isAlive() && zone2.isAlive() &&
        cost1 < zone2.getAllCost() && cost2 < zone1.getAllCost()) {
        while (countLoaded < countOfMortarsSecondArmy + countOfMortarsFirstArmy)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        countLoaded = 0;
        show.lock();
        printf("[Main Thread] Fire!!!\n");
        show.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        startVolley = true;
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        startVolley = false;
        cost1 += costOfBullet * countOfMortarsFirstArmy;
        cost2 += costOfBullet * countOfMortarsSecondArmy;
        show.lock();
        zone1.print();
        zone2.print();
        printf("Anchuria bullets cost: %d; Taranttery Objects Cost: %d\nTaranttery bullets cost: %d; Anchuria Objects Cost: %d\n", cost1,
               zone2.getAllCost(), cost2, zone1.getAllCost());
        show.unlock();
    }

    //Подводим итоги битвы
    if (zone1.isAlive() && zone2.isAlive())
        std::cout << "The war is too expensive...\n";
    else if (zone2.isAlive())
        std::cout << "Taranttery win!!!\n";
    else
        std::cout << "Anchuria win!!!\n";

    for (int i = 0; i < countOfMortarsFirstArmy; ++i)
        mortars1[i].detach();
    for (int i = 0; i < countOfMortarsSecondArmy; ++i)
        mortars2[i].detach();

    delete[] mortars1;
    delete[] mortars2;

    return 0;
}
