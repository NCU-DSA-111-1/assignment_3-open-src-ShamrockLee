{ lib
, stdenv
, pkg-config
, arcd
, drichardson_huffman
}:

stdenv.mkDerivation {
  pname = "bench-compression";
  version = "0.1.0";

  src = lib.cleanSource ./.;

  buildInputs = [
    arcd
    drichardson_huffman
  ];

  NIX_CFLAGS_COMPILE = toString [
    "-g"
    "-O3"
    "-std=c11"
    "-Werror"
    "-I${lib.getDev arcd}/include"
    "-I${lib.getDev drichardson_huffman}/include"
  ];
  
  NIX_CFLAGS_LINK = toString [
    "${lib.getLib arcd}/lib/*.a"
    "${lib.getLib drichardson_huffman}/lib/*.a"
  ];

  buildPhase = ''
    runHook preBuild
    echo "NIX_CFLAGS_COMPILE: ''${NIX_CFLAGS_COMPILE-}" >&2
    echo "NIX_CFLAGS_LINK: ''${NIX_CFLAGS_LINK-}" >&2
    cc $NIX_CFLAGS_COMPILE -fPIC -shared -o bench_case_arcd.so bench_case_arcd.c
    cc $NIX_CFLAGS_COMPILE -c -o bench.o bench.c
    cc $NIX_CFLAGS_LINK -o bench bench.o bench_case_arcd.so
    runHook postBuild0
  '';

  installPhase = ''
    runHook preInstall
    mkdir -p "$out/bin"
    install -m 755 bench "$out/bin"
    mkdir -p "$out/lib"
    install -m 644 *.so "$out/lib"
    mkdir -p "$out/include/bench-compression"
    install -m 644 *.h "$out/include/bench-compression"
    runHook postInstall
  '';

  meta = with lib; {
    description = "A small project to benchmark some open source projects.";
    platforms = platforms.all;
    maintainers = with maintainers; [ ShamrockLee ];
    mainProgram = "bench";
  };
}
