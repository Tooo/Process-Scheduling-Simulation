# PCB's and Process Scheduling Simulation
Travis Chan - 301358957

An interactive operating system simulating basic functions. The simulation recieves keyboard inputs and outputs reports to the screen.

### Process 
#### Process 0 (init)
Once simulations starts, process 0 (init) will be created and run. 
Process 0 cannot be forked and can't recieve if no messages are waiting. 
Process 0 will never be blocked and won't be sent to send waiting list nor semaphore list.
Process 0 only run if it's the only process or all other processes are blocked.
Process 0 can only be killed or exited when there is no processes left. 
Once Process 0 is exited or killed, the simulation terminates.

#### Scheduling Algorithm
The simulation will use a preemptive round robin algorithm with three levels of priority. 
The higher priority processes will continue run and cycle through its own priority.
Lower priority processes will only run if there are no ready processes that are higher priority. 

### Messaging
#### Send
Once message is sent, the sender will be blocked.
If the receiver is waiting for a message, the process will be unblocked and will display the message when the process runs.
Otherwise, the message will be displayed once that process does a recieve.

#### Receive
If the process has a waiting message, the message will be displayed. 
Otherwise, the process will be blocked and wait for a message.

#### Reply
If the reciever was not a sender, the command will fail.
Otherwise, reciever will be unblocked and will display the message when that process runs

#### Multiple Messages
If a process has many recieved messages, the oldest message will be displayed once sender replied or receiver received.
The other messages will be stored and can be displayed after each time the process receives. 

### Semaphore
Each Semaphore can only be created once.
If the same semaphore is created more than twice, the command will fail.
Semaphore P will block current process, if the semaphore value is less than 0.
Semaphore V will unblock a blocked process, if the semaphore value is less or equal to 0.

## Installation
1. Download the zip file and unzip the file.
2. Make the object and executable file: <br> `make`
3. Run the executable: <br>`main`

## Commands
Enter the character to execute the command. 
If there is a parameter, input the parameter after the entering the command. 
If there are multiple parameters, input each parameter one at a time.

### Process Commands
| Command| Parameters | Action |
| :---- |:----------| :-----|
| Create (C) | Priority Number <br> (0 - high, 1 - norm, 2 - low) | Create a process and put in ready Queue |
| Fork (F) | None | Copy running process and put in ready Queue |
| Kill (K) | Process ID | Kill named process and remove from system |
| Exit (E) | None | Kill running process and remove from system |
| Quantum (Q) | None | Time quantum of running process expires|

### Messaging Commands
| Command| Parameters | Action |
| :---- |:----------| :-----|
| Send (S) | Process ID <br> Message (max 40 char)| Send message to another process, block until reply |
| Receive (R) | None | Recieve message, block if no message waiting |
| Reply (Y) | Process ID <br> Message (max 40 char) | Unblock sender and reply message |

### Semaphore Commands
| Command| Parameters | Action |
| :---- |:----------| :-----|
| New Semaphore (N) | Semaphore ID (0-4) <br> Initial Value (=>0) | Initialize semaphore with value given |
| Semaphore P (P) | Semaphore ID (0-4) | Execute semaphore P on running process |
| Semaphore V (V) | Semaphore ID (0-4) | Execute semaphore V on running process |

### Info Commands
| Command| Parameters | Action |
| :---- |:----------| :-----|
| Procinfo (I) | Process ID | Display information of named process |
| Totalinfo (T) | None | Display all process queues and their contents <br> Uninitalized Semaphore queues are not displayed|
