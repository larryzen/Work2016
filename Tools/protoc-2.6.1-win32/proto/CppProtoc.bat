rem �л���.protoЭ�����ڵ�Ŀ¼
rem cd .\proto
rem ����ǰ�ļ����е�����Э���ļ�ת��Ϊlua�ļ�
for %%i in (*.proto) do (  
echo %%i
protoc.exe --cpp_out=..\Cpp\ %%i
)
echo end
Pause