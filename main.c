#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute(char *command) {
  // Kinda cheating here, since we're using sh
  system(command);
}

void interpret(char *input) {
  int cursor = 0;
  for (int i = 0; input[i] != '\0'; ++i) {
    if (input[i] == '\0')
      break;

    if (input[i] == ';') {
      input[i] = '\0';
      if (*(input + cursor) != '\0') { // Skip empty parts
        execute(input + cursor);
      }

      // Set cursor to the start of the next part
      cursor = i + 1;
      continue;
    }
  }

  execute(input + cursor);
}

int launch_interactive() {
  char *line = NULL;
  size_t len = 0;

  while (1) {
    printf("> ");

    if (getline(&line, &len, stdin) == -1) {
      break;
    }

    line[strcspn(line, "\n")] = '\0'; // EOF at newline

    interpret(line);
  }

  free(line);
  return 0;
}

void print_usage() {
  printf("Usage: myshell [OPTIONS]\n\n");
  printf("Options:\n");
  printf("  -c, --command <COMMAND>  Command string to be executed\n");
  printf("  -f, --file <FILE>        File to be executed\n");
  printf("  -h, --help               Print help\n");
  printf("\n");
  printf("Use myshell without options to enter interactive mode\n");
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    launch_interactive();
    return EXIT_SUCCESS;
  }

  if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
    print_usage();
    return EXIT_SUCCESS;
  }

  if (!strcmp(argv[1], "-c") || !strcmp(argv[1], "--command")) {
    interpret(argv[2]);
    return EXIT_SUCCESS;
  }

  if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "--file")) {
    FILE *file = fopen(argv[2], "r");
    if (!file) {
      fprintf(stderr, "Error opening file: %s\n", argv[2]);
      return EXIT_FAILURE;
    }

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
      line[strcspn(line, "\n")] = '\0'; // EOF at newline
      interpret(line);
    }

    free(line);
    fclose(file);
    return EXIT_SUCCESS;
  }

  printf("Unknown option: %s\n\n", argv[1]);
  printf("For more information, try '--help'\n");
  return EXIT_FAILURE;
}
