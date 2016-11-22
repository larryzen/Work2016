rem 切换到.proto协议所在的目录
cd ..\proto
rem 将当前文件夹中的所有协议文件转换为lua文件
for %%i in (*.proto) do (  
echo %%i
rem protoc.exe --plugin=protoc-gen-lua=".\plugin\protoc-gen-lua.bat" --lua_out=. %%i
protoc.exe --plugin=protoc-gen-lua="D:\soft2\protoc-2.6.1-win32\protoc-gen-lua-master\plugin\protoc-gen-lua.bat" --lua_out=..\luascript\. %%i
)
echo end
pause