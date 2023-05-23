# filtid
Filter identifier-like tokens from source code.

## Example

### Command
```
cat hello.c | filtid
```

### hello.c
```c
# include "stdio.h"

int main() {
  printf("Hello, ");
  printf("World!\n");
  return 0;
}
```

### Output
```
include
int
main
printf
printf
return
```
