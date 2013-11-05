#include "cplus.h"

class(Parent,
	int val;
)

class(Child,
// BEGIN() <- marker for function memory
	FUNC foo;
	FUNC bar;
// END()
	int num;
)

void p_ctor(this) {}
void p_dtor(this) {}
void foo(this, int a) {}
void bar(this, int a) {}

// defaulting values?!
//#define FOOD_INIT(...) { .id = -1, .foo = -1, .bar = -1, ## __VA_ARGS__ }
//struct food barf = FOO_INIT( .id = 42, .foo = 2 );

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
