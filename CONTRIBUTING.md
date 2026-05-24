# Contributing

YAI is source-available for public technical evaluation and review. Broad
external contributions are not open yet.

Technical feedback and issue reports are welcome. Pull requests may be
considered when they are explicitly scoped to documentation, tests, or small
fixes, but maintainers may decline or defer external changes while the
repository surface is still being stabilized.

Contributions do not change the license posture of the repository. By
submitting feedback, patches, documentation, tests, or other material, you
confirm that you have the right to submit it and that it does not include code,
data, secrets, generated output, or third-party material you are not authorized
to contribute.

Before submitting a scoped change, run the smallest relevant validation. The
current repository-level checks are exposed through:

```sh
make check-docs
make check
git diff --check
```

`make check` is broader and may build and run smoke tests. Use it when the
change touches source, tests, commands, or repository structure.
