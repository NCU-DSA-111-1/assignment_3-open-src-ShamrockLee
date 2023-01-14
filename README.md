# bench-compression

This project benchmarks the compression and decompression performance of two projects on github:

*   [wonder-mice/arcd](https://github.com/wonder-mice/arcd) (Arithematic coding)
*   [drichardson/huffman](https://github.com/drichardson/huffman) (Huffman coding)

## Benchmark items

The benchmark is done by calling the C functions and calculate the time

```c
const clock_t begin = clock();
// call function here
const clock_t end = clock();
const clock_t clock_diff = end - begin;
```

and the size of the encoded file:
```c
fseek(pf_encoded, 0, SEEK_END);
const long size_file_encoded = ftell(pf_encoded);
fseek(pf_encoded, 0, SEEK_SET);
```

This might not be careful benchmark, as the model for arcd is taken directly from the project example, while there's a reminder in the README that

> for arithmetic coding to be efficient you need to build a decent probability model of your data

It's understandable for the cost and the efficiency of arcd to be poor in this setup.

## Extra efforts made

Aside from the main executable and the build script for the two other projects, I also fixed a bug that prevents the arcd examples to build with CMake (`-std=c99` specified, while non-c-standard `fdopen` used), and provide options to install the build result of arcd examples and tests, so that the library object of arcd examples is exposed to this main project.

The fixes can be found here:
https://github.com/wonder-mice/arcd/compare/master...ShamrockLee:arcd:examples-drop-fdopen

The arcd project, however, is not actually open source. The license it is distributed under is no where to be found, making it all-right-reserved (See [this issue](https://github.com/wonder-mice/arcd/issues/1)). I should be able to upstreab these changes through a pull request once the license is added.


## Usage

The binary `bench` runs with the path to the sample file and the number of rounds provided. For example:

```sh
bench path/to/file 5
```

The benchmark cases are referred to as zero-based indices. There are two benchmark samples available, as mentioned above.

The above command is equivalent to

```sh
bench path/to/file 5 0 1
```

To reverse the benchmark order, run
```sh
bench path/to/file 5 1 0
```


## Build and run

This project is build with the Nix package manager / project manager. Due to the shortage of development time, I haven't prepared a package-manager-independent build automation such as GNU Make or Meson.

### TL;DR
```console
$ docker run -ti --rm -v /tmp:/tmp -v "$PWD":"$PWD" -w "$PWD" nixos/nix:2.12.0 nix --extra-experimental-features "nix-command flakes" run .#bench-with-hsimple
CLOCKS_PER_SEC: 1000000, size_file_orig: 414854, n_round: 10, 0 (arcd_mice_adaptime_model), 1 (huffman_drichardson), 
encoding time (1/CLOCKS_PER_SEC second), encoded size (byte), decoding time (1/CLOCKS_PER_SEC second),

58521, 414702, 84299, 8624, 415630, 3954, 
56174, 414702, 82345, 8446, 415630, 3785, 
55171, 414702, 87023, 9370, 415630, 4581, 
58997, 414702, 88121, 8933, 415630, 4137, 
52519, 414702, 82358, 8865, 415630, 4024, 
53569, 414702, 83290, 8676, 415630, 3996, 
55304, 414702, 83473, 8853, 415630, 3985, 
55054, 414702, 83933, 9038, 415630, 3994, 
55545, 414702, 82711, 8970, 415630, 3976, 
55763, 414702, 83835, 8901, 415630, 4107, 

```

### Detail

The binary, `bench`, can be built and run on the fly with

```sh
nix run .#bench-compression -- <your sample file> <number of rounds> [<sample order>]
```

The detailed build commands of the main program can be found inside `default.nix`.

To enter a shell session with the other two projects built readily, run

```
nix develop
```

This project comes with a sample known as `hsimple.root` taken from [CERN ROOT](https://github.com/root-project/root), a statistical framework in C++ I use in the High Energy Physics lab.

Run the benchmark with the samples:

```console
$ nix run .#bench-with-hsimple
CLOCKS_PER_SEC: 1000000, size_file_orig: 414854, n_round: 10, 0 (arcd_mice_adaptime_model), 1 (huffman_drichardson), 
encoding time (1/CLOCKS_PER_SEC second), encoded size (byte), decoding time (1/CLOCKS_PER_SEC second),

57077, 414705, 92581, 7808, 415630, 4090, 
55500, 414705, 85025, 7581, 415630, 4064, 
54377, 414705, 89352, 7919, 415630, 4151, 
54502, 414705, 82380, 7569, 415630, 4094, 
54993, 414705, 84141, 7520, 415630, 4000, 
55264, 414705, 84182, 7584, 415630, 4042, 
54883, 414705, 83809, 7554, 415630, 3974, 
54925, 414705, 84200, 7644, 415630, 3979, 
55014, 414705, 91699, 7710, 415630, 4148, 
55373, 414705, 85800, 7620, 415630, 3958, 

```

To see the sample, run
```
nix build .#hsimple-root
```

and a symbolic link to it named `result` will be created.

To use Docker instead of Nix, just replace

```sh
nix blablabla
```

with

```sh
docker run -ti --rm -v /tmp:/tmp -v "$PWD":"$PWD" -w "$PWD" nixos/nix:2.12.0 nix --extra-experimental-features "nix-command flakes" blablabla
```
