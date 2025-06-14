# C HTTP REST API Project

This project focuses on building a simple HTTP REST API in C, primarily involving socket programming and understanding the HTTP protocol. The API will manage a non-persistent "todo list" in memory.

---

## Table of Contents

- [Learning Objectives](#learning-objectives)
- [General Requirements](#general-requirements)
- [More Information](#more-information)
- [REST API Details](#rest-api-details)
- [Tasks](#tasks)
  - [0. Listen](#0-listen)
  - [1. Accept](#1-accept)
  - [2. Client](#2-client)
  - [3. Roger](#3-roger)
  - [4. REST API - The Request](#4-rest-api---the-request)

---

## Learning Objectives

Upon completion of this project, you should be able to explain the following concepts without external assistance:

### General Socket Concepts

*   **Socket Representation:** What is a socket and how is it represented on a Linux/UNIX system?
*   **Socket Types:** The different types of sockets (e.g., `SOCK_STREAM`, `SOCK_DGRAM`).
*   **Socket Domains:** The various socket domains (e.g., `AF_INET`, `AF_UNIX`).
*   **Socket Creation:** How to create a socket using `socket()`.
*   **Binding a Socket:** How to bind a socket to a specific address and port using `bind()`.
*   **Listening and Accepting Connections:** How to listen for and accept incoming network traffic using `listen()` and `accept()`.
*   **Connecting to a Remote Application:** How to establish a connection to a remote application using `connect()`.
*   **HTTP Protocol:** A fundamental understanding of the HTTP protocol.
*   **Simple HTTP Server:** How to create a basic HTTP server.

---

## General Requirements

*   **Allowed Editors:** `vi`, `vim`, `emacs`
*   **Compilation Environment:** All files will be compiled on Ubuntu 20.04 LTS.
*   **C Compiler Flags:** C programs and functions will be compiled with `gcc 9.*` using the flags: `-Wall -Werror -Wextra -pedantic`.
*   **File Endings:** All files must end with a new line.
*   **README:** A `README.md` file at the root of the project folder is mandatory.
*   **Coding Style:** Your code should adhere to the Betty style. It will be checked using `betty-style.pl` and `betty-doc.pl`.
*   **Function Limit:** A maximum of 5 functions per file is allowed.
*   **Header Guards:** All header files must be include guarded.
*   **Standard Library:** Unless otherwise specified, the C standard library is allowed.

---

## More Information

*   **REST API Testing:** For the REST API portion, you can easily test your HTTP server using tools like `curl`, Postman, or even your web browser.
*   **Data Structures:** You are free to use any data structure that suits your needs, provided their purpose is well-defined.

---

## REST API Details

The primary goal of this project is to build a simple HTTP REST API in C to manage a "TODO list". The data will be stored in RAM, meaning there is no persistent storage; the todo list will be empty each time the server starts.

### Todo Item Structure

A todo item must contain at least the following fields:

*   **`id`**: Positive integer, starting from 0.
*   **`title`**: String.
*   **`description`**: String.

### API Routes

The following routes must be implemented:

*   **`/todos`**
    *   **`POST`**: Creates a new todo item.
    *   **`GET`**: Retrieves all todo items.
*   **`/todos?id={id}`** (where `{id}` is a positive integer)
    *   **`GET`**: Retrieves the todo item with the corresponding ID.
    *   **`DELETE`**: Deletes the todo item with the corresponding ID.

---

## Tasks

### 0. Listen

**Mandatory**

Write a program that opens an IPv4/TCP socket and listens for traffic on port `12345` (any address).

*   Your program must hang indefinitely (it will eventually be killed by a signal during correction).
*   You are not required to accept incoming connections for this task.
*   Your program's output will not be checked; you can output any information you like.

**Example Usage:**

```bash
alex@~/sockets$ gcc -Wall -Wextra -Werror -pedantic -o 0-server 0-server.c
alex@~/sockets$ ./0-server
Server listening on port 12345
```

**Verification (in a second terminal):**

```bash
alex@~/sockets$ lsof -i :12345
COMMAND    PID    USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
0-server 24895 vagrant    3u  IPv4 251230      0t0  TCP *:12345 (LISTEN)
alex@~/sockets$ killall 0-server
alex@~/sockets$
```

**Repository Details:**

*   **GitHub Repository:** `atlas-system_linux`
*   **Directory:** `sockets`
*   **File:** `0-server.c`

---

### 1. Accept

**Mandatory**

Write a program that opens an IPv4/TCP socket and listens for traffic on port `12345` (any address).

*   Your program must accept an incoming connection.
*   Print the IP address of the connected client.
*   Close the connection immediately after.

**Example Output:**

```bash
alex@~/sockets$ gcc -Wall -Wextra -Werror -pedantic -o 1-server 1-server.c
alex@~/sockets$ ./1-server
Server listening on port 12345
Client connected: 127.0.0.1
alex@~/sockets$
```

**Testing:**

To test your server, you can use the program `1-client` available on this GitHub.

**Example Client Usage (in a second terminal):**

```bash
alex@~/sockets$ ./1-client localhost 12345
Connected to localhost:12345
alex@~/sockets$
```

**Repository Details:**

*   **GitHub Repository:** `atlas-system_linux`
*   **Directory:** `sockets`
*   **File:** `1-server.c`

---

### 2. Client

**Mandatory**

Write a program that connects to a listening server.

*   **Usage:** `2-client <host> <port>`
    *   `<port>` is an unsigned short.
*   **Error Handling:** If the number of command-line arguments is insufficient, your program must display: `Usage: %s <host> <port>` (where `%s` is replaced by the program name), followed by a new line, and exit with `EXIT_FAILURE`.
*   **Port Assumption:** You can assume that `<port>` will be a positive integer.
*   **Success Message:** Once your client establishes a connection with the server, it must print a message and exit successfully.

**Example Server Usage (in a first terminal):**

```bash
alex@~/sockets$ ./2-server
Server listening on port 12345
Client connected: 127.0.0.1
```

**Example Client Usage (in a second terminal):**

```bash
alex@~/sockets$ gcc -Wall -Wextra -Werror -pedantic -o 2-client 2-client.c
alex@~/sockets$ ./2-client localhost 12345
Connected to localhost:12345
alex@~/sockets$
```

**Testing:**

To test your client, you can use the program `2-server` available on this GitHub.

**Repository Details:**

*   **GitHub Repository:** `atlas-system_linux`
*   **Directory:** `sockets`
*   **File:** `2-client.c`

---

### 3. Roger

**Mandatory**

Write a program that opens an IPv4/TCP socket and listens for traffic on port `12345` (any address).

Your program must:

*   Accept an incoming connection.
*   Print the IP address of the connected client.
*   Wait for an incoming message from the connected client.
*   Print the received message.
*   Close the connection with the client.
*   Exit.

**Example Output:**

```bash
alex@~/sockets$ gcc -Wall -Wextra -Werror -pedantic -o 3-server 3-server.c
alex@~/sockets$ ./3-server
Server listening on port 12345
Client connected: 127.0.0.1
Message received: "Holberton School !!!"
alex@~/sockets$
```

**Testing:**

To test your server, you can use the program `3-client` available on this GitHub.

**Example Client Usage (in a second terminal):**

```bash
alex@~/sockets$ ./3-client localhost 12345 'Holberton School !!!'
Connected to localhost:12345
Sending the message: "Holberton School !!!"
Sent
alex@~/sockets$
```

**Repository Details:**

*   **GitHub Repository:** `atlas-system_linux`
*   **Directory:** `sockets`
*   **File:** `3-server.c`

---

### 4. REST API - The Request

**Mandatory**

Write a program that opens an IPv4/TCP socket and listens for traffic on port `8080` (any address).

Your program must:

*   Accept an incoming connection.
*   Print the IP address of the connected client.
*   Wait for an incoming message from the connected client.
*   Print the full received HTTP request.
*   Print the breakdown of the first line of the received HTTP request (see example).
*   Send back an HTTP `200 OK` response to the connected client.
*   Close the connection with the client.
*   Wait for the next connection.

**Important Notes:**

*   The client will send an `HTTP/1.1` request. Refer to the [RFC 2616 (HTTP/1.1)](https://www.rfc-editor.org/rfc/rfc2616) for details. (Tip: Sections 4, 5, 6, and 10 are particularly useful).
*   You can assume that all requests sent to your server will always be valid `HTTP/1.1` requests; no tricky or malformed requests will be sent.
*   Your `Makefile` must define the rule `todo_api_0`, which compiles and links your sources to form an executable with the same name.

**Example Server Usage (in a terminal):**

```bash
alex@~/sockets$ make todo_api_0
[...]
alex@~/sockets$ ./todo_api_0
Server listening on port 8080
Client connected: 127.0.0.1
Raw request: "GET /test HTTP/1.1
User-Agent: curl/7.35.0
Host: localhost:8080
Accept: */*

"
Method: GET
Path: /test
Version: HTTP/1.1
Client connected: 127.0.0.1
Raw request: "POST /holberton HTTP/1.1
User-Agent: curl/7.35.0
Host: localhost:8080
Accept: */*

"
Method: POST
Path: /holberton
Version: HTTP/1.1
^C
alex@~/sockets$
```

**Example Client Usage (in a second terminal):**

```bash
alex@~/sockets$ curl -D - 'http://localhost:8080/test' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
^M$
alex@~/sockets$ curl -D - -X POST 'http://localhost:8080/holberton' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
^M$
alex@~/sockets$
```

**Repository Details:**

*   **GitHub Repository:** `atlas-system_linux`
*   **Directory:** `sockets`
*   **File:** `Makefile`