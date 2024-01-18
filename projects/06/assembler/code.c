

#include "code.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

uint16_t c_dest(char* mnemonic) {
  if (mnemonic == NULL) return 0;

  size_t len = strlen(mnemonic);
  
  u_int16_t mask = 0;
  while(*mnemonic) {
    switch(*mnemonic) {
      case 'A':
        mask |= 1 << 5;
        break;
      case 'D':
        mask |= 1 << 4;
        break;
      case 'M':
        mask |= 1 << 3;
        break;
    }
    ++mnemonic;
  }

  return mask;
}

uint16_t c_comp(char* mnemonic) {
  if (mnemonic == NULL) return 0;

  if (strcmp(mnemonic, "0") == 0) {
    return 0b0101010;
  } else if (strcmp(mnemonic, "1") == 0) {
    return 0b0111111;
  } else if (strcmp(mnemonic, "-1") == 0) {
    return 0b0111010;
  } else if (strcmp(mnemonic, "D") == 0) {
    return 0b0001100;
  } else if (strcmp(mnemonic, "A") == 0) {
    return 0b0110000;
  } else if (strcmp(mnemonic, "!D") == 0) {
    return 0b0001101;
  } else if (strcmp(mnemonic, "!A") == 0) {
    return 0b0110001;
  } else if (strcmp(mnemonic, "-D") == 0) {
    return 0b0001111;
  } else if (strcmp(mnemonic, "-A") == 0) {
    return 0b0110011;
  } else if (strcmp(mnemonic, "D+1") == 0) {
    return 0b0011111;
  } else if (strcmp(mnemonic, "A+1") == 0) {
    return 0b0110111;
  } else if (strcmp(mnemonic, "D-1") == 0) {
    return 0b0001110;
  } else if (strcmp(mnemonic, "A-1") == 0) {
    return 0b0110010;
  } else if (strcmp(mnemonic, "D+A") == 0) {
    return 0b0000010;
  } else if (strcmp(mnemonic, "D-A") == 0) {
    return 0b0010011;
  } else if (strcmp(mnemonic, "A-D") == 0) {
    return 0b0000111;
  } else if (strcmp(mnemonic, "D&A") == 0) {
    return 0b0000000;
  } else if (strcmp(mnemonic, "D|A") == 0) {
    return 0b0010101;
  } else if (strcmp(mnemonic, "M") == 0) {
    return 0b1110000;
  } else if (strcmp(mnemonic, "!M") == 0) {
    return 0b1110001;
  } else if (strcmp(mnemonic, "M+1") == 0) {
    return 0b1110111;
  } else if (strcmp(mnemonic, "M-1") == 0) {
    return 0b1110010;
  } else if (strcmp(mnemonic, "D+M") == 0) {
    return 0b1000010;
  } else if (strcmp(mnemonic, "D-M") == 0) {
    return 0b1010011;
  } else if (strcmp(mnemonic, "M-D") == 0) {
    return 0b1000111;
  } else if (strcmp(mnemonic, "D&M") == 0) {
    return 0b1000000;
  } else if (strcmp(mnemonic, "D|M") == 0) {
    return 0b1010101;
  }
  
  return 0;
}

uint16_t c_jump(char* mnemonic) {
  if (mnemonic == NULL) return 0;

  if (strcmp(mnemonic, "JMP") == 0) {
    return 0x7;
  } else if (strcmp(mnemonic, "JGT") == 0) {
    return 0x1;
  } else if (strcmp(mnemonic, "JEQ") == 0) {
    return 0x2;
  } else if (strcmp(mnemonic, "JGE") == 0) {
    return 0x3;
  } else if (strcmp(mnemonic, "JLT") == 0) {
    return 0x4;
  } else if (strcmp(mnemonic, "JNE") == 0) {
    return 0x5;
  } else if (strcmp(mnemonic, "JLE") == 0) {
    return 0x6;
  }

  return 0;
}

char* c_tostr(uint16_t m) {
  int bits = 16; // could be sizeof(m) * CHAR_BIT

  char* str = (char*) malloc(bits + 1);
  str[bits--] = '\0';

  while(bits >= 0) {
    *(str + bits) = (m & 1) + '0';
    m >>= 1;
    --bits;
  }

  return str;
}
