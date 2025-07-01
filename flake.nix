{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      name = "liboqs";
      src = ./.;
      pkgs = nixpkgs.legacyPackages.${system};

      # Function to create compiler-specific package sets
      mkPackageSet = compiler: let
        # Override the stdenv to use the specified compiler
        stdenv =
          if compiler == "clang"
          then pkgs.clangStdenv
          else pkgs.stdenv;

        mkLib = shared:
          stdenv.mkDerivation {
            inherit name src;
            # for whatever reason, trying to 'fix' the CMake file causes a failure
            dontFixCmake = true;

            nativeBuildInputs = with pkgs;
              [cmake ninja doxygen pkg-config graphviz]
              ++ (
                if compiler == "clang"
                then [pkgs.clang]
                else [pkgs.gcc]
              );

            buildInputs = with pkgs; [openssl];

            cmakeFlags = [
              "-GNinja"
              "-DOQS_DIST_BUILD=ON"
              "-DOQS_BUILD_ONLY_LIB=ON"
              "-DBUILD_SHARED_LIBS=${
                if shared
                then "ON"
                else "OFF"
              }"
              "-DCMAKE_INSTALL_LIBDIR=lib"
              "-DCMAKE_INSTALL_INCLUDEDIR=include"
              "-DCMAKE_INSTALL_PREFIX=${placeholder "out"}"
              "-DCMAKE_INSTALL_FULL_LIBDIR=${placeholder "out"}/lib"
              "-DCMAKE_INSTALL_FULL_INCLUDEDIR=${placeholder "out"}/include"
            ];
          };
      in {
        shared = mkLib true;
        static = mkLib false;
      };

      # Create development shell for specified compiler
      mkDevShell = compiler: let
        packageSet = mkPackageSet compiler;
      in
        pkgs.mkShell {
          inherit (packageSet.shared) nativeBuildInputs buildInputs;

          # astyle formats C source code and alejandra formats nix source code
          packages = with pkgs; [astyle alejandra];

          shellHook = ''
            export CMAKE_EXPORT_COMPILE_COMMANDS=1
            echo "Using ${compiler} toolchain"
          '';
        };
    in {
      formatter = pkgs.alejandra;

      packages = {
        default = (mkPackageSet "gcc").shared; # default is gcc shared
        gcc-shared = (mkPackageSet "gcc").shared;
        clang-shared = (mkPackageSet "clang").shared;
        gcc-static = (mkPackageSet "gcc").static;
        clang-static = (mkPackageSet "clang").static;
      };

      # Development shells
      devShells = {
        default = mkDevShell "gcc";
        gcc = mkDevShell "gcc";
        clang = mkDevShell "clang";
      };
    });
}
