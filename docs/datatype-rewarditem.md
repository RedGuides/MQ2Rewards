---
tags:
  - datatype
---
# `RewardItem`

<!--dt-desc-start-->
Gives information about the specified reward, including number of options, items, and availability. (must specify reward, e.g. `Rewards.Reward[1].Text`)
<!--dt-desc-end-->

## Members
<!--dt-members-start-->
### {{ renderMember(type='string', name='Text') }}

:   Returns name of the indicated reward if available; else NULL

### {{ renderMember(type='bool', name='Selected') }}

:   Returns TRUE if indicated reward is available and selected; else FALSE

### {{ renderMember(type='bool', name='Select') }}

:   Selects the specified reward. TRUE if available and able to select; otherwise FALSE.

### {{ renderMember(type='bool', name='Claim') }}

:   Claims the specified reward if available and option (if any) selected. TRUE if able to claim; otherwise FALSE

### {{ renderMember(type='int', name='Options') }}

:   Returns number of options for the specified reward if available. Otherwise NULL.

### {{ renderMember(type='RewardOption', name='Option', params='#|name') }}

:   Returns the appropriate option by index or name; else returns NULL

### {{ renderMember(type='int', name='Items') }}

:   Returns number of items (right-side list) for the specified reward and selected option if available. Otherwise NULL.

### {{ renderMember(type='RewardOptionItem', name='Item', params='#|name') }}

:   Returns the appropriate item by index or name; else returns NULL

<!--dt-members-end-->

## Examples
<!--dt-examples-start-->
`${Rewards.Reward[1].Text}`  
`${Rewards.Reward[3].Item[2]}`
<!--dt-examples-end-->

<!--dt-linkrefs-start-->
[bool]: ../macroquest/reference/data-types/datatype-bool.md
[int]: ../macroquest/reference/data-types/datatype-int.md
[rewardoption]: datatype-rewardoption.md
[rewardoptionitem]: datatype-rewardoptionitem.md
[string]: ../macroquest/reference/data-types/datatype-string.md
<!--dt-linkrefs-end-->
