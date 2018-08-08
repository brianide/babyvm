#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "main.h"

VM* newVM()
{
	VM* vm = malloc(sizeof(VM));
	vm->stackSize = 0;
	vm->firstObject = NULL;

	vm->numObjects = 0;
	vm->maxObjects = GC_THRESHOLD;

	return vm;
}

void mark(Object* obj)
{
	if(obj->marked)
		return;

	obj->marked = 1;

	if(obj->type == OBJ_PAIR)
	{
		mark(obj->car);
		mark(obj->cdr);
	}
}

void markAll(VM* vm)
{
	for(int i = 0; i < vm->stackSize; i++)
	{
		mark(vm->stack[i]);
	}
}

void sweep(VM* vm)
{
	Object** object = &vm->firstObject;
	while(*object)
	{
		if(!(*object)->marked)
		{
			Object* unreached = *object;

			*object = unreached->next;
			free(unreached);
			vm->numObjects--;
		}
		else
		{
			(*object)->marked = 0;
			object = &(*object)->next;
		}
	}
}

void gc(VM* vm)
{
	markAll(vm);
	sweep(vm);

	vm->maxObjects = vm->numObjects * 2;
}

Object* newObject(VM* vm, ObjectType type)
{
	if(vm->numObjects == vm->maxObjects)
		gc(vm);

	Object* object = malloc(sizeof(Object));
	object->type = type;
	object->marked = 0;

	object->next = vm->firstObject;
	vm->firstObject = object;

	vm->numObjects++;
	return object;
}

void push(VM* vm, Object* value)
{
	assert(vm->stackSize < STACK_MAX && "Stack overflow");
	vm->stack[vm->stackSize++] = value;
}

Object* pop(VM* vm)
{
	assert(vm->stackSize > 0 && "Stack underflow");
	return vm->stack[--vm->stackSize];
}

void pushInt(VM* vm, int value)
{
	Object* object = newObject(vm, OBJ_INT);
	object->value = value;
	push(vm, object);
}

Object* pushPair(VM* vm)
{
	Object* object = newObject(vm, OBJ_PAIR);
	object->cdr = pop(vm);
	object->car = pop(vm);

	push(vm, object);
	return object;
}

void debugPrint(VM* vm)
{
	printf("== VM State ==\n");
	printf("Objects: %d/%d\n\n", vm->numObjects, vm->maxObjects);
	
	printf("Stack:\n");
	for(int i = 0; i < vm->stackSize; i++)
	{
		printf(" %02X: ", i);
		debugPrintObject(vm->stack[i]);
		printf("\n");
	}
	printf("==============\n");
}

void debugPrintObject(Object* obj)
{
	switch(obj->type)
	{
		case OBJ_INT:
			printf("%d", obj->value);
			break;
		case OBJ_PAIR:
			printf("[ ");
			debugPrintObject(obj->car);
			printf(" ");
			debugPrintObject(obj->cdr);
			printf(" ]");
			break;
	}
}

int main(int argc, char** argv)
{
	VM* vm = newVM();

	pushInt(vm, 1);
	pushInt(vm, 2);
	pushPair(vm);
	pushInt(vm, 3);
	gc(vm);

	debugPrint(vm);
}
