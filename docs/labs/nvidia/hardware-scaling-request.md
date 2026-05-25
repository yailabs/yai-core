# Hardware Scaling Request

Status: draft request.

## Ask

We need repeatable local-runtime measurements on a Linux NVIDIA host reachable
from a MacBook operator machine over LAN.

## Target Hardware

- NVIDIA GPU with 16 GB VRAM baseline.
- Recent proprietary NVIDIA driver.
- Enough system RAM to avoid swap during model load.
- Wired LAN preferred for provider endpoint tests.

## Measurements

- GPU identity, driver, CUDA visibility and VRAM.
- Model load viability for selected GGUF or provider-native models.
- Generation wall time, peak VRAM and observed throughput.
- Synthetic HNSW build/query latency.
- Provider endpoint smoke through OpenAI-compatible `/v1/models` and
  `/v1/chat/completions`, where implemented.

## Output

Return the generated benchmark directory plus notes about provider launch
commands, model path, quantization and any thermal or power constraints.
