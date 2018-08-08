#ifndef MAIN_H
#define MAIN_H

#define STACK_MAX 256
#define GC_THRESHOLD 20 

typedef enum
{
	OBJ_INT,
	OBJ_PAIR
} ObjectType;


typedef struct sObject
{
	struct sObject* next;

	unsigned char marked;
	ObjectType type;

	union
	{
		// INT
		int value;

		// PAIR
		struct
		{
			struct sObject* car;
			struct sObject* cdr;
		};
	};
} Object;


typedef struct
{
	Object* stack[STACK_MAX];
	Object* firstObject;
	int stackSize;

	int numObjects;
	int maxObjects;
} VM;


VM* newVM();
void mark(Object*);
void markAll(VM*);
void sweep(VM*);
void gc(VM*);

Object* newObject(VM*, ObjectType);
void push(VM*, Object*);
Object* pop(VM*);
void pushInt(VM*, int);
Object* pushPair(VM*);

void debugPrint(VM*);
void debugPrintObject(Object*);

#endif
