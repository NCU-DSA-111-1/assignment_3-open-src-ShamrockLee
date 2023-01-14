{ lib
, stdenv
, fetchFromGitHub
, cmake
}:

stdenv.mkDerivation {
  pname = "drichardson_huffman-unstable";
  version = "2021-04-25";

  src = fetchFromGitHub {
    owner = "wonder-mice";
    repo = "arcd";
    rev = "102150ac8ca5d5b8aa4a9ddd897a98e6793b8342";
    hash = "sha256-DF82uScdfjSP7LqscU9t1j3U0HnvY5k2yF+8ZAKohlM=";
  };

  nativeBuildInputs = [
    cmake
  ];

  # Use -O3
  preBuild = ''
    buildFlagsArray+=( "CFLAGS=-g -Wall -Werror -Wextra -O3 -std=c11 -D_POSIX_C_SOURCE=2" )
  '';

  meta = with lib; {
    description = "A huffman coding library and command line interface in C";
    homepage = "https://github.com/drichardson/huffman";
    platforms = platforms.all;
    maintainers = with maintainers; [ ShamrockLee ];
  };
}
