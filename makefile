obj = main.obj tileset.obj map.obj hroom.obj draw.obj sprite.obj inv.obj vend.obj sound.obj temp.obj elev.obj node.obj tools.obj
bin = nate.exe

CC = wcc386
CFLAGS = -zq -3s -s -bt=dos
LD = wlink library alleg.lib system causeway

$(bin): $(obj)
    $(LD) name $@ file { $(obj) }

.c.obj:
    $(CC) -fo=$@ $(CFLAGS) $<

clean: .symbolic
    rm *.obj
    rm $(bin)
    rm *.ERR
    rm *.TXT
