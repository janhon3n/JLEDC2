var SerialPort = require("serialport");
var port = new SerialPort("COM10", {baudRate: 9600});
var portReady = false;

var newValue = false;
var rgb = [253,100,0];

port.on('open', function(){
	console.log("Serial port open");
});
port.on('error', function(err){
	console.log(err);
});


//public
//color in form [r,g,b]
function setColor(color){
	for(var i = 0; i < 3; i++){
		rgb[i] = color[i] * 253;
	}
}
function getColor(){
	var color = [];
	for(var i = 0; i < 3; i++){
		color[i] = rgb[i] / 253;
	}
	return color;
}

function isReady(){
	return port.isOpen();
}


//private
function transmit(){
	if(port.isOpen()){
		var byteArray = new Uint8Array(5);
		byteArray[0] = 254;
		byteArray[4] = 255;
		for(var i = 0; i < 3; i++){
			byteArray[i+1] = rgb[i];
		}
		port.write(Buffer.from(byteArray));
	}
}

setInterval(function(){
	transmit();
}, 100);

module.exports.isReady = isReady;
module.exports.setColor = setColor;
module.exports.getColor = getColor;