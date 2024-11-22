### Building steps
- First, clone the repo
- Then, while in ./FourierSeries/, run
  - cmake -B build
  - cmake --build build
- The FourierSeries executable should now be in the project root.
- I use linux, so I'm not 100% sure this works for Windows. If there are any issues, make sure cmake knows what kind of environment you're using, such as Mingw.

### Language server
- Clangd
  - Clangd should function as expected, the compile_commands.json is in ./FourierSeries/build/
- CCLS
  - The bash below will copy the contents of the generated copmile_commands.json into the project root
  - touch ./compile_commands.json
  - cat ./build/compile_commands.json > ./compile_commands.json

### My tools
- Compilers: GCC and G++
- Language server: CCLS
- Editor: Neovim
- OS: Linux
  - Distro: Fedora
