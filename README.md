# Math-trainer
A program used to train your mathematical skills

## Documentation

### Getting Options
The first of input parsing is to get the short options from the options file in the config folder.

```c
FILE *options = fopen("config/options.txt", "r");
```

Nothing surprising here really, just opening a file with `fopen()`. The next part is to get the contents from `options.txt` into the program.

```c
fseek(options, 0, SEEK_END);
int len = ftell(options);
fseek(options, 0, SEEK_SET);

char *optstr = malloc(sizeof(char) * (len + 1));

/* Error Handling */

fread(optstr, sizeof(char), len, options);
optstr[len] = '\0';
```

The first three lines of code tell us the size of the file by moving the file position. Using `fseek()` with the `offset` to `0` and the `origin` to `SEEK_END` moves the file position to the end of the file. By using `ftell()` to check for the current offset, we can find the size of the file in bytes. Since each `char` is one `byte` long, we can infer that the offset is the amount of `char` we need. This is saved into the length variable. After that, we use `fseek()` again with the same `offset` but with the `origin` set to `SEEK_SET` instead. This simply moves the file position back to the start. Next with the comfirm length of the file's contents, we can allocate memory to a `char` pointer using `malloc()`. The amount of space we allocate is one more than the length of the file to fit the terminating byte (`\0`) into the end of the string. After that we read the contents of the file with `fread()` and set the last character in the string as `\0`.

Though this approach may be slower than just assigning an abitrary number as the maximum string length, I prefer it as we not only save space but also prevents the file contents to overflow the allocated space for the string. This will decrease the number of segfaults when using the program (which is a win in my books).

Next we have to setup the long options. This part is pretty simple.

```c
static struct option longopts[] = {
        {"archive", no_argument, NULL, 257},
        {"remember", no_argument, NULL, 258},
        {0, 0, 0, 0}
    };
```

This is the first part where we have to initliase a list / pointer that contains `struct option`. This is due to `getopt_long()` requiring a pointer to a series of `struct option`. A `struct option` is declared as:

```c
struct option {
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
};
```

Where, `name` is the name of the long option, `has_arg` being whether the option has `no_argument`, `required_argument` or `optional_argument` which represent the integers `1`, `2` and `3` respectively. The flag is a pointer to an int which will be set to whatever `val` is when found. If `flag` is not `NULL`, `getopt_long()` will return `0`. However, if `flag` is `NULL`, `getopt_long()` will return `val`. `val` is simply the value to return by `getopt_long()` or put into wherever `flag` points to.

***Note: Due to how `getop_long()` is coded, it is mandatory to fill the last value in your pointer or list with `{0, 0, 0, 0}`***

One thing I would like to mention is that I chose `val` for the options in longopts to start counting from `257` is due to how `getopt_long()` returns the character's value based on ASCII for shortopts. Additionally, I set the global vaiable `opterr` to `0` to prevent `getopt_long()` from printing its own error messages. Lastly, I created a variable `long_index` which will be set by `getopt_long()` to the index of the long option that has been found relative to `longopts`. It does not have much use but I may need to use it later. 

**If you want to find out more about the family of `getopts`, you should visit the Linux man pages. Here is the [link](https://linux.die.net/man/3/getopt_long)**

The last part of getting the options is actually getting the options themmselves.