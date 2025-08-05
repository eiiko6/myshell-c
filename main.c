#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 10000

int execute(char *command) {
  system(command);
  return 1;
}

int interpret(char *input) {
  char part[MAX_STRING_SIZE];
  int cursor = 0;
  for (int i = 0; i < MAX_STRING_SIZE; ++i) {
    if (input[i] == '\0')
      break;

    if (input[i] == ';') {
      input[i] = '\0';
      execute(input + cursor);
      cursor = i + 1;
      continue;
    }
  }

  execute(input + cursor);

  return 0;
}

int launch_interactive() {
  while (1) {
    printf("> ");

    char s[10000];
    fgets(s, sizeof(s), stdin);

    for (int i = 0; i < MAX_STRING_SIZE; ++i) {
      if (s[i] == '\n') {
        s[i] = '\0';
        break;
      }
    }

    interpret(s);
  }

  return 0;
}

int main(int argc, char *argv[]) {
  // for (int i = 1; i < argc; ++i) {
  //   printf("Arg %d: %s\n", i, argv[i]);
  // }

  if (argc == 1) {
    launch_interactive();
  }

  return EXIT_SUCCESS;
}
