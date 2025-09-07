---
tags:
  - command
---

# /mqrewards

## Syntax

<!--cmd-syntax-start-->
```eqcommand
/mqrewards [help | reward <reward> | option <option> | claim]
```
<!--cmd-syntax-end-->

## Description

<!--cmd-desc-start-->
Allows you to select and claim a reward, as well as reference the help text.
<!--cmd-desc-end-->

## Options

`help`
:   Displays a list of command options

`reward <reward>`
:   Opens reward window and selects the specified reward (tab) if exists. May be name (string) or 1-based number. Error if no appropriate match found.

`option <option>`
:   Selects the specified option for the currently selected reward (tab). May be name (string) or 1-based number. Error if no appropriate match found.

`claim`
:   Claims the currently selected reward and option.

## Examples

`/rewards reward 3`
:   Selects the third reward (tab).

`/rewards reward "Essence of Dragons"`
:   Selects the first reward titled "Essence of Dragons".

`/rewards option 3`
:   Selects the third option in the reward option list.

`/rewards option "Collection Item Dispenser"`
:   Selects the option labeled "Collection Item Dispenser".

## See also

- [/rewards](../everquest/commands/cmd-rewards.md)