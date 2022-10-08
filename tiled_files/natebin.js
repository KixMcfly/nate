var MAP_PATH = "maps";

var customMapFormat = {
    name: "Nathyn's Room Chunk Format",
    extension: "nat",

    write: function(map, fileName) {
		
		var sfb = new BinaryFile (fileName, BinaryFile.WriteOnly);
		
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

		//Lookup table for tileset assets
		var TSAssets = [];
		
		//Lookup for object sprite assets
		var objAssets = [];
		
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
						let tsId = null;
						let ts = null;
						let flags = null;
						if (tn >= 0){
							ts = layer.tileAt (x, y).asset.image;
							tsId = addAsset (TSAssets, ts);
							
							//Check if tile has a FLAGS prop type
							let flagsProp = layer.tileAt (x, y).property("FLAGS");
							flags = (flagsProp == undefined) ? 0 : flagsProp.value;
						}else{
							tn = 65535;
						}
						
						curLayer.data.push ({id: tn, ts: tsId, flags:flags});
						
					}
				}
	
            }else if (layer.isObjectLayer){
				
				var numOfObjects = layer.objects.length;
				var curLayer = {type: "objects", data: []};
				
				for (var ii = 0; ii < numOfObjects; ii++){
					let ox = layer.objects[ii].x;
					let oy = layer.objects[ii].y;
					let klass, data = undefined;
					
					//If object is derived from tile, grab class
					//name from that
					klass = layer.objects[ii].className;
					if (klass == ""){
						klass = layer.objects[ii].tile.className;
						props = layer.objects[ii].tile.properties ();
					}else
						props = layer.objects[ii].properties ();
					
					switch (klass){
						case "ENEMY":
							data = {};
							data.attacks = [];
							data.name = props.STAT.value.name;
							data.health = props.STAT.value.health;
							data.item = props.STAT.value.item;
							data.money = props.STAT.value.money;
							let afn = layer.objects[ii].tile.imageFileName;
							data.imageID = addAsset (objAssets, afn);
							
							//Remove STAT prop
							delete props.STAT;
							
							if (!data.money)
								data.money = 0;
								
							if (!data.item)
								data.item = 0;

							//Iterate attacks
							for (att in props){
								let ca = {};
								
								ca.name = att;
								ca.dam = props[att].value.dam;
								ca.desc = props[att].value.desc;
								ca.prob = props[att].value.prob;
								
								if (!ca.dam)
									ca.dam = 0;
								
								data.attacks.push (ca);
							}
							
							break;
						case "CHGROOM":
							data = {};
							data.name = props.name;
							data.x = props.x;
							data.y = props.y;
							break;
						case "VENDING":
							data = [];
							
							//20 potential items can be in a vending machine.
							for (let ci = 1; ci <= 20; ci++){
								let item = "ITEM";
								item += (ci <= 9) ? "0" + ci : ci;
								
								if (props[item] == undefined)
									data.push (0);
								else
									data.push (props[item].value);
							}
							
							break;
						default:
							tiled.log ("GENERAL OBJECT TYPE: " + klass);
							
					}
					
					curLayer.data.push ({klass: klass, data: data, x:ox, y:oy});

				}
			}else{
				tiled.log ("Dunno what layer type this is.");
			}
			
			layers.push (curLayer);
        }
        
        //Write tileset assets to lookup
        sfb.write (Uint8Array.from ([TSAssets.length]).buffer);
		for (let i = 0; i < TSAssets.length; i++){
			sfb.write (strLen (TSAssets[i]));
			sfb.write (strCharCodes (TSAssets[i]));
			tiled.log ("ADDING TILESET ASSETS: " + TSAssets[i]);
			
		}
		
		//Write object assets to lookup
		sfb.write (Uint8Array.from ([objAssets.length]).buffer);
		for (let i = 0; i < objAssets.length; i++){
			sfb.write (strLen (objAssets[i]));
			sfb.write (strCharCodes (objAssets[i]));
			tiled.log ("ADDING OBJECT ASSETS: " + objAssets[i]);
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

						//Object type to binary
						sfb.write (strLen (od.klass));
						sfb.write (strCharCodes (od.klass));
						
						//Write object X Y coord
						sfb.write (Uint16Array.from ([od.x, od.y]).buffer);
						
						switch (od.klass){
							case "ENEMY":
							
								//Image ref
								sfb.write (Uint16Array.from ([od.data.imageID]).buffer);
							
								//enemy name
								sfb.write (strLen (od.data.name));
								sfb.write (strCharCodes (od.data.name));
								
								//health
								sfb.write (Uint16Array.from ([od.data.health]).buffer);
								
								//money
								sfb.write (Uint16Array.from ([od.data.money]).buffer);
								
								//item
								sfb.write (Uint16Array.from ([od.data.item]).buffer);
								
								//write num of attacks
								let na = od.data.attacks.length;
								sfb.write (Uint8Array.from ([na]).buffer);
								
								//write attacks
								for (let ca = 0; ca < na; ca++){
									
									let att = od.data.attacks[ca];
									
									//attack name
									sfb.write (strLen (att.name));
									sfb.write (strCharCodes (att.name));
									
									//attack description
									sfb.write (strLen (att.desc));
									sfb.write (strCharCodes (att.desc));
									
									//attack damage
									sfb.write (Uint16Array.from ([att.dam]).buffer);
									
									//attack probablity
									sfb.write (Uint8Array.from ([att.prob]).buffer);
								}
								
								break;
							case "CHGROOM":
								//room to change to
								sfb.write (strLen (od.data.name));
								sfb.write (strCharCodes (od.data.name));
								
								//Write dest X Y coord
								sfb.write (Uint16Array.from ([od.data.x, od.data.y]).buffer);
							
								break;
							case "VENDING":
								//Write vending machine items
								sfb.write (Uint16Array.from (od.data).buffer);
								break;
							default:
							
						}
					}
				break;
				default:
					tiled.warn ("Unknown layer type: " + layers[i].type);
			}
		}
        
        //Write binary to disk
        sfb.commit ();
        
        //write map file to DAT
        fileToDAT ("nate", fileName);       
    }
}

//Register as custom tool in Tiled
tiled.registerMapFormat("custom", customMapFormat)

function strLen (str){
	
	//Add one to string length to include NULL terminator
	return Uint8Array.from ([str.length + 1]).buffer;
}

function addAsset (assets, fn){
	
	let found = false, i, n;
	
	fn = fn.toUpperCase ();
	n = fn.lastIndexOf('/');
	fn = fn.substring(n + 1);
	fn.replace (".", "_");
	
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

function fileToDAT (DATFileName, filename){

	let filebase = FileInfo.fileName (filename);
	let fileCheck = filebase.toUpperCase();
	let DATop;
	
	let proc = new Process();
	
	if (fileCheck.indexOf (".NAT") != -1)
		DATop = "DATaddMAP";
	else if (fileCheck.indexOf (".BMP") != -1)
		DATop = "DATaddBMP";
	
	var rc;
	switch (tiled.platform){
		case "windows":
			rc = proc.exec ("powershell.exe", [DATop, DATFileName, filename] );
			break;
		case "linux":
			rc = proc.exec ("pwsh", ["-C", DATop, DATFileName, filename] );
			break;
		default:
			throw new Error ("UNSUPPORTED PLATFORM: " + tiled.platform);
	}
	
	tiled.log (proc.readStdOut ());
       
	if (rc != 0)
		tiled.log ("Failed to add " + fileName + " to " + DATFileName + "!");
	else
		tiled.log ("Successfully added " + filebase + " to " + DATFileName + "!");
		
	proc.close ();
}
