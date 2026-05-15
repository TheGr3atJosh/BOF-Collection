# BOF-Collection

An [AdaptixC2](https://github.com/Adaptix-Framework/AdaptixC2)-compatible BOF collection for filesystem and process-control operations executed inside the beacon process.

# Installation

```
# Ubuntu/Kali
apt install gcc-mingw-w64-x86-64-posix gcc-mingw-w64-i686 mingw-w64-tools

# Arch
pacman -Syu mingw-w64-gcc
```

Building modules via make:

```bash
git clone https://github.com/TheGr3atJosh/BOF-Collection
cd BOF-Collection
make
```

A Docker-based build is also available for systems where installing mingw-w64 directly is inconvenient (e.g. macOS):

```bash
docker compose build && docker compose run --rm bof-build
```

Load all modules in AdaptixC2 client: **Main menu** → **AxScript** → **Script manager**, then **Context menu** → **Load new** and select the _bof-collection.axs_ file.

To auto-load on server start, add to your AdaptixC2 `profile.yaml` under `Teamserver`:

```yaml
axscripts:
  - "BOF-Collection/bof-collection.axs"  # path relative to the server working directory
```

# Modules

## FS-BOF

Filesystem operations: type, mkdir, copy, move, del, rmdir, pwd, cd. [More details](FS-BOF/README.md)

|Commands|Usage|Notes|
|--------|-----|-----|
|type|`type <file>`|Display contents of a file to beacon output|
|mkdir|`mkdir <directory>`|Create a directory and all intermediate directories|
|move|`move <source> <destination>`|Move or rename files; supports wildcards and UNC paths; file-only, directories skipped|
|del|`del <target>`|Delete files; supports wildcards; file-only, directories skipped|
|copy|`copy <source> <destination>`|Copy files; supports wildcards and UNC paths; file-only, directories skipped|
|pwd|`pwd`|Print the beacon's current working directory|
|cd|`cd <directory>`|Change working directory; supports relative paths and UNC paths|
|rmdir|`rmdir <directory>`|Remove an empty directory; supports UNC paths|

## Exit-BOF

Terminate the beacon process or thread. [More details](Exit-BOF/README.md)

|Commands|Usage|Notes|
|--------|-----|-----|
|exit process|`exit process`|Terminate beacon process via `RtlExitUserProcess(0)`. Terminal.|
|exit thread|`exit thread`|Terminate beacon thread via `RtlExitUserThread(0)`. Terminal.|

## Credits

- [Extension-Kit](https://github.com/Adaptix-Framework/Extension-Kit): Project structure and README
