# HelloFace privacy

The intended design is local-first. Face images are processed in memory for enrollment and matching and are not intended to be retained as files. Enrolled biometric templates are sensitive personal data and must be encrypted at rest, access-controlled and deletable per slot.

The initial repository scaffold does not capture camera data, enroll faces, authenticate users or transmit telemetry. No model files are included.

Before any camera or enrollment feature is enabled, the app must show a clear consent notice, explain retention and deletion, identify the model/licence, and provide a one-action erase function. Diagnostic logs must exclude images, embeddings, credentials, access tokens and raw IPC messages.
