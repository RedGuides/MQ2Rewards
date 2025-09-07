---
tags:
  - datatype
---
# `Rewards`

<!--dt-desc-start-->
Information on the number of rewards, and their index or name
<!--dt-desc-end-->

## Members
<!--dt-members-start-->
### {{ renderMember(type='int', name='Count') }}

:   Returns number of currently pending rewards

### {{ renderMember(type='RewardItem', name='Reward', params='#') }}

:   Returns the reward by specified index

### {{ renderMember(type='RewardItem', name='Reward', params='name') }}

:   Returns the reward by name

<!--dt-members-end-->

## Examples
<!--dt-examples-start-->
- ${Rewards.Count}
- ${Rewards.Reward[`<#/name>`]}
<!--dt-examples-end-->

<!--dt-linkrefs-start-->
[int]: ../macroquest/reference/data-types/datatype-int.md
[rewarditem]: datatype-rewarditem.md
<!--dt-linkrefs-end-->
