#!/usr/bin/env bash
set -u

out_dir="${1:-}"
if [ -z "$out_dir" ]; then
  printf 'usage: %s OUT_DIR\n' "$0" >&2
  exit 2
fi

mkdir -p "$out_dir"

run_capture() {
  name="$1"
  shift
  if command -v "$1" >/dev/null 2>&1; then
    "$@" >"$out_dir/$name" 2>"$out_dir/$name.err"
  else
    printf 'missing command: %s\n' "$1" >"$out_dir/$name.err"
  fi
}

run_capture uname.txt uname -a
run_capture lscpu.txt lscpu
run_capture free.txt free -h
run_capture env-path.txt printenv PATH
run_capture nvidia-smi.txt nvidia-smi

if command -v nvidia-smi >/dev/null 2>&1; then
  nvidia-smi --query-gpu=index,name,uuid,driver_version,cuda_version,memory.total,memory.used,power.limit,power.draw,temperature.gpu,clocks.sm,clocks.mem,pcie.link.gen.current,pcie.link.width.current --format=csv >"$out_dir/nvidia-smi-query.csv" 2>"$out_dir/nvidia-smi-query.csv.err"
fi

for cmd in nvcc docker llama-cli llama-server curl python3; do
  if command -v "$cmd" >/dev/null 2>&1; then
    path="$(command -v "$cmd")"
    version="$("$cmd" --version 2>&1 | head -n 5)"
    printf '%s\t%s\t%s\n' "$cmd" "$path" "$version" >>"$out_dir/tool-versions.tsv"
  else
    printf '%s\tmissing\n' "$cmd" >>"$out_dir/tool-versions.tsv"
  fi
done

cat >"$out_dir/baseline.json" <<EOF
{
  "schema": "yai.nvidia_baseline.v1",
  "created_at_utc": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
  "out_dir": "$out_dir",
  "files": [
    "uname.txt",
    "lscpu.txt",
    "free.txt",
    "nvidia-smi.txt",
    "nvidia-smi-query.csv",
    "tool-versions.tsv"
  ]
}
EOF

printf 'baseline written: %s\n' "$out_dir"
