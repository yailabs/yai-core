# yai-core zsh integration.
#
# Source this file once in a terminal. It keeps the normal `yai ...` command
# shape, while allowing `yai case enter ...` to update the parent shell prompt.

if [ -z "${YAI_CORE_BIN:-}" ]; then
  _yai_shell_source="${(%):-%N}"
  _yai_shell_dir="${_yai_shell_source:A:h}"
  export YAI_CORE_BIN="${_yai_shell_dir:h:h}/target/debug/yai"
fi

export YAI=yai

yai() {
  if [ "$#" -ge 2 ] && [ "$1" = "case" ] && { [ "$2" = "enter" ] || [ "$2" = "attach-provider" ]; }; then
    local _yai_has_shell=0
    local _yai_arg
    for _yai_arg in "$@"; do
      if [ "$_yai_arg" = "--shell" ]; then
        _yai_has_shell=1
        break
      fi
    done

    if [ "$_yai_has_shell" -eq 0 ]; then
      local _yai_case_enter_script
      _yai_case_enter_script="$("$YAI_CORE_BIN" "$@" --shell zsh)"
      local _yai_status=$?
      if [ "$_yai_status" -ne 0 ]; then
        print -r -- "$_yai_case_enter_script" >&2
        return "$_yai_status"
      fi
      eval "$_yai_case_enter_script"
      return 0
    fi
  fi

  "$YAI_CORE_BIN" "$@"
}

yai-case-leave() {
  unset YAI_JOURNAL
  unset YAI_CASE_REF
  unset YAI_SUBJECT_REF
  unset YAI_CASE_PROMPT_FLAG

  if [ -n "${YAI_PROMPT_BASE+x}" ]; then
    export PROMPT="$YAI_PROMPT_BASE"
    export PS1="$PROMPT"
    unset YAI_PROMPT_BASE
  fi

  if [ -n "${YAI_RPROMPT_BASE+x}" ]; then
    export RPROMPT="$YAI_RPROMPT_BASE"
    unset YAI_RPROMPT_BASE
  fi
}
