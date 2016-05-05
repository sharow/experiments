# partial

- only support x86_64(amd64) linux for now.


## Basic

```c
#include <stdio.h>
#include <partial/partial.h>

void greet(char *who)
{
    printf("hello %s!\n", who);
}

int main(void)
{
    greet("sharow");    // => print `hello sharow!`

	void (*greet_to_sharow)() = partial(greet, "sharow");
	greet_to_sharow();  // print `hello sharow!`

    unpartial(greet_to_sharow);  // cleanup
}
```

## Chaining

```c
#include <stdio.h>
#include <partial/partial.h>

void foo(char *a, char *b, char *c, char *d)
{
    printf("%s %s %s %s\n", a, b, c, d);
}

int main(void)
{
    void (*a)(char *b, char *c, char *d) = partial(foo, "A");
    void (*b)(char *c, char *d) = partial(a, "B");
    void (*c)(char *d) = partial(b, "C");
    void (*d)() = partial(c, "D");

    d();  // print "A B C D"

    // or ...
    ((void (*)())partial(partial(partial(partial(foo, "A"), "B"), "C"), "D"))();
}
```

## Class like struct member

```c
#include <stdio.h>
#include <stdlib.h>
#include <partial/partial.h>

typedef struct FooClass_ FooClass;
struct FooClass_ {
    char *_name;
    void (*set_name)(char *name);
    char *(*get_name)();
    void (*show_name)();
};

void FooClass_set_name(FooClass *foo, char *name)
{
    foo->_name = name;
}

char *FooClass_get_name(FooClass *foo)
{
    return foo->_name;
}

void FooClass_show_name(FooClass *foo)
{
    printf("foo->_name: %s\n", foo->_name);
}

FooClass *FooClass_new(char *name)
{
    FooClass *foo = malloc(sizeof(FooClass));
    foo->_name = name;
    foo->set_name = partial(FooClass_set_name, foo);
    foo->get_name = partial(FooClass_get_name, foo);
    foo->show_name = partial(FooClass_show_name, foo);
    return foo;
}

void FooClass_delete(FooClass *foo)
{
    unpartial(foo->show_name);
    unpartial(foo->get_name);
    unpartial(foo->set_name);
    free(foo);
}

int main(void)
{
    FooClass *foo0 = FooClass_new("foo0!");
    FooClass *foo1 = FooClass_new("foo1!");

    foo0->show_name();
    foo1->show_name();

    foo0->set_name("fooooooooo!");
    puts("");

    foo0->show_name();
    foo1->show_name();

    FooClass_delete(foo1);
    FooClass_delete(foo0);
}
	
/* output:
foo->_name: foo0!
foo->_name: foo1!

foo->_name: fooooooooo!
foo->_name: foo1!
*/

```
