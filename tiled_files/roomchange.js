var tileSel = false;
var selSrcTile = null;
var selSrcLayer = null;
var selSrcMap = null;

var roomChangeTool = {
    name: "Room Change",
    icon: "chgroom.png",

    mouseMoved: function(x, y, modifiers) {

		var tileW = this.map.tileWidth;
        var tileH = this.map.tileHeight;
        var tileX = Math.floor(x / this.map.tileWidth);
        var tileY = Math.floor(y / this.map.tileHeight);
        var mWidth = this.map.width * tileW;
		var mHeight = this.map.height * tileH;
        var statusText = "";
      
		if (x < mWidth && y < mHeight && y > 0 && x > 0){
			if (!tileSel){
				statusText += "Select room change tile: ";
				
			}else{
				statusText += selSrcMap + " -> ";
				statusText += getMapName (this.map.fileName) + " ";
				
			}
			
			statusText += "X: " + tileX;
			statusText += ", Y: " + tileY;
		}
        
        this.statusInfo = statusText;
    },

    mousePressed: function(button, x, y, modifiers) {
        this.pressed = true
        
        var tileW = this.map.tileWidth;
        var tileH = this.map.tileHeight;
        var tileX = Math.floor(x / this.map.tileWidth);
        var tileY = Math.floor(y / this.map.tileHeight);
        var mWidth = this.map.width * tileW;
		var mHeight = this.map.height * tileH;
		
		var found = false;
				
		if (button == 1){
			if (x < mWidth && y < mHeight && x > 0 && y > 0){
			
				//Set current layer to "triggers"
				for (var i = 0, found = false; i < this.map.layers.length; i++){
					if (this.map.layers[i].name == "triggers"){
						this.map.currentLayer = this.map.layers[i]
						this.map.currentLayer.horizontalOffset = 0;
						this.map.currentLayer.verticalOffset = 0;
						found = true;
					}
				}
				
				//Create trigger object group layer and make it active
				if (!found){
					tiled.alert ('No "Triggers" object layer found! Creating one now.');
					var trigLayer = new ObjectGroup();
					trigLayer.name = "triggers";
					trigLayer.horizontalOffset = 0;
					trigLayer.verticalOffset = 0;
					this.map.addLayer (trigLayer);
					this.map.currentLayer = trigLayer;
				}
				
				if (!tileSel){
		
					var roomChg = new MapObject();
					
					roomChg.width = tileW;
					roomChg.height = tileH;
					roomChg.shape = roomChg.Rectangle
					roomChg.type = "CHGROOM"
					roomChg.x = Math.floor (x / tileW) * tileW;
					roomChg.y = Math.floor (y / tileH) * tileH;
					this.map.currentLayer.addObject (roomChg);
					selSrcTile = roomChg;
					selSrcLayer = this.map.currentLayer;
					selSrcMap = getMapName (this.map.fileName);
					
					tileSel = true;
				}else{
					
					//Convert to naming convention that resides in Allegro DAT file
					var DATId = getMapName (this.map.fileName);
					
					selSrcTile.name = DATId;
					selSrcTile.setProperty ("name", DATId);
					selSrcTile.setProperty ("x", tileX);
					selSrcTile.setProperty ("y", tileY);
					tileSel = false;
					this.statusInfo = "Room Change Complete!";
					
				}
			}
		}else if (button == 2 && tileSel){
			selSrcLayer.removeObject (selSrcTile);
			tileSel = false;
			selSrcTile = null;
			this.statusInfo = "Cancelled!";
		}
    },

    mouseReleased: function(button, x, y, modifiers) {
        this.pressed = false
    }
}

tiled.registerTool("RoomChange", roomChangeTool)

function getMapName( mapAbs ){
	var mapAbs = mapAbs.toUpperCase ();
	var n = mapAbs.lastIndexOf('/');
	var DATId = mapAbs.substring(n + 1);
	
	return DATId.replace (".TMX", "") + "_NAT";
}
