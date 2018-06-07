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

## Main Layout
Bottom right two keys gave me terrible forearm pain so left them unmapped for a while.
Experimenting with having them for jumping around my text editor.
Jump is Shift Enter for [Jumpy](https://atom.io/packages/jumpy),
F2 lets you set and jump to bookmarks in a file but I don't use it a ton.

I am not loving CMSP very much as I often end up Cmd-A selecting everything in a window
and overwriting it. Not a huge hassle but annoying. Might just make left command work.
```
┌────┬────┬────┬────┬────┬────┐  ┌────┬────┬────┬────┬────┬────┐
│TAB │ Q  │ W  │ E  │ R  │ T  │  │ Y  │ U  │ I  │ O  │ P  │ =  │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│CTL"│ A  │SYMS│NUMD│ARWF│ G  │  │ H  │ J  │ K  │ L  │ ;  │CTL'│
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│SFT(│ Z  │ X  │ C  │ V  │ B  │  │ N  │ M  │ ,  │ .  │ /  │SFT)│
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│RGB`│ ESC│ [  │HYP<│OPT{│CMBS│  │CMSP│OPT}│HYP>│ ]  │ F2 │JUMP│
└────┴────┴────┴────┴────┴────┘  └────┴────┴────┴────┴────┴────┘
```
All modifiers are hold/tap so:
* CMBS: hold for left command, tap for backspace
* CMSP: hold for right command, tap for space (nulls A so you don't hit Cmd-A when you mean to type a space)
* CTL": hold for left control, tap for double quote "
* CTL': hold for right control, tap for single quote '
* SFT(: hold for left shift, tap for opening paren (
* SFT): hold for right shift, tap for closing paren )
* OPT{: hold for left option, tap for opening brace {
* OPT}: hold for right option, tap for closing brace }
* HYP<: hold for left hyper, tap for opening angle <
* HYP>: hold for right hyper, tap for closing angle >

## Arrows
Very common for me to move down an autocomplete dropdown and hit enter to select,
so put it here.

```
┌────┬────┬────┬────┬────┬────┐  ┌────┬────┬────┬────┬────┬────┐
│    │    │    │    │    │    │  │    │    │    │    │    │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│CAPS│    │    │    │    │    │  │LEFT│DOWN│ UP │RGHT│    │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │    │    │    │    │  │    │    │    │    │    │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│RSET│    │    │    │    │    │  │ENT │    │    │    │    │    │
└────┴────┴────┴────┴────┴────┘  └────┴────┴────┴────┴────┴────┘
```

## RGB
```
┌────┬────┬────┬────┬────┬────┐  ┌────┬────┬────┬────┬────┬────┐
│    │    │SMOD│ HUI│ SAI│ VAI│  │    │    │    │    │    │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │MODE│ HUD│ SAD│ VAD│  │    │    │    │    │    │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │    │    │    │    │  │    │    │    │    │    │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │TOGL│    │    │    │PLN │  │    │    │    │    │    │    │
└────┴────┴────┴────┴────┴────┘  └────┴────┴────┴────┴────┴────┘
```
### Numbers
In keeping with how sometimes dash acts like a space
```
┌────┬────┬────┬────┬────┬────┐  ┌────┬────┬────┬────┬────┬────┐
│    │    │    │    │    │    │  │    │ 7  │ 8  │ 9  │ +  │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │    │    │    │    │  │    │ 4  │ 5  │ 6  │ -  │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │    │    │    │    │  │    │ 1  │ 2  │ 3  │ *  │ M0 │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │    │    │    │    │  │ -  │ 0  │ .  │ENT │ /  │ M1 │
└────┴────┴────┴────┴────┴────┘  └────┴────┴────┴────┴────┴────┘
```
### Symbols
Again Underscore is space sometimes so that's why it's here.
Rarely use volume controls.
CAPP is capture part of the screen and CAPW is capture the whole screen.
```
┌────┬────┬────┬────┬────┬────┐  ┌────┬────┬────┬────┬────┬────┐
│    │    │    │    │    │    │  │    │ &  │ *  │ \  │ |  │CAPW│
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │    │    │    │    │  │ '  │ $  │ %  │ ^  │ _  │CAPP│
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │    │    │    │    │  │ `  │ !  │ @  │ #  │    │    │
├────┼────┼────┼────┼────┼────┤  ├────┼────┼────┼────┼────┼────┤
│    │    │    │    │    │    │  │ _  │ ~  │    │MUTE│VOLD│VOLU│
└────┴────┴────┴────┴────┴────┘  └────┴────┴────┴────┴────┴────┘
```
