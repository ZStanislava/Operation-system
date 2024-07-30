
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

DWORD WINAPI WorkThread(LPVOID param)
{
    int threadNum = (int)param + 1;
    Sleep(10000);
    cout << "Поток " << threadNum << " завершен" << endl;
    ExitThread(0);
}

int main()
{
    setlocale(LC_ALL, "Rus");
    int num;
    int count;
    HANDLE hArr[5]{};
    srand(time(NULL));

    for (int i = 0; i < 3; ++i) {

        cout << "Номер испытания: " << i + 1 << endl;

        // создать 5 потоков
        for (int i = 0; i < 5; ++i) {
            hArr[i] = CreateThread(NULL, 0, WorkThread, (LPVOID)i, CREATE_SUSPENDED, 0);
        }

       
        for (int j = 0; j < 70; ++j) {
            num = rand() % 5 + 1; 

            
            ResumeThread(hArr[num - 1]); // Проверка  ResumeThread

            DWORD resumeWaitResult = WaitForSingleObject(hArr[num - 1], 0); //проверяет сразу в каком он сотоянии находится в сигнальном или не сигнальном
            // в течение 0 миллисекунд-то есть сразу
            if (resumeWaitResult == WAIT_TIMEOUT) {
                cout << "Секунда: " << j + 1 << ", Поток " << num << " активирован" << endl;
            }
            else {
                cout << "Секунда: " << j + 1 << ", Поток " << num << " уже завершен" << endl;
            }

            Sleep(1000);

       
            SuspendThread(hArr[num - 1]);

            DWORD suspendWaitResult = WaitForSingleObject(hArr[num - 1], 0);      // Проверка  SuspendThread
            if (suspendWaitResult == WAIT_TIMEOUT) {
                cout << "Секунда: " << j + 2 << ", Поток " << num << " приостановлен" << endl;
            }
            else {
                cout << "Секунда: " << j + 2 << ", Поток " << num << " уже завершен" << endl;
            }
        }

        // проверка, завершены ли потоки
        count = 0;
        for (int i = 0; i < 5; ++i) {
            DWORD exitCode;
            GetExitCodeThread(hArr[i], &exitCode);
            if (exitCode != STILL_ACTIVE) {
                count += 1;
            }
        }

        cout << "Потоков завершено: " << count << endl << endl << endl;
    }

    return 0;
}
