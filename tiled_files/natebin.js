var MAP_PATH = "maps";

var customMapFormat = {
    name: "Nathyn's Room Chunk Format",
    extension: "nat",

    write: function(map, fileName) {
		
		var sfb = new BinaryFile (fileName, BinaryFile.WriteOnly);
		
		var proc = new Process();
		
		//Write file header
		sfb.write (strCharCodes ("NATE"));
		
		//Get map name property		
		var mapName = map.property ("name");
		
		if (mapName == undefined){
			tiled.warn ("Map " + tiled.activeAsset.fileName + " must have a name!");
			return;
		}
		
		//Length of map name and string
		sfb.write (strLen (mapName));
		sfb.write (strCharCodes (mapName));
		
		//Map width and height
		var mDim = Uint16Array.from ([map.width, map.height]).buffer;
		sfb.write (mDim);
		
		//Tile width and height
		var tWidthHeight = Uint8Array.from ([map.tileWidth, map.tileHeight]).buffer;
		sfb.write (tWidthHeight);
		
		/*
		 *	Here comes the crazy part. We need to hold 
		 *  a lookup table for all tilesets used in all
		 *  layers.
		 */

		//Lookup table for all layers assets
		var mapAssets = [];
		
		//Holds  all layer data, including object layers
		var layers = [];

		//Interate layers
        for (var i = 0; i < map.layerCount; i++) {
            var layer = map.layerAt(i);

            if (layer.isTileLayer) {
				
				var curLayer = {type: "tiles", data: []};
				
                for (y = 0; y < layer.height; y++){
                    for (x = 0; x < layer.width; x++){
						
						//Write current tile Id
						var tn = layer.cellAt (x, y).tileId;
								
						//Get tileset used for this tile, add it to the global
						//lookup table
						var tsId = null;
						var ts = null;
						var flags = null;
						if (tn >= 0){
							ts = layer.tileAt (x, y).asset.image;
							ts = getTSName (ts);
							tsId = addAsset (mapAssets, ts);
							
							//Check if tile has a FLAGS prop type
							var flagsProp = layer.tileAt (x, y).property("FLAGS");
							flags = (flagsProp == undefined) ? 0 : flagsProp.value;
						}else{
							tn = 65535;
						}
						
						curLayer.data.push ({id: tn, ts: tsId, flags:flags});
						
						//tiled.log (flags.toString(2).padStart (8, "0"));
						
					}
				}
	
            }else if (layer.isObjectLayer){
				
				var numOfObjects = layer.objects.length;
				var curLayer = {type: "objects", data: []};
				
				for (var ii = 0; ii < numOfObjects; ii++){
					var ox = layer.objects[ii].x;
					var oy = layer.objects[ii].y;
					var props, klass;
					
					//If object is derived from tile, grab class
					//name from that
					klass = layer.objects[ii].className;
					if (klass == ""){
						klass = layer.objects[ii].tile.className;
						props = layer.objects[ii].tile.properties ();
					}else
						props = layer.objects[ii].properties ();
					
					//X Y coords of object
					sfb.write (Uint16Array.from ([ox, oy]).buffer);
						
					switch (klass){
						case "ENEMY":
							
							let name = props.STAT.value.name;
							let health = props.STAT.value.health;
							let item = props.STAT.value.item;
							let money = props.STAT.value.money;
							let afn = layer.objects[ii].tile.imageFileName;
							afn = getTSName (afn);
							let imageID = addAsset (mapAssets, afn);
							
							if (!money)
								money = 0;
								
							if (!item)
								item = 0;
							
							sfb.write (strLen (name));
							sfb.write (strCharCodes (name));
							sfb.write (Uint8Array.from ([imageID]).buffer);
							sfb.write (Uint16Array.from ([health, item, money]).buffer);
							tiled.log (`ENEMY ${name} ${health} ${item} ${money} ${imageID}`);
							break;
						case "CHGROOM":
							tiled.log (`CHGROOM ${props.name} ${props.x} ${props.y}`);
							break;
						case "ELEV_BUTT":
							tiled.log ("Writing ELEV_BUTT");
							tiled.log (`CHGROOM ${props.name} ${props.x} ${props.y}`);
							break;
						case "VENDING":
							tiled.log ("Writing VENDING");
							break;
						case "ITEMBOX":
						case "STAT":
							
							break;
						default:
							tiled.log ("UNKNOWN OBJECT TYPE");
							
					}
					
					//tiled.log ("FFFFFFFFFFFFFFF: " +  layer.objects[ii].className);
					
					////If object is derived from a tile
					//if (layer.objects[ii].tile){
					//	props = layer.objects[ii].tile.properties();
					//	type = layer.objects[ii].tile.className;
					//	let afn = layer.objects[ii].tile.imageFileName;
					//	afn = getTSName (afn);
					//	props.imageID = addAsset (mapAssets, afn);
					////If object does not have a tile association
					//}else{
					//	type = layer.objects[ii].className;
					//	props = layer.objects[ii].properties();
					//}
					//
					////remove uneeded props from properties
					//var modProps = [];
					//
					//var co = 0;
					//for (var prop in props){
					//	
					//	if (typeof props[prop] == "object"){
					//		modProps.push ({data: props[prop].value, type: props[prop].typeName});
					//		
					//	}else{
					//		let data = {};
					//		data[prop] = props[prop];
					//		modProps.push ({data: data, type: prop});
					//	}
					//	
					//	co++;
					//}
					//
					//curLayer.data.push ({type: type, props: modProps, x:ox, y:oy});
				}
			}else{
				tiled.log ("Dunno what layer type this is.");
			}
			
			layers.push (curLayer);
        }
        
        //Write tileset asset lookup
        sfb.write (Uint8Array.from ([mapAssets.length]).buffer);
        for (var i = 0; i < mapAssets.length; i++){
			
			sfb.write (strLen (mapAssets[i]));
			sfb.write (strCharCodes (mapAssets[i]));
			
			//Save bitmap to DAT file
			var BMPAbs = "./images/" + (mapAssets[i].replace ("_", "."));
			
			var rc;
			switch (tiled.platform){
				case "windows":
					rc = proc.exec ("powershell.exe", ["DATaddBMP", "nate", BMPAbs] );
					break;
				case "linux":
					rc = proc.exec ("pwsh", ["-C", "DATaddBMP", "nate", BMPAbs] );
					break;
				default:
					throw new Error ("UNSUPPORTED PLATFORM: " + tiled.platform);
			}
			
			tiled.log (proc.readStdOut ());
        
			if (rc != 0)
				tiled.log ("Failed to add bmp " + fileName + " to DAT file!..");
			
		}
  
		//Number of layers
		var nLayers = Uint8Array.from ([map.layerCount]).buffer;
		sfb.write (nLayers);
  
        //Write layer data to buffer
        for (var i = 0; i < layers.length; i++){
			
			switch (layers[i].type){
				case "tiles":
					
					//Layer type (0 = tiles)
					sfb.write(Uint8Array.from ([0]).buffer);
					
					for (var ct = 0; ct < layers[i].data.length; ct++){
						var td = layers[i].data[ct];
						/*
							Null tiles will be 0xFFFF, no way in hell
							would someone need more than 65534 tiles in
							a tileset!
						*/
						
						//tiled.log (`ID: ${td.id}`);
						sfb.write (Uint16Array.from ([td.id]).buffer);
						if (td.id != 65535){
							sfb.write (Uint8Array.from ([td.ts]).buffer);
							sfb.write (Uint8Array.from ([td.flags]).buffer);
						}
					}
				break;
				case "objects":
					
					//Layer type  (1 = objects)
					sfb.write (Uint8Array.from ([1]).buffer);
					
					//Number of objects
					sfb.write (Uint8Array.from ([layers[i].data.length]).buffer);
					
					//Iterate and write objects
					for (var co = 0; co < layers[i].data.length; co++){
						var od = layers[i].data[co];

						sfb.write (strLen (od.type));
						sfb.write (strCharCodes (od.type));
						sfb.write (Uint32Array.from([od.x, od.y]).buffer);
						
						//Write number of props object has
						var np = layers[i].data[co].props.length;
						sfb.write (Uint8Array.from ([np]).buffer);
						
						var nw = 0;
						
						//interate and write object props
						for (let cp = 0; cp < np; cp++){
							let prop = layers[i].data[co].props[cp];
							let type = layers[i].data[co].props[cp].type;

							type = type.toUpperCase ();

							//Write prop type
							sfb.write (strLen (type));
							sfb.write (strCharCodes (type));
							
							//write prop to binary
							for (data in prop.data)
								writeProp (sfb, prop.data[data]);
	
							nw++;
						}
						
						tiled.log ("Number od props written for " + type + " :" + nw);
					}
				break;
				default:
					tiled.warn ("Unknown layer type: " + layers[i].type);
			}
		}
        
        //Write binary to disk
        sfb.commit ();
        
        //Run Allegro DAT script to add map
		var rc;
		switch (tiled.platform){
			case "windows":
				rc = proc.exec ("powershell.exe", ["DATaddMAP", "nate", fileName] );
				break;
			case "linux":
				rc = proc.exec ("pwsh", ["-C", "DATaddMAP", "nate", fileName] );
				break;
			default:
				throw new Error ("UNSUPPORTED PLATFORM: " + tiled.platform);
		}
        tiled.log (proc.readStdOut ());
        
        if (rc != 0)
			tiled.log ("Failed to add map " + fileName + " to DAT file!");
    }
}

