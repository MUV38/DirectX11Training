@echo off
set EXE_NAME=%~dp0MakeSpriteFont.exe
cd ..
cd ..
cd DATA\Font
set FONT_NAME=sprite_font.spf

@echo on
rem %EXE_NAME%
rem %FONT_NAME%
%EXE_NAME% consolas %FONT_NAME% /FontSize:12

@echo off
pause