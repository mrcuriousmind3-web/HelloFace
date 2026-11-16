# HelloFace — AI explainer video production script

**Format:** 75–90 seconds, 16:9, 1080p, clean motion graphics, calm technical narration.  
**Brand:** Use `assets/helloface-mark.svg` as the persistent lower-right watermark and opening/closing mark. Palette: deep navy `#101C2C`, mint `#70E1B5`, off-white `#F4F7FB`.  
**Accuracy note:** This is an explainer of the intended architecture and current prototype status, not a demonstration of a working Windows face-login product.

## Narration and visual storyboard

### 0:00–0:08 — The idea
**Visual:** Windows lock screen silhouette. A webcam icon illuminates; HelloFace logo draws on.
**Voiceover:** “Meet HelloFace: an open-source research project exploring local face matching on ordinary RGB webcams for Windows.”
**On-screen:** “HelloFace · Local-first · Up to four profiles”

### 0:08–0:19 — Four profiles
**Visual:** Four profile cards appear, labelled Slot 1 through Slot 4. Each shows a face-outline glyph, not a real person's photograph.
**Voiceover:** “The goal is simple: support up to four independently managed face profiles, with encrypted templates and explicit account mapping.”
**On-screen:** “4 slots · Encrypted templates · User-controlled”

### 0:19–0:32 — The pipeline
**Visual:** Animated flow: webcam → face box and landmarks → aligned crop → embedding vector → four-slot matcher.
**Voiceover:** “A camera pipeline captures frames, a detector locates the face, alignment standardizes the crop, and an on-device model compares a compact feature vector against enrolled profiles.”
**On-screen:** “Capture → Detect → Align → Embed → Compare”

### 0:32–0:45 — Liveness and limitations
**Visual:** Three layers: texture analysis, geometric movement, optional blink/head-turn challenge. A photo replay is rejected with a warning symbol.
**Voiceover:** “A presentation-attack pipeline can combine texture cues, motion geometry and active challenges. But RGB cameras have limits: no software can promise perfect spoof resistance.”
**On-screen:** “RGB is not IR/depth. Keep PIN/password fallback.”

### 0:45–0:58 — Security architecture
**Visual:** Credential Provider tile separated from a broker and inference process. A shield encloses encrypted templates and a TPM icon.
**Voiceover:** “The Windows sign-in component stays lightweight. Camera and model work stay out of LogonUI. A hardened local protocol, strict permissions and TPM-backed key protection are part of the design.”
**On-screen:** “Least privilege · Bounded IPC · TPM-backed keys”

### 0:58–1:10 — Honest project status
**Visual:** Repository view highlights protocol tests, security plan, recovery guide and CI workflow. A status label reads “Early scaffold”.
**Voiceover:** “Today, HelloFace is an early engineering scaffold: the protocol, four-slot payload, documentation and Windows CI are in place. The actual camera, model, enrollment UI and production Credential Provider still need implementation and hardware testing.”
**On-screen:** “Early scaffold — not ready for sign-in”

### 1:10–1:20 — Close
**Visual:** Four profile outlines resolve into the HelloFace mark. Repository URL appears.
**Voiceover:** “Follow development, review the security plan, and help make the next steps safer. HelloFace: local-first research, with recovery always in reach.”
**On-screen:** “github.com/mrcuriousmind3-web/HelloFace”

## Production notes
- Do not show a fake successful Windows sign-in or claim the prototype authenticates users.
- Use abstract profile icons rather than stock face imagery.
- Burn in captions; keep the logo watermark visible throughout.
- Use subtle electronic background music with narration clearly in front.
- Add an end-card disclaimer: “Experimental RGB face matching. Not Windows Hello. Never disable fallback sign-in.”
