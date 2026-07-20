#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Readline
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

std::mutex cout_mtx;  // serialize printing vs readline redraw

void receive_handler(int fd)
{
    char buf[1024];
    while (true)
    {
        int bytes = recv(fd, buf, sizeof(buf), 0);
        if (bytes <= 0)
            _exit(0); 

        std::string incoming(buf, bytes);

        // Use Readline helpers to print message without ruining the input line
        std::lock_guard<std::mutex> lk(cout_mtx);

        rl_save_prompt();            // save current prompt text
        rl_on_new_line();            // tell readline we're on a new line
        rl_crlf();                   // move cursor to a clean line (like printing '\n')

        // Print the incoming message (it may or may not have a trailing newline)
        std::cout << incoming;
        if (!incoming.empty() && incoming.back() != '\n')
            std::cout << '\n';
        std::cout.flush();

        // Restore prompt + current input line and redraw
        rl_on_new_line();
        rl_redisplay();
    }
}

int main()
{
    string name;
    std::cout << "Enter your username: ";
    std::getline(std::cin, name);

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return -1;

    // Send username immediately after connecting
    send(fd, name.c_str(), name.size(), 0);

    std::thread(receive_handler, fd).detach();

    // Readline-based input loop
    while (true)
    {
        // Show prompt and get a line the user is editing
        char* line = readline("Me: ");

        if (line == nullptr) {
            // EOF (Ctrl+D) — treat like /exit
            break;
        }

        std::string msg(line);
        free(line);  // readline allocates; free after use

        if (msg == "/exit")
            break;

        if (!msg.empty()) {
            add_history(msg.c_str()); // optional: store in history (Up/Down)
            send(fd, msg.c_str(), msg.size(), 0);
        }
    }

    close(fd);
    return 0;
}