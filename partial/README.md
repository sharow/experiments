# partial

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
