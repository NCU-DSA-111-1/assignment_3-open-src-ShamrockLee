{ lib
, stdenv
, fetchFromGitHub
, cmake
}:

stdenv.mkDerivation {
  pname = "arcd-unstable";
  version = "2015-10-19";

  src = fetchFromGitHub {
    owner = "wonder-mice";
    repo = "arcd";
    rev = "98e06d12b03f6824bef8fb7851972ace9c9ea436";
    hash = "sha256-D1mQdxv1AjQ79ivVJhiwEnR8F8ZakROKa/3WlM3uyWA=";
  };

  nativeBuildInputs = [
    cmake
  ];

  cmakeFlags = [
    "-DCMAKE_C_FLAGS=-O3"
  ];

  meta = with lib; {
    description = " Simple arithmetic coding library in C";
    homepage = "https://github.com/wonder-mice/arcd";
    platforms = platforms.all;
    maintainers = with maintainers; [ ShamrockLee ];
  };
}
