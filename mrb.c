
#include <stdlib.h>
#include <stdio.h>

/* Include the mruby headers */
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>

int main(int argc, const char * argv[])
{
	mrb_state *mrb = mrb_open();
	char code[] = "[1,2,3].each do |s| puts s+1 end";
	printf("Executing Ruby code with mruby!\n");

	mrb_load_string(mrb, code);
	return 0;
}
