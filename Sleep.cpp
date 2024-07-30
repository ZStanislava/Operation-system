#include <iostream>
#include <signal.h>
#include <unistd.h>

void stop(int signum) {
    std::cout << "Hello" << std::endl;
}

void sleep(int n) {
    if (n <= 0) return;

    struct sigaction act;
    sigset_t mask;

    sigfillset(&mask);
    sigdelset(&mask, SIGALRM);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    act.sa_handler = stop;
    sigaction(SIGALRM, &act, NULL);

    alarm(n);
    pause();
}

int main() {
    int x;
    std::cout << "Enter time: ";
    std::cin >> x;

    sleep(x);

    std::cout << "Bye" << std::endl;

    return 0;
}
