# Client-Server Kommunikation
Die Programme server.c und client.c veranschaulichen die Interprozesskommunikation
in einem Client/Server Beispiel.
Über ein Shared Memory Object können die beiden, in unterschiedlichen Terminals
gestarteten, Prozesse miteinander kommunizieren.

### Nutzung

In Terminal 1: server starten mit einem shm-object als Argument
    ./server new_shm

In Terminal 2: client starten mit demselben shm-object als Argument
    ./client new_shm
