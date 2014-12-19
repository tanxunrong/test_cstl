#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <mruby.h>
#include <mruby/irep.h>
#include <mruby/error.h>
#include <mruby/debug.h>
#include <mruby/variable.h>

int main(int argc,char *argv[])
{
    mrb_state *m = mrb_open(); 
    mrb_close(m);
	return 0;
}
