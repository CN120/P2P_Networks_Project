# P2P_Networks_Project

Description:
Peer to peer file syncing over TCP

Running on DC Linux machines:
1. Navigate to the folder you want to download the git repo into.

2. Run: git clone https://github.com/Chris300127/P2P_Networks_Project.git

3. All peers run: "hostname -i" to get everyone's machine's IP address.
4. Everyone sends their IP to all their peers. Everyone open "peers.txt" and
fill it with all of your peers' IPs.  
  For example, if there are 3 peers with IPs 1.1.1.1, 2.2.2.2, and 3.3.3.3,
  then peer 1's peers file should read:

  2.2.2.2  
  3.3.3.3

  peer 2's file should read:

  1.1.1.1  
  3.3.3.3

  and peer 3's file should read:

  1.1.1.1  
  2.2.2.2

5. After everyone's peers file is correct, run: ./start
6. Once all peers have run "./start", they can start editing and adding files in
the P2P_Networks_Project/repo and calling "./push" to push their changes. The
application is now running and fully functional.
7. Once a peer is done and wants to end their application, they run "./end".

Compilation on DC Linux machines (this really shouldn't be necessary, but we're
including instructions just in case the executable doesn't work for some
reason):  
  Note: Compilation requires the openssl library to be installed, but you
  shouldn't need to deal with this since the DC machines already have it.
1. Before running these commands, git clone the repository and navigate into its
top level.

2. gcc -Wall -lcrypto -lssl -std=c99 receive.c -o receive

3. gcc -Wall -lcrypto -lssl -std=c99 push.c -o push

4. After compiling both receive.c and push.c, skip to step 3 of the Running
instructions above.
