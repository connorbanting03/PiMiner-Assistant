#ifndef RUN_NODE_H
#define RUN_NODE_H

int run_node(int argc, char *argv[]);

void run_as_leader();
void run_as_worker(int leader_ip);

#endif