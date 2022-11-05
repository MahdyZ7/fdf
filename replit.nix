{ pkgs }: {
	deps = [
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
		pkgs.readline
		pkgs.xorg.libX11
		pkgs.gtk3-x11
		pkgs.nmap
		pkgs.libbsd
	];
}