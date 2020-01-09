# Aoiduino
Aoi enviroment for Arduino. Provies command line interface and flexible enhancements for your product.

Supported boards are Arduino Uno, Spresense Ast. Below functions were implemented.

- 2020/01/09
  - All boards
    - Baudrate is 115200.
    - Command line result is json format for future release.
    - Command line help (Type 'help *' to see all functions or Type 'help className' like 'help Uno').
    - Command line history (Max 10 count), Press up or down key to view.

### Operation example on Spresense Ast
```
[    0:01.889] Loaded : Shell
[    0:01.890] Loaded : Ast
[    0:01.890] *** Welcome to aoiduino, aoi environment for arduino. ***
[    0:01.890] help * : Show all usages.
[    0:01.891] exit   : Back to login.

aoiduino login : in
password : italy

# help Shell
[
{
  "className": "Shell",
  "methods": [
    {
      "name": "echo",
      "usage": "echo message"
    },
    {
      "name": "help",
      "usage": "help (classname|*)"
    }
  ]
}
]
# 
```
