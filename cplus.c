#include "cplus.h"

class(Parent,
	int val;
)

class(Child,
	int num;
	FUNC foo;
	FUNC bar;
)

void p_ctor(this) {}
void p_dtor(this) {}
void foo(this, int a) {}
void bar(this, int a) {}

void build_classes()
{
	CLASS(Child,p_ctor,p_dtor);
	//INHERITANCE(Child, Parent)
	METHODS(Child,2,foo,bar);
}

int main()
{
	build_classes();

	Child *p = new(Child);
	//call(p,foo)(2);
	p->foo(p,2);
	delete(p);

	return 0;
}
