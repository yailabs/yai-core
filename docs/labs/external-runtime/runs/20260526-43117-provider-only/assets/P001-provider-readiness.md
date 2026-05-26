# P001 - Provider Readiness

| Field | Value |
|---|---|
| Prompt ID | HWB.T001.R002.P001 |
| Source run | HWB.T001.R002 |
| Role | user |
| Source file | `benchmarks/nvidia/lan-provider-20260526-43117-run3-escalated/chat-request.json` |
| Purpose | Provider readiness chat probe |
| Expected output | One-sentence readiness answer if authenticated |
| Observed status | `auth_blocked` |

```text
Rispondi in una frase: provider LAN pronto sulla porta 43117.
```

Limitations:

- The prompt reached `/v1/chat/completions`.
- The provider returned `401 Invalid API Key`.
- No API key or authorization header is recorded here.
