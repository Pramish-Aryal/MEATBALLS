<#
Build script
#>
if(!(Test-Path("./build/"))) {
	New-Item -Itemtype Directory "build"
}

$source_name = "..\code\main.cpp"
$executable_name = "game.exe"

$lib_path = "..\lib\"
$include_path = "..\code\extern\"

$compiler_flags = "/nologo /EHsc /Zi /FC"
$linker_flags = "/NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:LIBCMTD"

$libraries = "raylib.lib shell32.lib user32.lib gdi32.lib winmm.lib opengl32.lib"


Push-Location .\build
#cl /MD $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries $linker_flags /SUBSYSTEM:console
cl -O2 /MD $source_name.split(' ') /Fe$executable_name $compiler_flags.split(' ') /I$include_path /link /LIBPATH:$lib_path $libraries.split(' ') $linker_flags.split(' ') /SUBSYSTEM:console
Pop-Location