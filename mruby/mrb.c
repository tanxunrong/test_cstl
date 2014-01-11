
#include <stdlib.h>
#include <stdio.h>

/* Include the mruby headers */
#include <mruby.h>
#include <mruby/value.h>
#include <mruby/compile.h>
#include <mruby/array.h>
#include <assert.h>

mrb_value fix_func(mrb_state *mrb,mrb_value self)
{
	//mrb_int a,b;
	//mrb_get_args(mrb,"ii",&a,&b);
	
	//char str[1000];
	//mrb_int len;
	//mrb_get_args(mrb,"s",str,&len);
	
	//mrb_value arr;
	//mrb_get_args(mrb,"A",&arr);
	
	return mrb_fixnum_value(1);
}

int main(int argc, const char * argv[])
{
	mrb_state *mrb = mrb_open();
	assert(mrb != NULL);
	mrb_define_method(mrb,mrb->object_class,"fix_func",fix_func,MRB_ARGS_NONE());

	// mrbc_context *cxt = mrbc_context_new(mrb);
	// mrbc_filename(mrb,cxt,"a.rb");
	// mrb_parser_state *par = mrbc_parser_new(mrb);
	// printf("Executing Ruby code with mruby!\n");
	// mrb_parser_parse(mrb,par,cxt);
	// mrbc_context_free(mrb,cxt);
	
	FILE *f = fopen("test.rb","r");
	mrb_load_file(mrb,f);
	fclose(f);	
	mrb_free(mrb,mrb);
	return 0;
}
