{
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.11";
  # inputs.arcd-source.url = "github:wonder-mice/arcd";
  inputs.arcd-source.url = "github:ShamrockLee/arcd/examples-drop-fdopen";
  inputs.arcd-source.flake = false;
  inputs.drichardson_huffman-source.url = "github:drichardson/huffman";
  inputs.drichardson_huffman-source.flake = false;

  outputs = inputs:
    inputs.flake-utils.lib.eachDefaultSystem (system:
      let
        lib = inputs.nixpkgs.lib;
        pkgs = inputs.nixpkgs.legacyPackages.${system} or (import inputs.nixpkgs { inherit system; });
        arcd = (pkgs.callPackage ./arcd.nix { }).overrideAttrs (oldAttrs: {
          version = inputs.arcd-source.lastModifiedDate;
          src = inputs.arcd-source;
          cmakeFlags = oldAttrs.cmakeFlags or [ ] ++ [
            "-DARCD_EXAMPLES=TRUE"
            "-DARCD_EXAMPLES_INSTALL=TRUE"
            "-DARCD_TESTS=TRUE"
            "-DARCD_TESTS_INSTALL=TRUE"
          ];
        });
        drichardson_huffman = (pkgs.callPackage ./drichardson_huffman.nix { }).overrideAttrs (oldAttrs: {
          version = inputs.drichardson_huffman-source.lastModifiedDate;
          src = inputs.drichardson_huffman-source;
        });
        bench-compression = (pkgs.callPackage ./. {
          inherit arcd drichardson_huffman;
        }).overrideAttrs (oldAttrs: {
          pname = oldAttrs.pname + "unstable";
          version = inputs.self.lastModifiedDate;
          src = inputs.self;
        });
        hsimple-root = pkgs.callPackage ./hsimple-root.nix { };
        bench-with-hsimple-script = pkgs.writeShellScript "bench-with-hsimple" ''
          N_ROUND="''${2-}"
          if [ -n "$N_ROUND" ]; then
            shift
          else
            N_ROUND=10
          fi
          "${bench-compression}/bin/bench" "${hsimple-root}" "$N_ROUND" "$@"
        '';
      in
      {
        packages = {
          default = bench-compression;
          inherit
            arcd
            bench-compression
            drichardson_huffman
            hsimple-root
            ;
        };
        devShells.default = pkgs.callPackage ./shell.nix {
          inherit
            arcd
            bench-compression
            drichardson_huffman
            ;
        };
        apps.bench-with-hsimple = {
          type = "app";
          program = "${bench-with-hsimple-script}";
        };
      }
    );
}
