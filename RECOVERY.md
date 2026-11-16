# HelloFace recovery and emergency removal

Keep a Windows PIN or password available at all times. HelloFace is experimental. Do not enable a lock-screen provider on a machine you cannot physically recover.

## If Windows still lets you sign in
1. Sign in using the built-in PIN or password tile.
2. Open PowerShell as Administrator.
3. Run the project uninstaller: powershell -ExecutionPolicy Bypass -File .\scripts\Unregister-HelloFace.ps1.
4. Restart Windows.

## If the sign-in UI is broken: enter Windows Recovery Environment
1. From the sign-in screen, hold Shift while selecting Power > Restart. If unavailable, interrupt startup twice to trigger Windows Recovery.
2. Select Troubleshoot > Advanced options > Command Prompt.
3. Unlock the Windows volume if BitLocker is enabled. You will need the recovery key.
4. Identify the Windows volume. In WinRE it may not be C:. Use diskpart, list volume, then exit.
5. Load the offline SOFTWARE hive (replace D: with the actual Windows volume):

    reg load HKLM\OFFSOFT D:\Windows\System32\config\SOFTWARE
    reg delete "HKLM\OFFSOFT\Microsoft\Windows\CurrentVersion\Authentication\Credential Providers\{YOUR-HELLOFACE-CLSID}" /f
    reg unload HKLM\OFFSOFT

Replace {YOUR-HELLOFACE-CLSID} with the CLSID shown in the installed version's release notes. Do not delete any other provider keys.

6. Type exit and select Continue to Windows. Built-in Windows providers should be available again.

## Safe Mode alternative
From WinRE choose Troubleshoot > Advanced options > Startup Settings > Restart, then press 4 for Safe Mode. Sign in with the password if requested, disable/uninstall HelloFace, and restart normally.

## Important
- Never delete the entire Credential Providers registry key.
- Do not disable Windows Hello, PIN or password providers to troubleshoot HelloFace.
- If BitLocker is enabled, make sure the recovery key is available before changing boot or registry settings.
- Test recovery on a spare PC or VM before deploying.
