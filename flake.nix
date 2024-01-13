{
    description = "atom-engine";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    };

    outputs = { self, nixpkgs }:
    let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};

	wjcrypt = pkgs.fetchFromGitHub {
	    owner = "WaterJuice";
	    repo = "WjCryptLib";
	    rev = "Version_2.3.0";
	    sha256 = "sha256-n+Lxj4zubuLjTAJeV7wIIbssV1EICsa1rWjvOYlDh6U";
	};
    in
    {
        devShells.${system}.default = pkgs.mkShell {
            
            packages = with pkgs; [
                cmake
                gnumake
                ninja
		fmt
                catch2
		gcc13
		clang_17
		clang-tools_17
            ];

            env = {
                "WJCRYPT" = wjcrypt;
            };
        };
    };
}
