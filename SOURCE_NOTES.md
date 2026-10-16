# Source and reconstruction notes

Source: the user-supplied 16-page `Smart_Irrigation_Code.pdf`, titled Smart Irrigation - Full Code Explanation (Bangla).

The document provides explanations and separate code blocks, with ellipses in some display branches. This repository assembles an executable sketch from those documented behaviors rather than claiming to contain the original source file.

Preserved: Uno pin map; active-LOW rain and relay signals; soil threshold 500; tank height 6.3 cm; low-water depth 2.3 cm; 30,000 microsecond echo timeout; 0.0343 cm/microsecond distance factor; DHT and ultrasonic error shutoff; LED meanings; LCD status priorities; 9600 baud and two-second loop delay.

Adaptations: pure control logic is in a testable header; pins and settings are constants; the relay OFF value is set before enabling its output and before sensor initialization; omitted LCD/Serial branches are completed using the documented text; unused high/low temperature variables are omitted. LCD and Serial labels are reconstructed, not byte-for-byte copies. Serial strings use Arduino's flash-string macro to conserve RAM.

No original PDF, embedded metadata, local paths, author identity, or credentials are distributed. The project has no Wi-Fi or cloud account configuration.
