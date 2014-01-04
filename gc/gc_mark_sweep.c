
// thanks to http://journal.stuffwithstuff.com/2013/12/08/babys-first-garbage-collector

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum {
	VAL_INT,
	VAL_PAIR
}ValType;

typedef struct _tVal TVal;
struct _tVal {
	TVal *next;
	ValType type;
	unsigned char mark;
	union {
		int data;
		struct {
			TVal *left;
			TVal *right;
		};
	}value;
};

typedef struct _tVm TVm;

struct _tVm {
#define MAX_STACK 512
	TVal *stack[MAX_STACK];
	int top;
	TVal *first;
	size_t maxgc;
	size_t used;
};

TVm *newVM();
void pushVal(TVm *vm,TVal *val);
TVal *popVal(TVm *vm);

TVal *newVal(TVm *vm,ValType type);
void pushInt(TVm *vm,int data);
TVal *pushPair(TVm *vm);

void mark(TVal *val);
void markAll(TVm *vm);
void sweep(TVm *vm);
void gc(TVm *vm);

TVm *newVM()
{
	TVm *vm = (TVm *)malloc(sizeof(TVm));
	assert(vm);
	memset(vm,0,sizeof(TVm));
	vm->top = 0;
	vm->first = NULL;
	vm->maxgc = 24;
	vm->used = 0;
	return vm;
};

void pushVal(TVm *vm,TVal *val)
{
	assert(vm->top < MAX_STACK);
	vm->stack[vm->top++] = val;
}

TVal *popVal(TVm *vm)
{
	assert(vm->top > 0);
	return vm->stack[--vm->top];
}

TVal *newVal(TVm *vm,ValType type)
{
	if (vm->used >= vm->maxgc)
		gc(vm);
	TVal *val = (TVal*)malloc(sizeof(TVal));
	assert(val);
	memset(val,0,sizeof(TVal));
	val->type = type;
	val->mark = 0;

	vm->used ++;
	val->next = vm->first;
	vm->first = val;
	return val;
}

void pushInt(TVm *vm,int data)
{
	TVal *val = newVal(vm,VAL_INT);
	val->value.data = data;
	pushVal(vm,val);
}

TVal *pushPair(TVm *vm)
{
	TVal *val = newVal(vm,VAL_PAIR);
	val->value.left = popVal(vm);
	val->value.right = popVal(vm);
	pushVal(vm,val);
	return val;
}

void gc(TVm *vm)
{
	int used = vm->used;
	markAll(vm);
	sweep(vm);

	vm->maxgc = vm->maxgc * 2;
	printf("collected %ld objects,%ld remain maxgc trigger %ld \n",used - vm->used,vm->used,vm->maxgc);
}

void mark(TVal *val)
{
	if (val->mark) return;
	val->mark = 1;
	if (val->type == VAL_PAIR)
	{
		mark(val->value.left);
		mark(val->value.right);
	}
}

void markAll(TVm *vm)
{
	for(int i=0;i<vm->top;i++)
	{
		mark(vm->stack[i]);
	}
}

void sweep(TVm *vm)
{
	TVal **valp = &(vm->first);
	while(*valp)
	{
		if (!(*valp)->mark)
		{
			TVal *unreached = *valp;
			*valp = unreached->next;
			free(unreached);
			vm->used--;
		}
		else
		{
			(*valp)->mark = 0;
			valp = &(*valp)->next;
		}
	}
}

void test1() {
  printf("Test 1: Objects on stack are preserved.\n");
  TVm* vm = newVM();
  pushInt(vm, 1);
  pushInt(vm, 2);

  gc(vm);
  assert(vm->used == 2);
}

void test2() {
  printf("Test 2: Unreached objects are collected.\n");
  TVm* vm = newVM();
  pushInt(vm, 1);
  pushInt(vm, 2);
  popVal(vm);
  popVal(vm);

  gc(vm);
  assert(vm->used == 0);
}

void test3() {
  printf("Test 3: Reach nested objects.\n");
  TVm* vm = newVM();
  pushInt(vm, 1);
  pushInt(vm, 2);
  pushPair(vm);
  pushInt(vm, 3);
  pushInt(vm, 4);
  pushPair(vm);
  pushPair(vm);

  gc(vm);
  assert(vm->used == 7);
}

void test4() {
  printf("Test 4: Handle cycles.\n");
  TVm* vm = newVM();
  pushInt(vm, 1);
  pushInt(vm, 2);
  TVal* a = pushPair(vm);
  pushInt(vm, 3);
  pushInt(vm, 4);
  TVal* b = pushPair(vm);

  a->value.right = b;
  b->value.right = a;

  gc(vm);
  assert(vm->used == 4);
}

void perfTest() {
  printf("Performance Test.\n");
  TVm* vm = newVM();

  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 20; j++) {
      pushInt(vm, i);
    }

    for (int k = 0; k < 20; k++) {
      popVal(vm);
    }
  }
}

int main(int argc, const char * argv[]) {
  test1();
  test2();
  test3();
  test4();

  perfTest();

  return 0;
}