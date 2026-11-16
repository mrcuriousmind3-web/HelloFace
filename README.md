# HelloFace

<div align="center">
  <img src="assets/helloface-mark.svg" width="96" alt="HelloFace logo"/>
  <h3>Private, local-first face matching for Windows</h3>
  <p>C++20 · CMake · Windows 10/11 · ONNX Runtime (planned)</p>
</div>

> **Experimental project. Not a Windows Hello replacement.** RGB webcam matching is vulnerable to sophisticated presentation attacks and must never be the only sign-in method. Keep Windows PIN/password fallback enabled.

## Current status

This repository is the initial, review-first engineering scaffold. It contains a portable, bounded binary IPC protocol with a four-slot match payload, protocol tests, a Windows CI workflow, a minimal brand mark, and security/recovery documentation.

It does **not** yet contain a working Credential Provider, camera service, ONNX models, enrollment UI, installer, signed binaries, or an automatic Windows sign-in flow. The Windows authentication build option is intentionally disabled. Do not install this scaffold as a Credential Provider.

## Design goals

- Four independently managed face-template slots.
- Local inference; no cloud upload of face images or embeddings.
- Camera and model inference outside the Credential Provider DLL.
- Strictly bounded IPC, explicit pipe ACLs, client PID/session/signer validation, replay protection.
- TPM-backed key protection and authenticated encryption for templates.
- Built-in Windows password/PIN providers remain available.
- Explicit enrollment consent, per-slot deletion, and recoverable uninstall.

## Security architecture decisions

A Credential Provider supplies a credential serialization to Windows; it does not create a Windows logon token itself. Storing reusable Windows passwords for later decryption inside LogonUI would create a high-impact credential-theft target. HelloFace will not put reusable account passwords or Windows Hello PINs in its biometric vault.

A SYSTEM service runs in Session 0 and cannot be assumed to access the interactive webcam. A session-aware capture design and physical-device validation are required before lock-screen integration. RGB PAD cannot offer the assurance of dedicated IR/depth hardware.

See [implementation plan](docs/IMPLEMENTATION_PLAN.md), [security policy](SECURITY.md), [privacy](PRIVACY.md), and [recovery guide](RECOVERY.md).

## Build the portable protocol tests

Requirements: CMake 3.24+ and a C++20 compiler.

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Windows-only authentication components are deliberately gated and not part of this initial build. CI runs the portable protocol tests on Windows Server 2022.

## Reference and attribution

Architecture inspiration: [nyxri0f8/winface](https://github.com/nyxri0f8/winface). HelloFace is a separate project; no WinFace source code or model assets are copied into this repository. Third-party model and runtime licences will be documented before dependencies are bundled.

## Branding

The HelloFace mark is in `assets/helloface-mark.svg`. Use this same mark in the configuration app, installer, documentation and release artwork.

## Contributions

Please report security issues privately through GitHub Security Advisories. Do not submit real face images, passwords, biometric templates or personal account data in issues.
