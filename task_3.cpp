#include <iostream>
#define MAX 20
using namespace std;

class Bankers {
private:
    int allocation[MAX][MAX];  // Выделенные ресурсы
    int maximum[MAX][MAX];     // Максимальные потребности
    int need[MAX][MAX];        // Оставшиеся потребности
    int available[MAX];        // Доступные ресурсы
    int num_processes;         // Количество процессов
    int num_resources;         // Количество ресурсов
    int safe_sequence[MAX];    // Безопасная последовательность
    int work[MAX];             // Рабочий массив
    bool finish[MAX];          // Флаги завершения процессов

public:
    Bankers() {
        // Инициализация массивов
        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {
                allocation[i][j] = 0;
                maximum[i][j] = 0;
                need[i][j] = 0;
            }
            available[i] = 0;
            safe_sequence[i] = -1;
            work[i] = 0;
            finish[i] = false;
        }
    }

    void input() {
        cout << "Введите количество процессов: ";
        cin >> num_processes;
        cout << "Введите количество ресурсов: ";
        cin >> num_resources;

        // Ввод выделенных ресурсов
        cout << "Сколько ресурсов уже выделено каждому процессу:\n";
        for (int i = 0; i < num_processes; i++) {
            cout << "Процесс " << i << ":\n";
            for (int j = 0; j < num_resources; j++) {
                cout << "Ресурс " << j << ": ";
                cin >> allocation[i][j];
            }
        }

        // Ввод максимальных потребностей
        cout << "Максимальные потребности для каждого процесса:\n";
        for (int i = 0; i < num_processes; i++) {
            cout << "Процесс " << i << ":\n";
            for (int j = 0; j < num_resources; j++) {
                cout << "Ресурс " << j << ": ";
                cin >> maximum[i][j];
                need[i][j] = maximum[i][j] - allocation[i][j];  // Вычисление потребностей
            }
        }

        // Ввод доступных ресурсов
        cout << "Сколько ресурсов каждого типа доступно в системе на данный момент:\n";
        for (int j = 0; j < num_resources; j++) {
            cout << "Ресурс " << j << ": ";
            cin >> available[j];
            work[j] = available[j];  // Инициализация рабочего массива
        }
    }

    bool isSafe() {
        int count = 0;  // Счетчик завершенных процессов
        bool found;

        // Поиск безопасной последовательности
        while (count < num_processes) {
            found = false;
            for (int i = 0; i < num_processes; i++) {
                if (!finish[i]) {
                    bool canAllocate = true;
                    for (int j = 0; j < num_resources; j++) {
                        if (need[i][j] > work[j]) {
                            canAllocate = false;
                            break;
                        }
                    }
                    if (canAllocate) {
                        // Освобождение ресурсов завершенного процесса
                        for (int j = 0; j < num_resources; j++) {
                            work[j] += allocation[i][j];
                        }
                        safe_sequence[count++] = i;
                        finish[i] = true;
                        found = true;
                    }
                }
            }
            if (!found) {
                break;  // Если безопасная последовательность не найдена
            }
        }

        // Проверка, все ли процессы завершены
        if (count < num_processes) {
            return false;  // Система не в безопасном состоянии
        }
        return true;  // Система в безопасном состоянии
    }

    void displaySafeSequence() {
        bool safe = true;
        for (int i = 0; i < num_processes; i++) {
            if (safe_sequence[i] == -1) {
                safe = false;
                break;
            }
        }

        if (safe) {
            cout << "Безопасная последовательность: ";
            for (int i = 0; i < num_processes; i++) {
                cout << "P" << safe_sequence[i] << " ";
            }
            cout << endl;
        } else {
            cout << "Безопасной последовательности нет.\n";
        }
    }
};

int main() {
    Bankers banker;
    banker.input();

    if (banker.isSafe()) {
        cout << "Система находится в безопасном состоянии.\n";
        banker.displaySafeSequence();
    } else {
        cout << "Система не в безопасном состоянии. Возможна взаимоблокировка!\n";
    }

    return 0;
}
