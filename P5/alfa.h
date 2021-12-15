#ifndef _ALFA_H
#define _ALFA_H

#define MAX_VECTOR_SIZE 64
#define MAX_IDENTIFIER_LENGTH 100

#define MAX_INT_DIGITS 12

typedef enum { false = 0, true = 1 } bool;

typedef enum { VARIABLE, PARAMETRO, FUNCION } elementType;
typedef enum { BOOLEAN, INT } dataType;
typedef enum { SCALAR, VECTOR } variableType;

typedef struct {
    char lexeme[MAX_IDENTIFIER_LENGTH + 1];
    dataType type;
    int value_int;
    bool is_address;
    int label;

} attributes_t;

#endif