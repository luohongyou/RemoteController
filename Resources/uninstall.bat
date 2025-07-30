@echo off
cd /d %~dp0

cd Website\bin
net stop "Coral Remote Controller Website"
httpd.exe -k uninstall -n "Coral Remote Controller Website"

netsh advfirewall firewall delete rule name="Coral Remote Controller"

taskkill -f -im agentprotect.exe
taskkill -f -im rcagent.exe
taskkill -f -im rcauth.exe
reg delete "HKLM\Software\Microsoft\Windows\CurrentVersion\Run" /f /v "Coral Remote Controller UI"
