# RTX 5060 Ti 16GB Baseline

Status: expected lab target, not verified repository evidence.

Use this page to record the first clean Linux NVIDIA LAN baseline.

## Machine

```text
host:
os:
kernel:
cpu:
ram:
gpu:
driver:
cuda:
provider:
model:
model_format:
quantization:
```

## First Baseline Command

```bash
tools/labs/collect-gpu-baseline.sh benchmarks/nvidia/baseline-$(date +%Y%m%d-%H%M%S)
```

## Minimum Evidence

- `baseline.json`
- `nvidia-smi.txt`
- `nvidia-smi-query.csv`
- provider launch command
- model benchmark `summary.json`
- HNSW benchmark `summary.json`

## Notes

Record thermal limits, power limits, PCIe mode, driver version and whether the
provider was the only major GPU workload during the run.
