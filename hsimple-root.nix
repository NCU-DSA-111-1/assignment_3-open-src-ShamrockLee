{ lib
, runCommand
, root
}:
# hsimple.root
# a sample file from the tutorial of CERN ROOT,
# a statistical framework widely used
# in the field of High Energy Physics.

runCommand "hsimple.root" { } ''
  cp "${root}/tutorials/hsimple.root" "$out"
''
