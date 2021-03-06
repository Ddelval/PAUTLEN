#ifndef _ALFA_H
#define _ALFA_H

#define MAX_VECTOR_SIZE 64
#define MAX_IDENTIFIER_LENGTH 100

#define MAX_INT_DIGITS 12

typedef enum {
    false = 0, true = 1
} bool;

typedef enum {
    VARIABLE, PARAMETRO, FUNCION
} elementType;
typedef enum {
    BOOLEAN = 1, INT = 0
} dataType;
typedef enum {
    SCALAR, VECTOR
} variableType;

/*
typedef struct {
    char lexeme[MAX_IDENTIFIER_LENGTH + 1];
    int value_int;
    bool is_constexpr;
} index_attributes_t;
*/

typedef struct {
    char lexeme[MAX_IDENTIFIER_LENGTH + 1];
    dataType data_type;
    int value_int;
    bool is_address;
    bool is_constexpr;
    int label;
    int length;
    //index_attributes_t index_attributes;

} attributes_t;


#endif