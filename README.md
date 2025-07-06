🔗 Distributed Crypto Miner
A lightweight, modular, and scalable distributed cryptocurrency miner built in C, designed to split and execute mining tasks across multiple nodes—perfect for Raspberry Pi clusters, embedded systems, or small-scale compute farms.

⚙️ Features
📡 TCP-Based Communication: Reliable socket communication between leader and workers.

🧠 Leader-Worker Architecture: Centralized control node (leader) distributes jobs to compute nodes (workers).

📈 Performance-Oriented: Designed for low-resource devices.

🔐 Pluggable Algorithm Support: Easily swap in different mining algorithms (e.g. SHA256, Scrypt, etc.).

🧱 Architecture
[ Leader Node (Master) ]
         |
   -----------------
   |       |       |
[W1]     [W2]     [W3]     <- Worker nodes (Pi, Mac, or other clients)
Leader: Accepts new block header data, splits the nonce space, and dispatches jobs.

Workers: Perform hash computations and report any valid results.

Communication: Bi-directional TCP sockets. Leader polls and monitors node health.


🔒 Security
No wallet credentials are stored or transmitted.

All nodes are assumed to be within a trusted network.

Future improvements will include encrypted communication and signature verification.

🛠️ Customization
Add your own mining algorithm in hashing.c

Change port or communication strategy in networking.c

Monitor worker stats in status.c (optional)

🧪 Example Use Case
Set up a 4-node Raspberry Pi cluster where one Pi is the leader and 3 are workers. The leader splits the nonce range for each block and assigns subranges to the workers. The first worker to find a valid hash sends it back.

🧭 Roadmap
 Add real-time dashboard (websocket + JS frontend)
 
 Fault-tolerant node rejoining

 Wallet integration (remote)

📜 License
MIT License — use it, change it, ship it 🚀

Let me know if you want it styled more for GitHub (with emojis, badges, etc.) or trimmed for embedded docs.