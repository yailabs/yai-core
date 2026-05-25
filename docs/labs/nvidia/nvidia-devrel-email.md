# NVIDIA DevRel Email Draft

Subject: Request for guidance: repeatable single-GPU local LLM provider baseline

Hello,

I am preparing a local lab baseline for a Linux NVIDIA workstation serving a
local OpenAI-compatible LLM provider over LAN to a MacBook operator machine.

The initial target is a single 16 GB VRAM GPU. I plan to collect:

- GPU and driver inventory through `nvidia-smi`.
- Provider launch configuration.
- Prompt wall time, estimated throughput and peak VRAM.
- Basic synthetic vector-search measurements where relevant.

I would appreciate guidance on the smallest useful set of NVIDIA-side telemetry
for repeatable local inference measurements, especially fields or settings that
help explain throttling, power limits, memory pressure or driver differences.

The goal is a practical reproducible baseline, not a marketing benchmark.

Thank you.
