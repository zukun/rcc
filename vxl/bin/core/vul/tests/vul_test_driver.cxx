#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




/* Forward declare test functions. */
int test_arg(int, char*[]);
int test_file_iterator(int, char*[]);
int test_vul_file(int, char*[]);
int test_ios_state(int, char*[]);
int test_psfile(int, char*[]);
int test_sprintf(int, char*[]);
int test_temp_filename(int, char*[]);
int test_regexp(int, char*[]);
int test_string(int, char*[]);
int test_sleep_timer(int, char*[]);
int test_get_time_as_string(int, char*[]);
int test_awk(int, char*[]);
int test_url(int, char*[]);


/* Create map.  */

typedef int (*MainFuncPointer)(int , char*[]);
typedef struct
{
  const char* name;
  MainFuncPointer func;
} functionMapEntry;

functionMapEntry cmakeGeneratedFunctionMapEntries[] = {
    {
    "test_arg",
    test_arg
  },
  {
    "test_file_iterator",
    test_file_iterator
  },
  {
    "test_vul_file",
    test_vul_file
  },
  {
    "test_ios_state",
    test_ios_state
  },
  {
    "test_psfile",
    test_psfile
  },
  {
    "test_sprintf",
    test_sprintf
  },
  {
    "test_temp_filename",
    test_temp_filename
  },
  {
    "test_regexp",
    test_regexp
  },
  {
    "test_string",
    test_string
  },
  {
    "test_sleep_timer",
    test_sleep_timer
  },
  {
    "test_get_time_as_string",
    test_get_time_as_string
  },
  {
    "test_awk",
    test_awk
  },
  {
    "test_url",
    test_url
  },

  {0,0}
};

/* Allocate and create a lowercased copy of string
   (note that it has to be free'd manually) */

char* lowercase(const char *string)
{
  char *new_string, *p;
  new_string = (char *)malloc(sizeof(char) * (size_t)(strlen(string) + 1));
  if (!new_string)
    {
    return 0;
    }
  strcpy(new_string, string);
  p = new_string;
  while (*p != 0)
    {
    *p = (char)tolower(*p);
    ++p;
    }
  return new_string;
}

int main(int ac, char *av[])
{
  int i, NumTests, testNum, partial_match;
  char *arg, *test_name;
  int count;
  int testToRun = -1;

  
    
  for(count =0; cmakeGeneratedFunctionMapEntries[count].name != 0; count++)
    {
    }
  NumTests = count;
  /* If no test name was given */
  /* process command line with user function.  */
  if (ac < 2)
    {
    /* Ask for a test.  */
    printf("Available tests:\n");
    for (i =0; i < NumTests; ++i)
      {
      printf("%3d. %s\n", i, cmakeGeneratedFunctionMapEntries[i].name);
      }
    printf("To run a test, enter the test number: ");
    fflush(stdout);
    testNum = 0;
    scanf("%d", &testNum);
    if (testNum >= NumTests)
      {
      printf("%3d is an invalid test number.\n", testNum);
      return -1;
      }
    testToRun = testNum;
    ac--;
    av++;
    }
  partial_match = 0;
  arg = 0;
  /* If partial match is requested.  */
  if(testToRun == -1 && ac > 1)
    {
    partial_match = (strcmp(av[1], "-R") == 0) ? 1 : 0;
    }
  if (partial_match && ac < 3)
    {
    printf("-R needs an additional parameter.\n");
    return -1;
    }
  if(testToRun == -1)
    {
    arg = lowercase(av[1 + partial_match]);
    }
  for (i =0; i < NumTests && testToRun == -1; ++i)
    {
    test_name = lowercase(cmakeGeneratedFunctionMapEntries[i].name);
    if (partial_match && strstr(test_name, arg) != NULL)
      {
      testToRun = i;
      ac -=2;
      av += 2;
      }
    else if (!partial_match && strcmp(test_name, arg) == 0)
      {
      testToRun = i;
      ac--;
      av++;
      }
    free(test_name);
    }
  if(arg)
    {
    free(arg);
    }
  if(testToRun != -1)
    {
    int result;

    result = (*cmakeGeneratedFunctionMapEntries[testToRun].func)(ac, av);

    return result;
    }
  
  
  /* Nothing was run, display the test names.  */
  printf("Available tests:\n");
  for (i =0; i < NumTests; ++i)
    {
    printf("%3d. %s\n", i, cmakeGeneratedFunctionMapEntries[i].name);
    }
  printf("Failed: %s is an invalid test name.\n", av[1]);
  
  return -1;
}
