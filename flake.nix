{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";
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

      # Function to create compiler-specific package sets.
      # minimalBuild: null builds the full default algorithm set
      # (OQS_DIST_BUILD=ON, what end users of the flake get). Passing a
      # non-null string (e.g. "KEM_ml_kem_768;SIG_ml_dsa_65") switches to
      # OQS_MINIMAL_BUILD instead -- used by the "ci" package below to keep
      # the CI flake-build check fast, since it only needs to validate that
      # the flake mechanism itself works, not compile all ~330+ algorithms
      # on every single commit
      mkPackageSet = compiler: minimalBuild: let
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
              (
                if minimalBuild == null
                then "-DOQS_DIST_BUILD=ON"
                else "-DOQS_MINIMAL_BUILD=${minimalBuild}"
              )
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
        packageSet = mkPackageSet compiler null;
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
        default = (mkPackageSet "gcc" null).shared; # default is gcc shared, full algorithm set
        gcc-shared = (mkPackageSet "gcc" null).shared;
        clang-shared = (mkPackageSet "clang" null).shared;
        gcc-static = (mkPackageSet "gcc" null).static;
        clang-static = (mkPackageSet "clang" null).static;
        # Minimal-build variant used by CI (basic-checks/nixflakecheck) to
        # keep the flake-build check fast -- validates the flake mechanism
        # itself without compiling all ~330+ algorithms on every commit.
        # End users of the flake are unaffected: `nix build` (no target)
        # still resolves to `default`, the full OQS_DIST_BUILD=ON package.
        ci = (mkPackageSet "gcc" "KEM_ml_kem_768;SIG_ml_dsa_65").shared;
      };

      # Development shells
      devShells = {
        default = mkDevShell "gcc";
        gcc = mkDevShell "gcc";
        clang = mkDevShell "clang";
      };
    });
}
