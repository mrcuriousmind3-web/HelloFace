#requires -RunAsAdministrator
$ErrorActionPreference = 'Stop'
$clsid = '{9B6F8A21-7C4D-4D2F-9A31-8E0C5B17F462}'
$providerKey = "HKLM:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\Credential Providers\\$clsid"
$comKey = "HKLM:\\SOFTWARE\\Classes\\CLSID\\$clsid"
Write-Host "Removing only HelloFace provider registration ($clsid)."
if (Test-Path -LiteralPath $providerKey) { Remove-Item -LiteralPath $providerKey -Recurse -Force }
if (Test-Path -LiteralPath $comKey) { Remove-Item -LiteralPath $comKey -Recurse -Force }
Write-Host "Registry cleanup complete. Restart Windows."
Write-Warning "This scaffold does not install a provider DLL or service. Remove program files only after verifying their exact install path."
