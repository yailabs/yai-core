# NVIDIA Developer Forum Post Draft

Title: Baseline methodology for local OpenAI-compatible LLM provider on single NVIDIA GPU over LAN

I am preparing a local lab where a Linux workstation with a single NVIDIA GPU
serves an OpenAI-compatible LLM endpoint over LAN, while a MacBook remains the
operator machine.

The goal is to collect a practical baseline:

- GPU identity, driver version, CUDA visibility and VRAM usage.
- Provider launch posture for llama.cpp or a comparable local provider.
- Wall time and peak VRAM for a small set of repeatable prompts.
- Basic vector-search synthetic benchmark on the same machine when relevant.

Questions:

1. For a 16 GB VRAM card, which `nvidia-smi` query fields are most useful for a
   repeatable local inference baseline?
2. Are there driver or power-management settings that materially affect
   repeatability for short local inference runs?
3. For single-GPU local inference, are there recommended ways to capture memory
   pressure and throttling without using heavyweight profiling tools?

I am not trying to benchmark NVIDIA against other vendors here. I want a clean
local baseline that can be reproduced and compared across model/provider
configurations on the same host.
