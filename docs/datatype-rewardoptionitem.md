---
tags:
  - datatype
---
# `RewardOptionItem`

<!--dt-desc-start-->
Returns name of the indicated option if available; else NULL. Must specify reward, option, and item, e.g. `${Rewards.Reward[<#/name>].Item[#/name].Text}`
<!--dt-desc-end-->

## Members
<!--dt-members-start-->
### {{ renderMember(type='string', name='Text') }}

:   Returns name of the indicated option if available; else NULL

<!--dt-members-end-->

## Examples
<!--dt-examples-start-->
- `${Rewards.Reward[2].Item[3].Text}`
<!--dt-examples-end-->

<!--dt-linkrefs-start-->
[string]: ../macroquest/reference/data-types/datatype-string.md
<!--dt-linkrefs-end-->
