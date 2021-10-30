{
    "targets": [{
        "target_name": "MediaDevicesUtil",
        "include_dirs": [
          "<!@(node -p \"require('node-addon-api').include\")"
        ],
        "sources": [
          "./src/Device.cpp",
          "./src/NapiUtil.cpp"
        ],
        "conditions": [
            ["OS == 'mac'", {
                "sources": [
                  "./src/MediaDevicesUtilMac.mm"
                ],
                "cflags+": ["-fvisibility=hidden"],
                "xcode_settings": {
                    "GCC_SYMBOLS_PRIVATE_EXTERN": "YES",
                    "OTHER_CFLAGS": ["-ObjC++"],
                    "WARNING_CFLAGS": [
                        "-Wno-sign-compare",
                        "-Wno-unused-variable",
                        "-Wno-unused-function",
                        "-Wno-ignored-qualifiers"
                    ],
                    "OTHER_CPLUSPLUSFLAGS": [
                        "-mmacosx-version-min=10.8",
                        "-stdlib=libc++",
                        "-arch x86_64",
                        "-arch arm64"
                    ],
                    "OTHER_LDFLAGS": [
                        "-stdlib=libc++",
                        "-arch x86_64",
                        "-arch arm64"
                    ],
                    "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                    "CLANG_CXX_LIBRARY": "libc++",
                    "MACOSX_DEPLOYMENT_TARGET": "10.8"
                },
                "link_settings": {
                    "libraries": [
                        "-framework AVFoundation"
                    ]
                }
            }],
            ["OS == 'win'", {
                "sources": [
                  "./src/MediaDevicesUtilWin.cpp"
                ],
                "defines": [
                    "_HAS_EXCEPTIONS=1",
                    "OS_WIN=1"
                ],
                "msvs_settings": {
                    "VCCLCompilerTool": {
                        "RuntimeTypeInfo": "false",
                        "EnableFunctionLevelLinking": "true",
                        "ExceptionHandling": "2",
                    },
                    "VCLinkerTool": {
                        "AdditionalDependencies": [
                            "strmiids.lib"
                        ]
                    }
                }
            }]
        ]
    }]
}
