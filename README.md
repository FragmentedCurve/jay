# Jay

Fork of [silentbob](https://sourceforge.net/projects/silentbob/) by
Oleg Puchinin with the intention of

  - Making it compile...
  - Cleaning up junk in the codebase with the intention of making the
    code fulfill it's original purpose.

For more information, see the original [README](README).

# Requirements

  - GNU Make
  - GCC
  
# Build Instructions

    $ make
	$ ./jay
	Usage: ./jay [<function>|<files>] [<options>]
    Options:
       -C <dir>        -  chdir to <dir>
      --cgrep          -  search in code
      --grep           -  search text
      --replace        -  replace text
      --call-tags      -  create "call_tags" file
       -f              -  create "files" file
      --ctags-append   -  don't rewrite "tags" file
      --depth <arg>    -  set reentrant level
      --file <files>   -  show file functions
       -fn             -  echo filenames
      --structs        -  show all structs in stream
       -i              -  interactive mode "tags"
      --indent         -  indention
       -j              -  number of jobs
      --kinds          -  (c - classes, s - structs,    f - functions,
                           m - modules, n - namespaces, p - packages)
       -L <file>       -  read files list from file
       -c              -  create "tags" file
      --tags           -  access to tags
      --the-tt         -  C preprocessor
       -ts             -  force ctags style
       -u              -  reverse call-tree
