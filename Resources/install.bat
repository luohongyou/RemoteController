@echo off
cd /d %~dp0

cacls . /t /e /g everyone:f

netsh advfirewall firewall delete rule name="Coral Remote Controller"
netsh advfirewall firewall add rule name="Coral Remote Controller" protocol=TCP dir=in localport=8088-8089 action=allow

cd Website\bin
httpd.exe -k install -n "Coral Remote Controller Website"
net start "Coral Remote Controller Website"

cd ..\..\ManageUI\agent
reg add "HKLM\Software\Microsoft\Windows\CurrentVersion\Run" /f /v "Coral Remote Controller UI" /t REG_SZ /d "%~dp0ManageUI\agent\agentprotect.exe"
start agentprotect.exe
