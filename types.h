#ifndef TYPES_H
#define TYPES_H

// Status of function execution
typedef enum
{
    e_success,
    e_failure
}Status;

// Operation type
typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
}OperationType;

#endif