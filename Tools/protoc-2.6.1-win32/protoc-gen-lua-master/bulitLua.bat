rem �л���.protoЭ�����ڵ�Ŀ¼
cd ..\proto
rem ����ǰ�ļ����е�����Э���ļ�ת��Ϊlua�ļ�
for %%i in (*.proto) do (  
echo %%i
rem protoc.exe --plugin=protoc-gen-lua=".\plugin\protoc-gen-lua.bat" --lua_out=. %%i
protoc.exe --plugin=protoc-gen-lua="D:\soft2\protoc-2.6.1-win32\protoc-gen-lua-master\plugin\protoc-gen-lua.bat" --lua_out=..\luascript\. %%i
)
echo end
pause