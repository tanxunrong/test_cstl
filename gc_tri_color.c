
//	Tri Color Marking Tricolor marking
//	
//	A conceptual tool for ﬁnding live data
//	
//	white status unknown
//	gray deﬁnitely live, children not known yet
//	black live, and children also live
//	
//	Collection:
//	1. Initially, all objects are white
//	2. Colorroots gray
//	3. While there is a gray node
//	
//	(a) if there is a white successor, pick one and
//	colorit gray
//	(b) otherwise, colorthe gray node black
//	
//	Invariant:
//	black nodes point only to black and gray nodes
//	When no gray nodes left, white nodes are garbage

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum {
	VAL_INT,
	VAL_PAIR
}ValType;

typedef enum {
	WHITE,
	BLACK,
	GRAY
}ValColor;

typedef struct _tVal TVal;
struct _tVal {
	TVal *next;
	TVal *prev;
	ValType type;
	ValColor color;
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
	//tri-color three sets
	TVal *white;
	int n_white;
	TVal *gray;
	int n_gray;
	TVal *black;
	int n_black;

	int trigger;
	size_t used;
	int top;
};

TVm *newVM();
void pushVal(TVm *vm,TVal *val);
TVal *popVal(TVm *vm);

TVal *newVal(TVm *vm,ValType type);
void pushInt(TVm *vm,int data);
TVal *pushPair(TVm *vm);

void gc(TVm *vm);
void release(TVm *vm);
void moveToGray(TVm *vm,TVal *val);
void moveToBlack(TVm *vm,TVal *val);

TVm *newVM()
{
	TVm *vm = (TVm *)malloc(sizeof(TVm));
	assert(vm);
	memset(vm,0,sizeof(TVm));
	vm->top = 0;
	vm->used = 0;
	vm->trigger = 0;

	vm->white = NULL;
	vm->gray = NULL;
	vm->black = NULL;

	vm->n_white = 0;
	vm->n_black = 0;
	vm->n_gray = 0;

	return vm;
};

void pushVal(TVm *vm,TVal *val)
{
	TVal *gray = vm->gray;
	vm->gray = val;
	val->prev = NULL;
	val->color = GRAY;
	vm->n_gray++;
	val->next = gray;

	if (gray == NULL)
		return;
	gray->prev = val;
}

TVal *popVal(TVm *vm)
{
	assert(vm->gray);

	TVal *gray = vm->gray;
	vm->gray = gray->next;
	if (gray->next)
	{
		gray->next->prev = NULL;
	}
	vm->n_gray--;

	TVal *white = vm->white;
	gray->next = white;
	if (white)
	{
		white->prev = gray;
	}
	gray->prev=NULL;
	vm->white = gray;
	vm->n_white++;

	return gray;
}

TVal *newVal(TVm *vm,ValType type)
{
	if (vm->trigger >= 100)
	{
		gc(vm);
		vm->trigger = 0;
	}
	TVal *val = (TVal*)malloc(sizeof(TVal));
	assert(val);
	memset(val,0,sizeof(TVal));
	val->type = type;
	vm->used ++;

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
	if (vm->gray == NULL)
	{
		release(vm);
		return;
	}
	TVal *gray = vm->gray;
#define GRAY_PICK 5
	for(int i=0;i<GRAY_PICK;i++)
	{		
		if (gray->type == VAL_PAIR)
		{
			moveToGray(vm,gray->value.left);
			moveToGray(vm,gray->value.right);
		}
		moveToBlack(vm,gray);			
		if(vm->gray == NULL)
		{
			release(vm);
			return;
		}
		gray=vm->gray;
	}
	printf("gc round\n");
}

void moveToBlack(TVm *vm,TVal *val)
{
	assert(val);
	assert(val->color == GRAY);
	if (val->prev)
	{
		val->prev->next = val->next;		
	}
	if (val->next)
	{
		val->next->prev = val->prev;
	}
	if (val == vm->gray)
	{
		vm->gray = val->next;
	}
	vm->n_gray--;

	TVal *black = vm->black;
	val->next = black;
	val->prev = NULL;
	if (black)
	{
		black->prev = val;
	}
	vm->black = val;
	vm->n_black++;

}

void moveToGray(TVm *vm,TVal *val)
{

	assert(val->color == WHITE);
	if (val->prev)
	{
		val->prev->next = val->next;		
	}
	if (val->next)
	{
		val->next->prev = val->prev;
	}
	if (val == vm->white)
	{
		vm->white = val->next;
	}
	vm->n_white--;

	TVal *gray = vm->gray;
	val->next = gray;
	val->prev = NULL;
	if (gray)
	{
		gray->prev = val;
	}
	vm->gray = val;
	vm->n_gray++;
}

void release(TVm *vm)
{
	TVal *val = vm->white;
	printf("free white list %d\n",vm->n_white);

	while(val)
	{
		vm->white = val->next;
		free(val);
		vm->n_white--;
		vm->used--;

		if (vm->white)
		{
			val = vm->white;
		}
		else
		{
			break;
		}
	}
	printf("left value count %ld\n", vm->used);
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
  // test4();

  // perfTest();

  return 0;
}
