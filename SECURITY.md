# Security policy

HelloFace is an experimental Windows authentication prototype. Do not use it as the only sign-in method or on systems requiring high-assurance authentication.

## Reporting
Please use GitHub Security Advisories for private vulnerability reports. Do not publish exploit details before maintainers have had a reasonable opportunity to investigate.

## In scope
- Credential Provider crashes, hangs or interference with built-in sign-in tiles
- Unauthorized template access or slot/account mapping changes
- Pipe impersonation, spoofing, replay, ACL bypass or privilege escalation
- Authentication on a photo, screen replay, virtual camera, mask or deepfake
- Installer, updater, DLL search-order or registry ACL vulnerabilities

## Known limitations
RGB webcam face matching is not equivalent to Windows Hello IR/depth sensing. Similar relatives, twins, high-quality masks, deepfakes and compromised camera pipelines may defeat it. An administrator or SYSTEM compromise is outside the protection boundary of a local biometric vault.

## Safe testing
Test only on systems and accounts you own or are explicitly authorized to assess. Do not attempt sign-in to another person's account.
