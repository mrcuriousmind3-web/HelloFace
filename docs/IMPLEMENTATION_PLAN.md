# HelloFace implementation plan

## Scope and security posture
HelloFace is an experimental RGB-webcam convenience sign-in prototype, not a Windows Hello replacement and not suitable for high-assurance authentication. RGB presentation-attack detection can be defeated by sophisticated replay, masks, virtual cameras, look-alike relatives and deepfakes. A match must never be described as proof of identity.

Reference: https://github.com/nyxri0f8/winface. Its modular C++20/CMake layout, on-device inference, enrollment utility, recovery guide and testing approach are useful architectural references. HelloFace does not copy its implementation or assets; model weights and dependencies require separate licence review.

## Corrected process architecture
- Credential Provider DLL: COM adapter only; bounded work, cancellation, no model loading and no camera loop in DllMain. It must never hide or modify built-in providers.
- Capture/inference: session-aware component. A Windows service in Session 0 cannot assume it can open the user's interactive camera. Validate camera ownership, secure-desktop transitions and OS-version behavior on physical Windows hardware before enabling lock-screen mode.
- Privileged broker: if a service is retained, it exposes only a narrow authenticated protocol; it never accepts a bare AUTH_SUCCESS message as authorization.
- Enrollment GUI: ordinary user-mode app; elevation only for installation and machine-wide configuration.
- Vault: CNG/TPM-protected key material and authenticated encryption. No plaintext templates, images or reusable account passwords in files, logs, command lines or IPC.

## Authentication constraint
A custom Credential Provider supplies credential serialization to Windows; it does not independently mint a Windows logon token. A reusable password vault is high impact: compromise of the provider, SYSTEM service, administrator account or TPM authorization path can expose account credentials. The first implementation therefore targets a non-authoritative match/demo flow and a non-destructive CredUI test harness. Any real automatic sign-in path requires a separately reviewed credential lifecycle, local-account-only scope, password-rotation recovery, Windows LSA serialization review, and explicit opt-in. Microsoft account passwords and Windows Hello PINs are not interchangeable.

## Proposed directory tree
HelloFace/
- CMakeLists.txt
- README.md, LICENSE, SECURITY.md, PRIVACY.md, RECOVERY.md
- assets/helloface-mark.svg
- include/helloface/ipc_protocol.hpp, crypto_vault.hpp
- src/provider/ (COM adapter; gated until reviewed)
- src/broker/ (authenticated pipe endpoint)
- src/vision/ (Media Foundation + ONNX Runtime)
- src/vault/ (CNG, AES-GCM, TPM key lifecycle)
- src/config/ (enrollment and four-slot UI)
- tests/protocol_tests.cpp
- scripts/Register-HelloFace.ps1, Unregister-HelloFace.ps1
- .github/workflows/windows-build.yml

## CMake strategy
The root CMake project is C++20 and builds portable protocol tests by default. Windows-only authentication components remain behind an explicit OFF-by-default option until their code, signing, integration tests and recovery behavior are reviewed. ONNX Runtime and model weights are acquired as pinned, licence-reviewed dependencies; no unverified binary is checked into source control.

## Registry and COM registration
Use a dedicated CLSID and register only during an elevated, transactional install. The provider registration belongs under:
- HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Authentication\Credential Providers\{CLSID}
- HKLM\SOFTWARE\Classes\CLSID\{CLSID}\InprocServer32

The install script must verify the signed DLL path, architecture, ACLs and file hashes before writing either key. The uninstall script removes only HelloFace's own CLSID and files. Do not ship an auto-enable registry switch until physical-machine fallback tests pass. Built-in PIN/password providers must remain untouched.

## Named-pipe hardening checklist
1. Create the pipe with an explicit SDDL DACL; allow only SYSTEM and the intended logon identity.
2. Use PIPE_REJECT_REMOTE_CLIENTS, message mode, bounded message sizes, strict timeouts and one request per connection.
3. Call GetNamedPipeClientProcessId; verify PID liveness and expected session.
4. Query client image path and verify Authenticode signer and expected install path; reject reparse-point/path substitution.
5. Impersonate only when required, inspect client token SID/session/integrity level, then immediately RevertToSelf.
6. Use a per-boot random nonce, monotonic request ID, expiry, and MAC/signature over the complete request and response. Reject replay and stale responses.
7. Never trust a client-supplied slot ID, SID, score or liveness flag. The trusted decision component computes these.
8. Fail closed on malformed data, disconnects, timeout, service restart or unavailable camera. Keep Windows password/PIN tiles available.
9. Log event IDs and reason codes only; never log images, embeddings, passwords, tokens or raw IPC payloads.
10. Fuzz the parser and run service/pipe ACL tests under standard-user, admin and SYSTEM identities.

## Threat model
| Threat | Mitigation | Residual risk |
|---|---|---|
| Printed photo / screen replay | PAD model plus randomized active challenge | RGB PAD is not robust against all replay/deepfake attacks |
| Flat image parallax spoof | Multi-frame geometry and challenge | Landmark noise and 3D masks remain |
| Pipe spoofing / replay | ACL, PID + signer checks, nonce, MAC, expiry | SYSTEM/admin compromise defeats local controls |
| Template theft | AES-256-GCM, TPM-protected key, strict ACLs | SYSTEM/admin can access runtime plaintext |
| DLL hijack / tampering | absolute paths, ACLs, signature verification, CFG/ASLR | compromised trusted signer/update pipeline |
| Lock-screen denial of service | bounded work, watchdog, timeout, untouched fallback tiles | provider bugs can still destabilize LogonUI |
| Wrong-account selection | SID binding, explicit user selection, no silent cross-user mapping | similar faces / twins and model errors |

## Model and data licensing
Before bundling detector, embedding and PAD models, record exact upstream URL, version, SHA-256, licence and intended commercial/non-commercial use. Do not download from arbitrary mirrors. No model weights are included in this initial scaffold.
