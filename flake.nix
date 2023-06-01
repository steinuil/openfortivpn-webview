{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        nativeBuildInputs = with pkgs; [
          extra-cmake-modules
          libsForQt5.wrapQtAppsHook
        ];

        buildInputs = with pkgs; [
          libsForQt5.qtwebengine
        ];
      in
      {
        defaultPackage = with pkgs; stdenv.mkDerivation {
          pname = "openfortivpn-webview";
          version = "1.1.1";

          src = ./.;

          inherit nativeBuildInputs buildInputs;
        };

        devShell = pkgs.mkShell {
          inherit buildInputs nativeBuildInputs;
        };
      });
}
