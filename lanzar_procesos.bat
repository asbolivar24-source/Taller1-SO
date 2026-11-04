@echo off
echo === Lanzando 3 procesos de Bloc de Notas ===
start notepad.exe
start notepad.exe
start notepad.exe

echo.
echo === Procesos Notepad en ejecucion actualmente ===
tasklist /FI "IMAGENAME eq notepad.exe"

echo.
echo Esperando 5 segundos antes de cerrar...
timeout /t 5 /nobreak > nul

echo.
echo === Finalizando todos los procesos Notepad ===
taskkill /IM notepad.exe /F

echo.
echo === Finalizado con exito ===
pause