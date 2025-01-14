#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <set>
#include <algorithm>
#include <chrono>

using namespace std;

// Класс для замера времени
class Timer {
public:
    Timer() : start(chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Время выполнения: " << duration.count() << " мс" << endl;
    }

private:
    chrono::time_point<chrono::high_resolution_clock> start;
};

class DominoSet {
public:
    DominoSet(int maxValue) : maxValue(maxValue) {
        // Генерация полного набора домино
        for (int i = 0; i <= maxValue; ++i) {
            for (int j = i; j <= maxValue; ++j) {
                fullSet.insert({i, j});
            }
        }
    }

    void addDomino(int a, int b) {
        lock_guard<mutex> lock(mx);
        currentSet.insert({min(a, b), max(a, b)});
    }

    void findMissingDominoes(int start, int end) {
        auto it = fullSet.begin();
        advance(it, start);
        for (int i = start; i < end; ++i, ++it) {
            if (currentSet.find(*it) == currentSet.end()) {
                lock_guard<mutex> lock(mx);
                missingDominoes.insert(*it);
            }
        }
    }

    void printMissingDominoes() {
        //cout << "Недостающие кости домино: " << endl;
        for (const auto& domino : missingDominoes) {
            //cout << "[" << domino.first << "|" << domino.second << "] ";
        }
        cout << endl;
    }

    int getFullSetSize() const {
        return fullSet.size();
    }

private:
    int maxValue; // Максимальное значение на кости домино
    set<pair<int, int>> fullSet; // Полный набор домино
    set<pair<int, int>> currentSet; // Текущий набор домино
    set<pair<int, int>> missingDominoes; // Недостающие кости домино
    mutex mx;
};

int main() {
    int choice;
    cout << "Выберите версию домино:\n"
         << "1. Классическая (до [6|6])\n"
         << "2. Расширенная (до [30|30])\n"
         << "Ваш выбор: ";
    cin >> choice;

    int maxValue;
    if (choice == 1) {
        maxValue = 6;
    } else if (choice == 2) {
        maxValue = 30;
    } else {
        cout << "Неверный выбор. Используется классическая версия (до [6|6]).\n";
        maxValue = 6;
    }

    DominoSet dominoSet(maxValue);

    // Добавление существующих костей домино (не все кости добавлены)
    dominoSet.addDomino(0, 0);
     

    // Однопоточная обработка
    {
        Timer t;
        cout << "Однопоточная обработка: " << endl;
        dominoSet.findMissingDominoes(0, dominoSet.getFullSetSize());
        dominoSet.printMissingDominoes();
    }

    // Многопоточная обработка
    int countThread;
    cout << "Введите количество потоков: ";
    cin >> countThread;
    int chunkSize = dominoSet.getFullSetSize() / countThread;
    {
        Timer t;
        cout << "Многопоточная обработка: " << endl;
        vector<thread> threads(countThread);
        for (int i = 0; i < countThread; ++i) {
            int start = i * chunkSize;
            int end = (i == countThread - 1) ? dominoSet.getFullSetSize() : start + chunkSize;
            threads[i] = thread([&dominoSet, start, end]() { dominoSet.findMissingDominoes(start, end); });
        }
        for (int i = 0; i < countThread; ++i) {
            threads[i].join();
        }
        dominoSet.printMissingDominoes();
    }

    return 0;
}