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
    jmp_buf buf;
    m->jmp = &buf;
    if (setjmp(m->jmp) == 0)
    {
        struct RClass *hcd = NULL;
        mrb_bool defined = mrb_const_defined(m,mrb_obj_value(m->object_class),mrb_intern_cstr(m,"Hello"));
        if (defined == 0) {
            hcd = mrb_define_class(m,"Hello",m->object_class);
        }
        else
        {
            hcd = mrb_class_get_under(m,m->object_class,"Hello");
        }

        if (hcd == NULL)
        {
            return -1;
        }
    }
    else
    {
        mrb_print_error(m);
        fprintf(stderr,"unexpected error\n");
    }

    mrb_close(m);
	return 0;
}
