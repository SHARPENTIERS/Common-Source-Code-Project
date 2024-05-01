echo off

if exist "%ProgramFiles(x86)%" goto is_x64
set path="%ProgramFiles%\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin";%PATH%
goto start

:is_x64
set path="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin";%PATH%

:start
rmdir /s /q SHARP
mkdir SHARP
mkdir SHARP\mz80a
mkdir SHARP\mz80b
mkdir SHARP\mz80k
mkdir SHARP\mz700
mkdir SHARP\mz700
mkdir SHARP\mz700pal
mkdir SHARP\mz800
mkdir SHARP\mz1200
mkdir SHARP\mz1500
mkdir SHARP\mz2200
mkdir SHARP\mz2500
mkdir SHARP\mz2800
mkdir SHARP\mz3500
mkdir SHARP\mz5500
mkdir SHARP\mz6500

msbuild.exe mz80a.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz80a.exe SHARP\mz80a\.

msbuild.exe mz80b.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz80b.exe SHARP\mz80b\.

msbuild.exe mz80k.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz80k.exe SHARP\mz80k\.

msbuild.exe mz700.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz700.exe SHARP\mz700\.

msbuild.exe mz700pal.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz700pal.exe SHARP\mz700pal\.

msbuild.exe mz800.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz800.exe SHARP\mz800\.

msbuild.exe mz1200.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz1200.exe SHARP\mz1200\.

msbuild.exe mz1500.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz1500.exe SHARP\mz1500\.

msbuild.exe mz2200.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz2200.exe SHARP\mz2200\.

msbuild.exe mz2500.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz2500.exe SHARP\mz2500\.

msbuild.exe mz2800.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz2800.exe SHARP\mz2800\.

msbuild.exe mz3500.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz3500.exe SHARP\mz3500\.

msbuild.exe mz5500.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz5500.exe SHARP\mz5500\.

msbuild.exe mz6500.vcxproj /t:clean;build /p:Configuration=Release;Platform="Win32"
copy bin\x86\Release\mz6500.exe SHARP\mz6500\.

pause
echo on
exit /b
