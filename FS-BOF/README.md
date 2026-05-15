# FS-BOF

Filesystem operations: type, mkdir, copy, move, del, rmdir, pwd, cd

## type

Display contents of a file to beacon output. UTF-8 and UTF-16 LE BOM bytes are stripped from output, matching Windows `cmd.exe type` behavior.

```
type <file>
```

## mkdir

Create a directory and all intermediate directories.

```
mkdir <directory>
```

## move

Move or rename files. Supports wildcards in source and UNC paths. File-only; directories are skipped when using wildcards. Replaces an existing destination file without prompting.

```
move <source> <destination>
```

## del

Delete files at a specified path. Supports wildcards (e.g. `C:\temp\*.tmp`). File-only; directories are skipped.

```
del <target>
```

## copy

Copy files to a destination path. Supports wildcards in source (e.g. `C:\src\*.txt`) and UNC paths. File-only; directories are skipped when using wildcards. Overwrites an existing destination file without prompting.

```
copy <source> <destination>
```

## pwd

Print the beacon's current working directory.

```
pwd
```

## cd

Change the beacon's current working directory. Supports relative paths (`.` and `..`) and UNC paths.

```
cd <directory>
```

## rmdir

Remove an empty directory at a specified path. Supports UNC paths.

```
rmdir <directory>
```
