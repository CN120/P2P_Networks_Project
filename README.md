# P2P_Networks_Project

Description:
Peer to peer file syncing over TCP

Running on DC Linux machines:
1. Navigate to the folder you want to download the git repo in.

2. Run: git clone https://github.com/Chris300127/P2P_Networks_Project.git

3. All peers run: "hostname -i" to get everyone's machine's IP address.
4. All peers send their IPs to a designated peer. Only one peer in the group needs
to know everyone's IPs.
  Note: peers.txt is a file in the synced repository. The easiest way to set up
  the first time is for everyone to send their IP address to one designated peer.
  That peer then fills peers.txt with those IPs, runs ./start, then runs ./push.
5. Navigate to the top level of the git repository (cd P2P_Networks_Project).
6. The designated peer must open "peers.txt" and fill it with all of their peers' IPs.
  For example, if there are 3 peers total in the group with IPs 1.1.1.1, 2.2.2.2,
  and 3.3.3.3, then everyone's peers.txt file should read:

  1.1.1.1
  2.2.2.2
  3.3.3.3

7. Run: ./start

8. Once all peers have run "./start", they can start editing and adding files in
the P2P_Networks_Project/repo and calling "./push" to push their changes. The
application is now running and fully functional.
9. Once a peer is done and wants to end their application, they run "./end".

Compilation on DC Linux machines (shouldn't be necessary, but including
instructions just in case the executable doesn't work for some reason):
Compilation requires the openssl library to be installed, but you shouldn't need
to deal with this since the DC machines already have it.
1. Before running these commands, git clone the repository and navigate into its
top level.

2. gcc -Wall -lcrypto -lssl -std=c99 receive.c -o receive

3. gcc -Wall -lcrypto -lssl -std=c99 push.c -o push

4. After compiling both receive.c and push.c, skip to step 3 of the Running
instructions above.
