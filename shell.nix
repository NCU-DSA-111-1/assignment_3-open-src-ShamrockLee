{ lib ? import <nixpkgs/lib>
, pkgs ? import <nixpkgs> { }
, mkShell ? pkgs.mkShell
, bashInteractive ? pkgs.bashInteractive
, clang-tools ? pkgs.clang-tools
, gitAndTools ? pkgs.gitAndTools
, python3 ? pkgs.python3
, arcd ? pkgs.callPackage ./arcd.nix { }
, drichardson_huffman ? pkgs.callPackage ./drichardson_huffman.nix { }
, bench-compression ? pkgs.callPackage ./. { inherit arcd drichardson_huffman; }
}:

mkShell {
  packages = [
    bashInteractive # Workaround the nix-shell bash-non-interactiv issue
    clang-tools
    gitAndTools.gitFull
    python3 # Workaround the meson Nixpkgs package completion issue
  ];

  inputsFrom = [
    bench-compression
  ];

  shellHook = ''
    SHELL="$(type -p bash)"
    export SHELL
  '';
}
