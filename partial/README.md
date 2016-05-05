# partial


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
}
```

## Class like struct member
TBD
