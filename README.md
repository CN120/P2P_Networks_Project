# P2P_Networks_Project

Description:
Peer to peer file syncing over TCP

Running on DC Linux machines:
git clone https://github.com/Chris300127/P2P_Networks_Project.git
Everyone run "hostname -i" to get your machine's IP address.
Send your IP to all your peers.
Everyone open "peers.txt" and fill it with all of your peers' IPs.
  For example, if there are 3 peers with IPs 1.1.1.1, 2.2.2.2, and 3.3.3.3,
  then peer 1's peers file should read:
  ```
  2.2.2.2
  3.3.3.3
  ```
  and peer 2's file should read:
  ```
  1.1.1.1
  3.3.3.3
  ```
Navigate to the top level of the git repository.
./start

Compilation on DC Linux machines (shouldn't be necessary, but including
just in case the executable doesn't work):
You shouldn't need to do anything since the DC machines already have it,
but compilation requires the openssl library to be installed.
gcc -Wall -lcrypto -lssl -std=c99 receive.c -o receive
gcc -Wall -lcrypto -lssl -std=c99 push.c -o push
