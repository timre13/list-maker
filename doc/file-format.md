# List Maker File Format
File Format version: 3

Rules:
+ Documents are made of tags
+ Boards start with a *name*, a *color* and a *last modification date*
+ Boards can contain Lists
+ Lists start with a *name*
+ Lists can contain Items
+ Items start with a *name* and contain a boolean whether the task is *done*
+ Everythings ends with `end`

New in version 2:
+ Spaces and tabs are ignored at the beginning of the lines

## Example

~~~ Lua
version 2
board My Board 1
    #ff7700
    1602271164
    list My List One
        item My Item 1
            false
        end
        item My Item 2
            false
        end
        item My Item 3
            true
        end
        item My Item 4
            false
        end
    end
    list My List Two
        item My Item 1
            false
        end
        item My Item 2
            true
        end
    end
end
board My Board 2
    cyan
    2345234325
    list My List 1
        item My Item 1
            true
        end
    end
end
board Board 3
    blue
    345646463456
    list List 1
    end
end
board Board 4
    green
    2345234523
end
~~~

---

Document version: 3.0

---

**TODO: Finish this**