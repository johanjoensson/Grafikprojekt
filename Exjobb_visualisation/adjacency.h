#ifndef ADJACENCY_H
#define ADJACENCY_H

void create_adjacency_table(uint32_t size);

void insert_neighbours(uint32_t a, uint32_t b, uint32_t c);

int32_t find_neighbour(uint32_t a, uint32_t b);

void delete_adjacency_table();

#endif /* ADJACENCY_H */
