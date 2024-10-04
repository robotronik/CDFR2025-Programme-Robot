mkdir -p .vscode
cat <<EOT > .vscode/c_cpp_properties.json
{
    "configurations": [
        {
            "name": "CDFR25",
            "includePath": [
                "\${workspaceFolder}/**",
                "\${workspaceFolder}/include",
                "\${workspaceFolder}/../librairie-commune/**",
                "\${workspaceFolder}/../librairie-commune/include"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64",
            "configurationProvider": "ms-vscode.makefile-tools"
        }
    ],
    "version": 4
}
EOT
echo ".vscode/c_cpp_properties.json created!"