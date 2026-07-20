#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

map<int, string> clients; // fd -> username
map<string, int> user_fd; // username -> fd
mutex clients_mtx;

void broadcast(string message, int sender_fd)
{
    lock_guard<mutex> lock(clients_mtx);
    for (auto const &[fd, name] : clients)
    {
        if (fd != sender_fd)
        {
            send(fd, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_fd)
{
    char buffer[1024];

    // First message is username
    int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes <= 0)
    {
        close(client_fd);
        return;
    }

    string username(buffer, bytes);

    {
        lock_guard<mutex> lock(clients_mtx);
        clients[client_fd] = username;
        user_fd[username] = client_fd;
    }

    broadcast(username + " has joined the chat!", client_fd);
    cout << "[LOG] " << username << " connected.\n";

    while (true)
    {
        bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes <= 0)
        {
            broadcast(username + " has left the chat.", client_fd);
            cout << "[LOG] " << username << " disconnected.\n";

            lock_guard<mutex> lock(clients_mtx);
            clients.erase(client_fd);
            user_fd.erase(username);

            close(client_fd);
            return;
        }

        string msg(buffer, bytes);

        if (msg[0] == '@')
        {
            int space_pos = msg.find(' ');
            if (space_pos != string::npos)
            {
                string target = msg.substr(1, space_pos - 1);
                string body = msg.substr(space_pos + 1);

                int target_fd = -1;

                {
                    lock_guard<mutex> lock(clients_mtx);
                    if (user_fd.count(target))
                        target_fd = user_fd[target];
                }

                if (target_fd != -1)
                {
                    string out = "[DM from " + username + "]: " + body;
                    send(target_fd, out.c_str(), out.size(), 0);

                    // Optional confirmation to sender
                    // send(client_fd, ("[You -> " + target + "]: " + body).c_str(), body.size() + target.size() + 10, 0);
                }
                else
                {
                    string err = "User not found\n";
                    send(client_fd, err.c_str(), err.size(), 0);
                }

                continue;
            }
        }
        else
        {
            // NORMAL BROADCAST
            broadcast("[" + username + "]: " + msg, client_fd);
        }
    }
}

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = {AF_INET, htons(8080), INADDR_ANY};
    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    while (true)
    {
        int client_fd = accept(server_fd, nullptr, nullptr);
        thread(handle_client, client_fd).detach();
    }
}