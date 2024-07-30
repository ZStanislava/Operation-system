/*
7. Даны файл прямого доступа и файл последовательного доступа одинакового размера,
содержащие одинаковую информацию числового типа.
Переписать эти файлы в ОП 10-20 раз с использованием алгоритма чтения «от конца файла».
Характеристика: размер файла (диапазон изменения 100-200 Кбайт).
*/


#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;

#define FileSizeMin 100 * 1024  // Минимальный размер файла (100 Кбайт)
#define FileSizeMax 200 * 1024  // Максимальный размер файла (200 Кбайт)
#define StepSize 10 * 1024      // Шаг изменения размера файла
#define M 10                    // Количество итераций

int main() {
    setlocale(LC_ALL, "Russian");
    __int64 c1, c2, fr;
    byte* MemoryArea;
    int i;
    HANDLE RandomAccessFile = INVALID_HANDLE_VALUE;
    HANDLE SequentialAccessFile = INVALID_HANDLE_VALUE;
    DWORD c;
    float t, v;
    LARGE_INTEGER start, end, frequency;

    QueryPerformanceFrequency(&frequency);

    const wchar_t* RandomAccessFileName = TEXT("r.bin");
    const wchar_t* SequentialAccessFileName = TEXT("s.bin");

    srand(static_cast<unsigned int>(time(NULL)));

    float totalRandomAccessSpeed = 0.0;
    float totalSequentialAccessSpeed = 0.0;

    // Создаем файл для последовательного доступа перед циклом
    SequentialAccessFile = CreateFile(SequentialAccessFileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        0);

    if (SequentialAccessFile == INVALID_HANDLE_VALUE || SequentialAccessFile == NULL) {
        cout << "Ошибка при создании файла последовательного доступа: " << GetLastError() << endl;
        cin.get();
        return 0;
    }

    for (int fileSize = FileSizeMin; fileSize <= FileSizeMax; fileSize += StepSize) {
        if (fileSize > FileSizeMax) {
            break; // Выходим из цикла, если размер файла превышает максимальный размер
        }

        totalRandomAccessSpeed = 0.0;
        totalSequentialAccessSpeed = 0.0;

        for (int z = 0; z < M; z++) {
            // Закрытие предыдущих файлов перед созданием новых
            if (RandomAccessFile != INVALID_HANDLE_VALUE && RandomAccessFile != NULL) {
                CloseHandle(RandomAccessFile);
                RandomAccessFile = INVALID_HANDLE_VALUE; // Установка значения INVALID_HANDLE_VALUE после закрытия
            }

            if (SequentialAccessFile != INVALID_HANDLE_VALUE && SequentialAccessFile != NULL) {
                CloseHandle(SequentialAccessFile);
                SequentialAccessFile = INVALID_HANDLE_VALUE; // Установка значения INVALID_HANDLE_VALUE после закрытия
            }

            RandomAccessFile = CreateFile(RandomAccessFileName,
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                0);

            if (RandomAccessFile == INVALID_HANDLE_VALUE || RandomAccessFile == NULL) {
                cout << "Ошибка при создании файла прямого доступа: " << GetLastError() << endl;
                cin.get();
                return 0;
            }

            SequentialAccessFile = CreateFile(SequentialAccessFileName,
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                0);

            if (SequentialAccessFile == INVALID_HANDLE_VALUE || SequentialAccessFile == NULL) {
                cout << "Ошибка при создании файла последовательного доступа: " << GetLastError() << endl;
                CloseHandle(RandomAccessFile);
                cin.get();
                return 0;
            }

            MemoryArea = (byte*)GlobalAlloc(GPTR, fileSize);

            for (i = 0; i < fileSize; i += sizeof(int)) {
                int randomValue = rand() % 1000 + 1;  // Генерируем случайное число (от 1 до 1000)
                CopyMemory(MemoryArea + i, &randomValue, sizeof(int));
            }

            WriteFile(RandomAccessFile, MemoryArea, fileSize, &c, 0);
            WriteFile(SequentialAccessFile, MemoryArea, fileSize, &c, 0);

            GlobalFree(MemoryArea);

            QueryPerformanceCounter(&start);

            i = fileSize;
            t = 0.0;
            MemoryArea = (byte*)GlobalAlloc(GPTR, i);

            SetFilePointer(RandomAccessFile, 0, NULL, FILE_END); //ПРЯМОЙ ДОСТУП

            for (int j = i - 1; j >= 0; j -= 1) { //читает каждые байт и сдвигает указатель файла на 1 байта назад после каждой операции чтения
                ReadFile(RandomAccessFile, MemoryArea + j, 1, &c, 0);
                SetFilePointer(RandomAccessFile, -1, NULL, FILE_CURRENT);
            }

            QueryPerformanceCounter(&end);
            t = static_cast<float>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
            v = fileSize / (1024 * t);


            totalRandomAccessSpeed += v;

            GlobalFree(MemoryArea);

            QueryPerformanceCounter(&start);

            t = 0.0;
            MemoryArea = (byte*)GlobalAlloc(GPTR, fileSize);

            //Последовательный

            // буфер для считанных данных
            //const int bufferSize = 1; // Размер буфера
            const int bufferSize = fileSize; // Размер буфера
            BYTE buffer[200];

            SetFilePointer(SequentialAccessFile, 0, NULL, FILE_BEGIN);
            for (int j = 0; j < fileSize; j++) {

                SetFilePointer(SequentialAccessFile, 1, NULL, FILE_CURRENT); //побайтово идем до конца
            }
            for (int k = fileSize; k >= 0; k--) {
                // передвигается и читает 
                SetFilePointer(SequentialAccessFile, -1, NULL, FILE_CURRENT);
                SetFilePointer(SequentialAccessFile, -1, NULL, FILE_CURRENT);
                ReadFile(SequentialAccessFile, buffer+fileSize-1, 1, &c, NULL);

            }

            QueryPerformanceCounter(&end);
            t = static_cast<float>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
            v = fileSize / (1024 * t);

            totalSequentialAccessSpeed += v;

            GlobalFree(MemoryArea);
        }

        float averageRandomAccessSpeed = totalRandomAccessSpeed / M;
        float averageSequentialAccessSpeed = totalSequentialAccessSpeed / M;
        cout << "File Size: " << fileSize / 1024 << " Kbytes" << endl;
        cout << "Average Random Access Speed(KB/s): " << averageRandomAccessSpeed << endl;
        cout << "Average Sequential Access Speed(KB/s): " << averageSequentialAccessSpeed << endl;
        std::cout << "\n";
    }

    CloseHandle(RandomAccessFile);
    CloseHandle(SequentialAccessFile);

    cin.get();
    return 0;
}
