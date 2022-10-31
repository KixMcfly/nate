obj = main.obj tileset.obj map.obj draw.obj sprite.obj text.obj inv.obj vend.obj temp.obj elev.obj node.obj tools.obj
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
    rm *.err
