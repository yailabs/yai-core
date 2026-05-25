#!/usr/bin/env bash
set -u

model=""
workload=""
out_dir=""

while [ "$#" -gt 0 ]; do
  case "$1" in
    --model)
      model="${2:-}"
      shift 2
      ;;
    --workload)
      workload="${2:-}"
      shift 2
      ;;
    --out)
      out_dir="${2:-}"
      shift 2
      ;;
    --)
      shift
      break
      ;;
    *)
      printf 'unknown argument before --: %s\n' "$1" >&2
      exit 2
      ;;
  esac
done

if [ -z "$model" ] || [ -z "$workload" ] || [ -z "$out_dir" ] || [ "$#" -eq 0 ]; then
  printf 'usage: %s --model MODEL --workload WORKLOAD --out OUT_DIR -- COMMAND [ARGS...]\n' "$0" >&2
  exit 2
fi

mkdir -p "$out_dir"
printf '%s\n' "$*" >"$out_dir/command.txt"

samples_file="$out_dir/gpu-samples.csv"
sampler_pid=""
if command -v nvidia-smi >/dev/null 2>&1; then
  (
    printf 'timestamp,index,name,utilization.gpu,memory.used,memory.total,power.draw,temperature.gpu\n'
    while true; do
      nvidia-smi --query-gpu=timestamp,index,name,utilization.gpu,memory.used,memory.total,power.draw,temperature.gpu --format=csv,noheader,nounits 2>/dev/null
      sleep 1
    done
  ) >"$samples_file" &
  sampler_pid="$!"
fi

start_epoch="$(date +%s)"
start_ns="$(date +%s%N 2>/dev/null || date +%s000000000)"
"$@" >"$out_dir/stdout.txt" 2>"$out_dir/stderr.txt"
status="$?"
end_ns="$(date +%s%N 2>/dev/null || date +%s000000000)"
end_epoch="$(date +%s)"

if [ -n "$sampler_pid" ]; then
  kill "$sampler_pid" >/dev/null 2>&1 || true
  wait "$sampler_pid" >/dev/null 2>&1 || true
fi

wall_seconds="$(awk -v s="$start_ns" -v e="$end_ns" 'BEGIN { printf "%.3f", (e - s) / 1000000000 }')"
estimated_tokens="$(wc -w <"$out_dir/stdout.txt" | tr -d ' ')"
estimated_tok_s="$(awk -v t="$estimated_tokens" -v w="$wall_seconds" 'BEGIN { if (w > 0) printf "%.3f", t / w; else printf "0.000" }')"

peak_vram_mib=""
if [ -s "$samples_file" ]; then
  peak_vram_mib="$(awk -F, 'NR > 1 { gsub(/^[ \t]+|[ \t]+$/, "", $5); if ($5 + 0 > max) max = $5 + 0 } END { if (NR > 1) print max }' "$samples_file")"
fi

cat >"$out_dir/summary.json" <<EOF
{
  "schema": "yai.local_model_benchmark.v1",
  "created_at_utc": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
  "model": "$model",
  "workload": "$workload",
  "exit_status": $status,
  "start_epoch": $start_epoch,
  "end_epoch": $end_epoch,
  "wall_time_seconds": $wall_seconds,
  "estimated_output_tokens": $estimated_tokens,
  "estimated_tokens_per_second": $estimated_tok_s,
  "peak_vram_mib": "${peak_vram_mib}",
  "files": {
    "command": "command.txt",
    "stdout": "stdout.txt",
    "stderr": "stderr.txt",
    "gpu_samples": "gpu-samples.csv"
  }
}
EOF

printf 'benchmark written: %s\n' "$out_dir"
exit "$status"
