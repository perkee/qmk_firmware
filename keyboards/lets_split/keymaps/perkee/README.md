Perkee's Let's Split Layout
============================

### Changes from Hexwire's layout

- General changes:
    - This is a highly opinionated macOS board now
    - Since emacs shortcuts C-a C-e C-f C-b C-p C-n are pretty universal, no arrows on main layer.
    - Qwerty only since Colemak and Dvorak are spooky

- Main layer
    - Every modifier has a tap behavior so US brackets are available from this layer
    - swap space and backspace
    - See diagram, honestly
    - XO is command when held, backspace when tapped
    - X1 is command when held, space when tapped
    - X2 is control when held, enter when tapped
- Lower layer
    - numpad number layout
    - two dedicated screencap key (CAPP: cmd shift 4) and (CAPW: cmd shift 3)
    - also a reset key so you don't have to hit pins with a paper clip.
- Upper layer
    - vi arrow layout and some symbols
    - command plus numbers

Pretty graphical layout of keyboard [here](http://www.keyboard-layout-editor.com/#/gists/6f49fdce9c9dfb3b8bbb6c55a300b318)

## Layouts

### QWERTY
```
,----+----+----+----+----+----.    ,----+----+----+----+----+----.
|TAB | Q  | W  | E  | R  | T  |    | Y  | U  | I  | O  | P  | =  |
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|CTL"| A  | S  | D  | F  | G  |    | H  | J  | K  | L  | ;  | X2 |
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|SFT(| Z  | X  | C  | V  | B  |    | N  | M  | ,  | .  | /  |SFT)|
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|  ` |ESC |HYP[|LOW<|OPT{| X0 |    | X1 |OPT}|RAS>|HYP>| -  | '  |
`----+----+----+----+----+----'    `----+----+----+----+----+----'
```

### Raise
```
|----+----+----+----+----+----.    ,----+----+----+----+----+----.
|    |    |CMD7|CMD8|CMD9|    |    | ^  | @  | #  | $  | %  | \  |
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|    |CMD0|CMD4|CMD5|CMD6|    |    |LEFT|DOWN| UP |RGHT| '  | |  |
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|    |    |CMD1|CMD2|CMD3|    |    |  0 | &  | *  | !  | `  | ~  |
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|    |    |CMD0|    |    |    |    |    |    |    |    |    |    |
`----+----+----+----+----+----'    `----+----+----+----+----+----'
```

### Lower
```
|----+----+----+----+----+----.    ,----+----+----+----+----+----.
|CAPW| _  | 7  | 8  | 9  | =  |    |    |    |    |    |    |RSET|
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|CAPP| -  | 4  | 5  | 6  | +  |    |WH L|WH D|WH U|WH R|    |    |
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|MUTE| *  | 1  | 2  | 3  | /  |    |    |    |    |    |    |    |
|----+----+----+----+----+----|    |----+----+----+----+----+----|
|VOLD|VOLU| TAB| 0  | ENT|    |    |    |    |    |    |    |    |
`----+----+----+----+----+----'    `----+----+----+----+----+----'
```
