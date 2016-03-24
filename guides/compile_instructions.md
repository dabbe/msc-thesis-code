# Compile Instructions

## Compiling Mono and running C# programs
Here are instructions for installing Mono on Debian 4.6.3-14. Commands needed for solving issues within Axis Communications are omitted.

### Download Mono
 - Clone the git repository from GitHub: `git clone https://github.com/mono/mono.git`
 - Checkout Mono version 4.2.2: `git checkout mono-4.2.2`

### Installing Mono for Building
 - Create a `$PREFIX` variable in your terminal to where you want to install the build part of Mono. For our terminal, we used: `export PREFIX=YOUR_PATH`.
 - Add an `apt-key` with the following command: 
```
$ sudo apt-key adv --keyserver-options --keyserver 
hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
```
 - Download Mono for building
```
$ echo "deb http://download.mono-project.com/repo/debian wheezy main" | 
sudo tee /etc/apt/sources.list.d/mono-xamarin.list
```
- Make sure package lists from are updated: `sudo apt-get update`
- Install Mono Complete: `sudo apt-get install mono-complete`
- Go to where Mono was cloned from GitHub and run `autogen.sh`:
```
./autogen.sh --prefix=$PREFIX
```
- For a smaller footprint, consider adding the following parameters to the compilation for `autogen.sh`:
    - `--enable-minimal=attach,com,debug,full_messages,logging,profiler`
    - `--with-gc=included`
    - `--with-tls=__thread`
    - `--with-xen-opt=no`
    - `--enable-small-config=yes`
    - `--with-ikvm-native=no`
    - `--with-profile4=no`
    - `--with-mcs-docs=no`
    - `--with-profile2=no`
    - `--with-profile4=no`
    - `--with-profile4_5=yes`
    - `--with-moonlight=no`
    - `--with-glib=embedded`
 - Run make on the project: `make`
 - Run make install on the project: `make install`

### Installing Mono for Target
 - Create the base directory for Mono and save it in a terminal variable `$TARGET_PREFIX`. This is where it will start looking for `dll`-files needed. `chmod` the rights to be able to run the following scripts.
 - Add the path to your compiler to your `$PATH` terminal variable. 
 - Enter the directory where you initially cloned the Mono repository and run `autogen.sh` with different parameters: 
```
./autogen.sh --disable-mcs-build --prefix=$TARGET_PREFIX --host=YOUR_COMPILER
```
 - If you used the smaller footprint enabling parameters in your previous autogen run, you must also add them here.
 - Run make on the project: `make`.
 - Run make install on the project: `make install`.

### Merging Build and Target
 - Copy the folder at `$PREFIX`and merge it with the folder at `$TARGET_PREFIX`.
 - Copy this folder you just merged and put it on your target machine.
 - Modify the rights with `chmod`.
 - Run `./mono-sgen HelloWorld.exe` and grab a beer as you read the beautiful words that spell **Hello World** on your screen.
 - Strip your binary folder in the Mono directory from files you do not need.
 
### Compiling C# Code
Create a source code file `HelloWorld.cs` and to compile it, run: `mcs HelloWorld.cs`. 
The output will be a file called `HelloWorld.exe`. In order to execute it, type: `mono HelloWorld.exe`.

### Compiling libgdiplus
In order to compile libgdiplus for our ARM platform, we had to compile the following libraries for our platform.

- libcairo
 - libfontconfig
 - libpixman
 - libpng12
 - libjpeg
 - libz
 - libX11 with the following dependencies:
    - kbproto
    - libXi
    - xcb
    - xcb-proto
    - xorg-macros
    - xproto
    - xtrans
    - libXau
    - libXrender
    - renderproto