//Register as custom tool in Tiled
tiled.registerMapFormat("custom", customMapFormat)

function writeProp (buff, prop){
	
	tiled.log ("      : " + prop);
	
	switch (typeof prop){
		case "number":
			buff.write (Uint16Array.from([prop]).buffer);
			break;
		case "string":
			buff.write (strLen (prop));
			buff.write (strCharCodes (prop));
			tiled.log ("WRITING " + prop);
			break;
		case "boolean":
			buff.write (Uint8Array.from([(prop ? 1 : 0)]).buffer);
			break;
		default:
			tiled.log ("WARNING: Unexpected data type: " + typeof prop);
	}
}

function strLen (str){
	
	//Add one to string length to include NULL terminator
	return Uint8Array.from ([str.length + 1]).buffer;
}

function addAsset (assets, fn){
	
	//Check if asset is already in lookup
	var found = false, i;
	for (i = 0; i < assets.length; i++ ){
		if (assets[i] == fn ){
			found = true;
			break;
		}
	}
	
	if (found == false)
		assets.push (fn);
		
	return i;
	
}

function strCharCodes (str){
	
	var cs = [];
	
	for (var i = 0; i < str.length; i++)
		cs.push (str.charCodeAt(i));
	
	//Add null terminator for C
	cs.push (0);
		
	return Uint8Array.from (cs).buffer;
}

function getMapName( mapAbs ){
	mapAbs = mapAbs.toUpperCase ();
	var n = mapAbs.lastIndexOf('/');
	var DATId = mapAbs.substring(n + 1);
	
	return DATId.replace (".TMX", "_NAT");
}

function getTSName (tsAbs){
	tsAbs = tsAbs.toUpperCase ();
	var n = tsAbs.lastIndexOf('/');
	var DATId = tsAbs.substring(n + 1);
	
	return DATId.replace (".", "_");
}
