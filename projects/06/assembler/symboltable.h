#ifndef SYMBOLTABLEH
#define SYMBOLTABLEH

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 64
#define MAX_MEM_ADDRESS 0x3FFF
#define MAX_ABS_ADDRESS 0x6001

typedef struct {
    char* symbol;
    uint16_t address;
} Symbol;

typedef struct {
    uint16_t cur_mem_address;
    uint16_t cur_rom_address;
    Symbol* symbols;
    size_t cap;
    size_t length;
} SymbolTable;


void st_initialize(void);
uint16_t st_add_symbol(const char* symbol);
void st_inc_rom_address(void);
void st_add_label(const char* symbol);
uint16_t st_get_address(const char* symbol);
bool st_has_symbol(const char* symbol);

// hash table
void st_free(void);


#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL


#endif
