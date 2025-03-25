# Multi-Threaded Server and Client System Programming

This program can open and close pipes, send IP and Port to workers, and is no longer able to print statistics. Workers can receive country data, compile statistics, send them to the server, and I was very close to enabling waiting for requests from the server. I have fixed many issues, so the statistics and the overall process work as expected. 

The server is multithreaded with a thread pool where workers are queued. The threads read if the connection is from a worker or a client. When not accepting requests, they add data to the queue. The thread function can print the statistics it has received and handle IP, port, and worker count data. It can also accept client queries and print them. There is a function for handling queries, although it doesn't perform any actions yet because I haven’t worked on queries. Finally, there’s a function that starts the connections for queries and statistics.

The client is single-threaded, though a significant portion of the code is commented out because I could not resolve some issues related to multi-threading.

---

## Structure

- **BASH Folder**: Contains the shell script and two files with countries and diseases.
- **MASTER Folder**: Contains files from the second exercise.
  - **BUILD Folder**: Contains all `.o` files.
  - **HEADERS Folder**: Contains all `.h` files.
  - **SRC Folder**: Contains all `.c` files.
  - **files Folder**: Stores pipe files.
  - **Makefile**
- **SERVER Folder**: Contains everything needed to run the server.
  - **BUILD Folder**: Contains all `.o` files.
  - **HEADERS Folder**: Contains all `.h` files.
  - **SRC Folder**: Contains all `.c` files.
  - **Makefile**
- **CLIENT Folder**: Contains everything needed for the client to run.
  - **BUILD Folder**: Contains all `.o` files.
  - **HEADERS Folder**: Contains all `.h` files.
  - **SRC Folder**: Contains all `.c` files.
  - **queries Folder**: Contains an empty file where queries will be written.
  - **Makefile**
- **README**: This file.

Each folder has its own Makefile, so you need to run `make` in all three (MASTER, SERVER, CLIENT) folders.

---

## Features of the Program

I have made significant improvements to my second exercise, and here’s what the **MASTER** program can do:
- It can read all countries and send them to the workers.
- It can open and close pipes.
- It sends the IP and Port to the workers, though it can no longer print the statistics.
- Workers can receive countries, compile statistics, and send them to the server. I was very close to implementing the ability to wait for requests from the server.
- I have fixed many things, so the statistics and the whole process seem to be working properly.

### SERVER:
- The server is **multithreaded** with a pool of threads that are placed in a queue.
- Threads can read if the connection comes from a worker or a client. When they are not accepting requests, they add data to the queue.
- The thread function can print the statistics it has received (though I commented it out to avoid cluttering the terminal).
- It can accept IPs, ports, and the number of workers.
- It can also accept queries from the client and print them.
- There is a function for handling queries, though it doesn’t do anything because I haven't worked on that feature yet.
- Finally, there’s a function that starts the connections for queries and statistics.

### CLIENT:
- The client is **single-threaded**.
- While the multi-threaded code is written, it is commented out due to some unresolved bugs, and it would be appreciated if you could review that section.
- The client code supports multi-threading, but due to bugs, it currently runs as a single thread.

---

## Limitations of the Program

- The program **cannot process queries**.
- It **cannot open communication** from the server to the workers to pass queries.
- The **client is not multi-threaded** (though the code for multi-threading exists, but it’s currently not functioning correctly).
- There may be other minor issues I’ve overlooked.

### Notes:
- Running **MASTER** may occasionally fail. If this happens, simply run it again.
