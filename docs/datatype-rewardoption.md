---
tags:
  - datatype
---
# `RewardOption`

<!--dt-desc-start-->
Returns information on reward options. Must specify reward and option, e.g. `Rewards.Reward[1].Option[1].Text`
<!--dt-desc-end-->

## Members
<!--dt-members-start-->
### {{ renderMember(type='string', name='Text') }}

:   Returns name of the indicated option if available; else NULL

### {{ renderMember(type='bool', name='Selected') }}

:   Returns TRUE if the indicated option is available and selected; otherwise FALSE

### {{ renderMember(type='bool', name='Select') }}

:   Selects the specified option. TRUE if available and able to select; otherwise FALSE

### {{ renderMember(type='int', name='ItemCount') }}

:   Returns number of items (right-side list) for the specified reward and selected option if available. Otherwise NULL.

### {{ renderMember(type='RewardOptionItem', name='Item', params='#|name') }}

:   Returns the appropriate item by index or name; else returns NULL

<!--dt-members-end-->

## Examples
<!--dt-examples-start-->
`${Rewards.Reward[1].Option[1].Text}`
`${Rewards.Reward[5].Option[2].Selected}`
`${Rewards.Reward[1].Option[1].Item[7]}`
<!--dt-examples-end-->

<!--dt-linkrefs-start-->
[bool]: ../macroquest/reference/data-types/datatype-bool.md
[int]: ../macroquest/reference/data-types/datatype-int.md
[rewardoptionitem]: datatype-rewardoptionitem.md
[string]: ../macroquest/reference/data-types/datatype-string.md
<!--dt-linkrefs-end-->
