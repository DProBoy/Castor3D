@echo OFF

set DATA_DIR=..\..\..\..\source\Core\Castor3D\Test\Data

setlocal disableDelayedExpansion
for /f "delims=" %%A in ('forfiles /s /p %DATA_DIR% /m *.cscn /c "cmd /c echo @relpath"') do (
	set "file=%%~A"
	setlocal enableDelayedExpansion
	echo "Testing !file:~2!"
	CastorViewer /opengl %DATA_DIR%\!file:~2!
	endlocal
)