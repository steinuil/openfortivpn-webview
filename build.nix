{ stdenv
, extra-cmake-modules
, libsForQt5
}:
stdenv.mkDerivation {
  pname = "openfortivpn-webview";
  version = "1.1.1";

  src = ./.;

  nativeBuildInputs = [ extra-cmake-modules libsForQt5.wrapQtAppsHook ];

  buildInputs = [ libsForQt5.qt5.qtwebengine ];
}
