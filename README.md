# FileSystem
Project to simulate a distributed file system following EXT3

**Authors: 
Vinícius Vaz,
Nathan Marcelino**

## STEP 1:
**GOAL:**
Develop a distributed file system based on EXT3 template from Linux O.S family

1. The system must allow local files to be acessed by remote users simultaneously.
2. The server must be acessed by socket TCP
3. Each conection will be managed by a thread
4. No user authentication required
5. Concurrent access to the same file must be guaranteed using semaphores / mutexes

We used Netcat app to simulate the client.


## SETP 2:
**GOAL:**
Extend step 1 by developing the file server allocation and organization structure.

1. The file server must follow the same guidelines as the EXT3 file system.
2. The server disk must be simulated through a single file in the file system of the operating systems where the server is running.



