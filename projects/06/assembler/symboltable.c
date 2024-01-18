#include "symboltable.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//hash table
static uint16_t _st_get(const char* symbol);
static void _st_add(const char* symbol, uint16_t address);
static const char* _st_set_entry(Symbol* symbols, size_t cap, const char* symbol, uint16_t address, size_t* length);
static bool _st_expand();
static uint64_t _st_hash_key(const char* key);
//hash table


static SymbolTable st; // = {.cur_mem_address = 0};

void st_initialize(void) {
  static int is_initialized = 0;
  if (!is_initialized) {
    st.cur_mem_address = 0x0010;
    st.cur_rom_address = 0x0;
    st.cap = INITIAL_CAPACITY;
    st.length = 0;
    st._cil = 0;
    st._ci = malloc(sizeof(int) * 50000);
    st.symbols = calloc(st.cap, sizeof(Symbol));
    if (st.symbols == NULL) {
      perror("Could not initialize symbol table");
      exit(-1);
    }
    _st_add("SP",     0x0000);
    _st_add("LCL",    0x0001);
    _st_add("ARG",    0x0002);
    _st_add("THIS",   0x0003);
    _st_add("THAT",   0x0004);
    _st_add("R0", 		0x0000);
    _st_add("R1", 		0x0001);
    _st_add("R2", 		0x0002);
    _st_add("R3", 		0x0003);
    _st_add("R4", 		0x0004);
    _st_add("R5", 		0x0005);
    _st_add("R6", 		0x0006);
    _st_add("R7", 		0x0007);
    _st_add("R8", 		0x0008);
    _st_add("R9", 		0x0009);
    _st_add("R10", 		0x000A);
    _st_add("R11", 		0x000B);
    _st_add("R12", 		0x000C);
    _st_add("R13", 		0x000D);
    _st_add("R14", 		0x000E);
    _st_add("R15", 		0x000F);
    _st_add("SCREEN", 0x4000);
    _st_add("KBD", 		0x6000);

    is_initialized = 1;
  } else {
    perror("Symbol table already initialized");
  }
}

uint16_t st_add_symbol(const char *symbol) {
  assert(st.cur_mem_address < MAX_MEM_ADDRESS);
  const uint16_t address = st.cur_mem_address;
  _st_add(symbol, st.cur_mem_address++);

  return address;
}

void st_inc_rom_address(void) {
  ++st.cur_rom_address;
}

void st_add_label(const char *symbol){
  /* assert(st.cur_rom_address < 1024); */
  _st_add(symbol, st.cur_rom_address);
}

uint16_t st_get_address(const char* symbol) {
  return _st_get(symbol);
}

bool st_has_symbol(const char* symbol) {
  return st_get_address(symbol) > 0;
}

// hash table
static uint16_t _st_get(const char* symbol){
  uint64_t hash = _st_hash_key(symbol);
  size_t index = (size_t)(hash & (uint64_t)(st.cap - 1));

  // Loop till we find an empty entry.
  while (st.symbols[index].symbol != NULL) {
    if (strcmp(symbol, st.symbols[index].symbol) == 0) {
      // Found key, return value.
      return st.symbols[index].address;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= st.cap) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }

  return -1;
}

static void _st_add(const char* symbol, uint16_t address) {
  if (st.length >= st.cap / 2) {
    _st_expand();
  } 

  _st_set_entry(st.symbols, st.cap, symbol, address, &st.length);
}

static const char* _st_set_entry(Symbol* symbols, size_t cap, const char* symbol, uint16_t address, size_t* length) {
  uint64_t hash = _st_hash_key(symbol);
  size_t index = (size_t)(hash & (uint64_t)(cap - 1));

  while (symbols[index].symbol != NULL) {
    if (strcmp(symbol, symbols[index].symbol) == 0) {
      // Found key (it already exists), update value.
      symbols[index].address = address;
      return symbols[index].symbol;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= cap) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }


  if (length != NULL) {
    symbol = strdup(symbol);
    if (symbol == NULL) {
      return NULL;
    }
    ++(*length);
    st._ci[st._cil++] = index;
  }

  symbols[index].symbol = (char*) symbol;
  symbols[index].address = address;

  return symbol;
}

static bool _st_expand() {
  // Allocate new entries array.
  size_t new_capacity = st.cap * 2;
  if (new_capacity < st.cap) {
    return false;  // overflow (capacity would be too big)
  }
  Symbol* new_symbols = calloc(new_capacity, sizeof(Symbol));
  if (new_symbols == NULL) {
    return false;
  }

  // Iterate entries, move all non-empty ones to new table's entries.
  for (size_t i = 0; i < st.cap; i++) {
    Symbol symbol = st.symbols[i];
    if (symbol.symbol != NULL) {
      _st_set_entry(new_symbols, new_capacity, symbol.symbol, symbol.address, NULL);
    }
  }

  // Free old entries array and update this table's details.
  free(st.symbols);
  st.symbols = new_symbols;
  st.cap = new_capacity;
  return true;
}

void st_free() {
  for(size_t i = 0; i < st._cil; i++) {
    if(st.symbols[st._ci[i]].symbol != NULL) {
      free(st.symbols[st._ci[i]].symbol);
    }
  }

  free(st._ci);
  free(st.symbols);
}

static uint64_t _st_hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

