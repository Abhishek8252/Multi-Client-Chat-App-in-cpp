# Multi-Client Chat Application

A Linux-based multi-client chat application implemented in **C++** using **TCP sockets**, **multithreading**, and the **Readline library**. The application enables multiple clients to communicate simultaneously through a centralized server with support for both public group messaging and private direct messages.

---

## Features

- Multi-client TCP chat server
- Concurrent client handling using threads
- Public group chat
- Private messaging using `@username`
- User join/leave notifications
- Interactive command-line interface using Readline
- Thread-safe client management using mutexes
- Linux compatible
- Simple Makefile build

---

## Technologies

- C++
- POSIX Socket Programming
- TCP/IP Networking
- Multithreading (std::thread)
- Mutex Synchronization
- GNU Readline
- Linux

---

## Project Structure

```
.
├── client.cpp
├── server.cpp
├── Makefile
├── README.md
```

---

## Build

Compile both applications:

```bash
make
```

---

## Run

### Terminal 1

```bash
./server
```

### Terminal 2

```bash
./client
```

### Terminal 3

```bash
./client
```

Connect as many clients as required.

---

## Usage

### Enter Username

```
Enter your username:
Alice
```

---

### Group Chat

```
Me: Hello Everyone!
```

All connected users receive

```
[Alice]: Hello Everyone!
```

---

### Private Message

```
@Bob Hi Bob!
```

Bob receives

```
[DM from Alice]: Hi Bob!
```

---

### Exit

```
/exit
```

---

## Working

### Server

- Creates a TCP socket.
- Listens for incoming client connections.
- Accepts multiple clients concurrently.
- Creates one thread per client.
- Stores usernames and socket descriptors.
- Broadcasts messages to all connected users.
- Supports private messaging.
- Detects client disconnects.

### Client

- Connects to the server.
- Sends username after connecting.
- Starts a background receiver thread.
- Uses GNU Readline for an interactive terminal.
- Sends messages continuously.
- Supports private messages.
- Gracefully disconnects using `/exit`.

---

## Example

### Alice

```
Me: Hello
```

### Bob

```
[Alice]: Hello
```

### Alice

```
@Bob Are you available?
```

### Bob

```
[DM from Alice]: Are you available?
```

---

## Learning Outcomes

- TCP Socket Programming
- Client-Server Architecture
- Concurrent Programming
- Thread Synchronization
- Mutexes
- Network Communication
- Readline Integration
- Direct Messaging
- Broadcast Communication
- Linux System Programming

---

## Future Improvements

- Authentication and Passwords
- Chat Rooms / Channels
- Message History
- File Transfer
- TLS/SSL Encryption
- Voice Chat
- Emoji Support
- User Presence Status
- Logging
- Epoll / Select based scalable server
- IPv6 Support

---

## License

MIT License
