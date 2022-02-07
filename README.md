# realtime
POSIX Timers, Signals, Message Queues and Real-time Scheduling

## Scheduling
The script periodicTask implements a POSIX signal which is envoked periodically, executes a generic load (to simulate some task) and outupts a warning if the deadline (equal to the period) is missed. The period, priority, CPU load factor and scheduling policy can be assigned thru command line arguments. Arbitrarily many periodic tasks can be executet on the same processor to see how the real time system handles the scheduling and how the different command line arguments affect deadline misses. This litte function demonstrates the inner workings of scheduling in real time systems.

To execute the code you need be on a POSIX compliant operating system and type:

```
gcc -Wall -o myprogram periodicTask.c -lrt

./myprogram --period 500 --priority 1 --cpu-load 10000 --shed-policy SCHED_FIFO

```

This video shows how two tasks run on the same processor:

https://user-images.githubusercontent.com/96864967/152888194-281665c9-b9d8-434a-863b-6b9134257ecd.mp4

https://user-images.githubusercontent.com/96864967/152888281-8a3b5eca-9ef9-402a-8c87-5a5579925863.mp4

## Message Queues
The scripts in the folder messageQueue demonstrate interprocess commnuication with POSIX message queues. Some clients communicate with a server. The clients send string messages to the server and the server responds. Each client has it's own queue depending on its process id and receives an unique token as an answer from the server. An arbitrary amount of clients can be created to communicate with the server.

open a terminal an start the server
```
gcc -Wall -o server server.c

./server

```

open a second terminal and start one client
```
gcc -Wall -o client1 client.c

./client1

```
do the same for every additional client 
