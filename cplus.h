#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include <stdio.h>

////////////////////////////////////////////////////////////////////////
// C+ User Macros
////////////////////////////////////////////////////////////////////////

#define class(x,y) __cplus_class(x,y)
#define new(x) __cplus_new(x)
#define delete(x) __cplus_delete(x)
#define this __cplus_this

#define FUNC __FUNC_PTR
#define CLASS(name,ctor,dtor) __cplus_CLASS(name,ctor,dtor)
#define METHODS(name,n,...) __cplus_METHODS(name,n,##__VA_ARGS__)
//#define INHERITANCE() __cplus_INHERITANCE()

////////////////////////////////////////////////////////////////////////
// Small Linked-List Implementation
////////////////////////////////////////////////////////////////////////
struct node;
typedef struct node { void *data; struct node *next; } node;
struct llist;
typedef struct llist { void *head; int count; } llist;
void ll_add(llist *l, void *data) {
	node *n = malloc(sizeof(node));
	n->next = l->head;
	l->head = n;
}
void *ll_rem(llist *l, void *data) {
	node *n = l->head, *p = NULL;
	for(; n != NULL; p = n, n = n->next)
		if(n->data == data) {
			if(!p) { if(n->next) l->head = NULL; else l->head = n->next; }
			else p->next = n->next;
			free(n);
			return data;
		}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Type Definitions
////////////////////////////////////////////////////////////////////////
typedef void (*__FUNC_PTR)();

#define __cplus_this void * __this

#define __cplus_class(x,y) \
	typedef struct x {\
		int __id; \
		__FUNC_PTR __ctor; \
		__FUNC_PTR __dtor; \
		y \
	} x;

typedef struct __class_info {
	char *name;
	unsigned int id;
	size_t size;
	__FUNC_PTR ctor;
	__FUNC_PTR dtor;
	llist vinfo;
	llist iinfo;
} __class_info;

typedef struct __virtual_info {
	char *name;
	__FUNC_PTR f;
} __virtual_info;

////////////////////////////////////////////////////////////////////////
// Static Variables
////////////////////////////////////////////////////////////////////////
#define __MAX_CLASSES 100

unsigned int __uid_incr = 0;
#define __UID_INCR (__uid_incr++)

static __class_info *__ctab[__MAX_CLASSES] = {0};
static int __ctab_count = 0;

////////////////////////////////////////////////////////////////////////
// Helper Functions
////////////////////////////////////////////////////////////////////////
__class_info *__class_lookup(char *name)
{
	int i = 0;
	for(; i < __uid_incr; i++)
		if(!strcmp(__ctab[i]->name,name))
			return __ctab[i];
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Build Class Info
////////////////////////////////////////////////////////////////////////
#define __cplus_CLASS(name, ctor, dtor) \
	__load_class(#name, sizeof(name), ctor, dtor) \

void __load_class(char *name, size_t size, __FUNC_PTR ctor, __FUNC_PTR dtor)
{
	__class_info *cinfo = malloc(sizeof(__class_info));
	cinfo->name = strdup(name);
	cinfo->size = size;
	cinfo->id = __UID_INCR;
	cinfo->ctor = ctor;
	cinfo->dtor = dtor;

	__ctab[cinfo->id] = cinfo;
}

char *__arg_string(int n, ...)
{
	char *s;
	va_list args;
	va_start(args, n);
	s = va_arg(args, char* );
	va_end(args);
	return s;
}

#define __cplus_METHODS(name, n, ...) \
	__load_methods(#name, #__VA_ARGS__, n, ##__VA_ARGS__)

int __load_methods(char *name, char *func_names, int n, ...)
{
	int i = 0;
	char *fstr, *s;
	__virtual_info *vinfo;
	__class_info *cinfo = __class_lookup(name);

	va_list args;
	va_start(args, n);

	fstr = strdup(func_names);
	s = strtok(fstr, " ,");
	for(; i < n; i++) {
		vinfo = malloc(sizeof(__virtual_info));
		vinfo->name = strdup(s);
		s = strtok(NULL, ", ");
		vinfo->f = va_arg(args, __FUNC_PTR );
		ll_add(&cinfo->vinfo, vinfo);
	}
	free(fstr);

	va_end(args);

	return 0;
}

////////////////////////////////////////////////////////////////////////
// Construct Object
////////////////////////////////////////////////////////////////////////
#define __cplus_new(x) \
	__ctor_object(#x)

void *__ctor_object(char *name)
{
	__class_info *cinfo = __class_lookup(name);
	void *data = malloc(cinfo->size);
	cinfo->ctor(data);
	return data;
}

////////////////////////////////////////////////////////////////////////
// Destruct Object
////////////////////////////////////////////////////////////////////////
#define __cplus_delete(x) \
	__dtor_object(x)

void *__dtor_object(void *data)
{
	__class_info *cinfo = __ctab[*((int*)data)];
	cinfo->dtor(data);
	free(data);
}
