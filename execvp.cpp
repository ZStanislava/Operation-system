#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

// Вспомогательная функция - ищем файл с именем file в путях, содержащихся
// в переменной PATH
char *findfile(const char *file)//Функция возвращает указатель на строку с полным путем к найденному файлу.
{
    char *pathvar;//объявляет указатель pathvar
    pathvar = getenv("PATH");// для получения значения переменной окружения PATH. Функция возвращает указатель на строку с этим значением, и этот указатель присваивается переменной pathvar.
    char *fname = (char*)malloc(strlen(pathvar) + strlen(file) + 2);
    fname[0] = '\0';
    // разбиваем на токены используя двоеточие как разделитель
    char *pch = strtok(pathvar, ":");
    while (pch != NULL) {
        // в fname объединим путь и имя файла, разделив их слэшем
        strcpy(fname, pch);
        strcat(fname, "/");
        strcat(fname, file);
        // если файл существует - то используем его для запуска
        if (access(fname, F_OK) == 0) {
            break;
        }
        pch = strtok (NULL, ":");
        fname[0] = '\0';
    }

    return fname;
}

int execvp(const char *file, char *const argv[])
{
    int res = 0;

    // Путь абсолютный - используем как есть
    if (file[0] == '/') {
        res = execv(file, argv);
    }
    else {//поиск файла в переменной окружения PATH, используя функцию findfile. Если файл успешно найден, он выполняется через execv.
        char *fname = findfile(file);
        if (fname[0] != '\0') {
            res = execv(fname, argv);
        }
        else {
            // Если не нашли команду, то вызовем execv с несуществующим
            // файлом и вернем его код возврата
            res = execv(file, argv);
        }
        free(fname);
    }

    return res;
}

int main()
{
    char *argv[] = {"/bin/ls", "/tmp", "/home", NULL};
    execvp("ls", argv);

    return 0;
}
